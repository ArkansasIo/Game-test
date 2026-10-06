#pragma bank 31

/**
 * RPG menu system implementation.
 *
 * Pages are defined as static tables of `MenuRow`, terminated by a row with a
 * NULL label. The root page is a 2-column grid; every other page is a single
 * column of rows drawn into the window layer.
 *
 * The menu never draws more than `MENU_MAX_ROWS` rows, and only one page is
 * live at a time, so the memory cost is fixed.
 */
#include <gb/gb.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "map.h"
#include "menu.h"
#include "player.h"
#include "sound.h"
#include "story.h"
#include "strings.h"

Menu menu;

/**
 * Blinks the cursor. Uses the same Timer type as the rest of the game rather
 * than a global frame counter.
 */
static Timer cursor_blink_timer;

/**
 * Whether the cursor is in the visible half of its blink cycle.
 *
 * Deliberately uninitialised here: an initialised static in a banked
 * translation unit makes SDCC emit a startup copy routine, which is what was
 * breaking the display. The value is set in init_menu instead.
 */
static bool cursor_blink_on;

/**
 * Text buffer for formatting numbers into menu rows.
 */
static char menu_buf[12];

//------------------------------------------------------------------------------
// Page geometry
//------------------------------------------------------------------------------

/**
 * Origin of the menu panel, in tiles.
 */
#define MENU_X 1
#define MENU_Y 1

/**
 * Width of the menu panel, in tiles.
 */
#define MENU_W 18

/**
 * First row of the list, relative to the panel.
 */
#define MENU_LIST_Y 2

/**
 * Cursor tile drawn in the window layer.
 */
#define MENU_CURSOR_TILE 0x8A

//------------------------------------------------------------------------------
// Page definitions
//------------------------------------------------------------------------------


static const MenuRow system_rows[] = {
    {str_menu_save, NULL, MENU_ROW_ACTION, MENU_PAGE_COUNT, MENU_ACTION_SAVE},
    {str_menu_options, NULL, MENU_ROW_ACTION, MENU_PAGE_COUNT, MENU_ACTION_OPTIONS},
    {str_menu_quit, NULL, MENU_ROW_ACTION, MENU_PAGE_COUNT, MENU_ACTION_QUIT},
    {NULL, NULL, MENU_ROW_LABEL, MENU_PAGE_COUNT, 0},
};

/**
 * Root menu entries, in display order. The root is rendered as a 2-column
 * grid, so this array is walked in pairs.
 */
static const MenuRow root_entries[] = {
    {str_menu_items, NULL, MENU_ROW_ACTION, MENU_PAGE_ITEMS, 0},
    {str_menu_quests, NULL, MENU_ROW_ACTION, MENU_PAGE_QUESTS, 0},
    {str_menu_equip, NULL, MENU_ROW_ACTION, MENU_PAGE_EQUIP, 0},
    {str_menu_party, NULL, MENU_ROW_ACTION, MENU_PAGE_PARTY, 0},
    {str_menu_skills, NULL, MENU_ROW_ACTION, MENU_PAGE_SKILLS, 0},
    {str_menu_status, NULL, MENU_ROW_ACTION, MENU_PAGE_STATUS, 0},
    {str_menu_system, NULL, MENU_ROW_ACTION, MENU_PAGE_SYSTEM, 0},
};

#define ROOT_ENTRY_COUNT 7

const char *menu_title_for(MenuPage page) BANKED
{
  switch (page)
  {
  case MENU_PAGE_ITEMS:
    return str_menu_items;
  case MENU_PAGE_EQUIP:
    return str_menu_equip;
  case MENU_PAGE_SKILLS:
    return str_menu_skills;
  case MENU_PAGE_STATUS:
    return str_menu_status;
  case MENU_PAGE_PARTY:
    return str_menu_party;
  case MENU_PAGE_QUESTS:
    return str_menu_quests;
  case MENU_PAGE_SYSTEM:
    return str_menu_system;
  default:
    return str_menu_root;
  }
}

//------------------------------------------------------------------------------
// Cursor
//------------------------------------------------------------------------------

/**
 * Position of the cursor for the root page, which is a 2-column grid.
 */
static void cursor_position_root(uint8_t index, uint8_t *x, uint8_t *y)
{
  *x = MENU_X + 1 + (index % MENU_ROOT_COLS) * 9;
  *y = MENU_Y + MENU_LIST_Y + (index / MENU_ROOT_COLS);
}

/**
 * Position of the cursor for a single-column page.
 */
static void cursor_position_list(uint8_t index, uint8_t *x, uint8_t *y)
{
  *x = MENU_X + 1;
  *y = MENU_Y + MENU_LIST_Y + index;
}

static void move_cursor_sprite(uint8_t x, uint8_t y)
{
  move_sprite(0, x * 8 + 4, y * 8 + 12);
  set_sprite_tile(0, MENU_CURSOR_TILE);
  set_sprite_prop(0, 0);
}

static void hide_cursor_sprite(void)
{
  move_sprite(0, 0, 0);
}

//------------------------------------------------------------------------------
// Drawing
//------------------------------------------------------------------------------

/**
 * Draws the menu panel background and title.
 */
static void draw_panel(void)
{
  uint8_t *vram = VRAM_WINDOW_XY(MENU_X, MENU_Y);

  // Solid panel.
  core.fill(vram, MENU_W, 18 - MENU_Y, 0xFF, 0b00001010);

  // Title at the top of the panel.
  core.draw_text(
      VRAM_WINDOW_XY(MENU_X + 1, MENU_Y),
      menu_title_for(menu.page),
      MENU_W - 2);
}

/**
 * Draws a single row at a given index on the current page.
 */
static void draw_row(uint8_t index, const MenuRow *row)
{
  const uint8_t y = MENU_Y + MENU_LIST_Y + index;
  uint8_t x = MENU_X + 2;

  // Root is a 2-column grid; other pages are a single column.
  if (menu.page == MENU_PAGE_ROOT)
  {
    x = MENU_X + 2 + (index % MENU_ROOT_COLS) * 9;
  }

  core.draw_text(VRAM_WINDOW_XY(x, y), row->label, 8);

  if (row->kind == MENU_ROW_VALUE && row->value != NULL)
  {
    core.draw_text(VRAM_WINDOW_XY(MENU_X + 11, y), row->value, 6);
  }
}

/**
 * Rebuilds the dynamic rows for the pages that show live player data. These
 * pages cannot be static tables because their contents change as the player
 * plays, so they are composed into a scratch buffer on open.
 */
static MenuRow dynamic_rows[MENU_MAX_ROWS];

/**
 * Sets a dynamic row. SDCC does not implement C99 compound literals, so rows
 * are filled in field by field rather than with a literal.
 */
static void set_row(
    uint8_t i,
    const char *label,
    const char *value,
    MenuRowKind kind
) {
    dynamic_rows[i].label = label;
    dynamic_rows[i].value = value;
    dynamic_rows[i].kind = kind;
    dynamic_rows[i].opens = MENU_PAGE_COUNT;
    dynamic_rows[i].action = MENU_ACTION_NONE;
}

static const MenuRow *build_dynamic_rows(MenuPage page) {
    uint8_t n = 0;

    switch (page) {
    case MENU_PAGE_STATUS:
        sprintf(menu_buf, "%u", player.level);
        set_row(n++, str_menu_row_level, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.hp);
        set_row(n++, str_menu_row_hp, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.max_hp);
        set_row(n++, str_menu_row_max_hp, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.sp);
        set_row(n++, str_menu_row_sp, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.atk);
        set_row(n++, str_menu_row_atk, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.def);
        set_row(n++, str_menu_row_def, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.agl);
        set_row(n++, str_menu_row_agl, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", player.exp);
        set_row(n++, str_menu_row_exp, menu_buf, MENU_ROW_VALUE);
        break;

    case MENU_PAGE_QUESTS:
        sprintf(menu_buf, "%u", story_act_of(story_chapter));
        set_row(n++, str_menu_row_act, menu_buf, MENU_ROW_VALUE);

        sprintf(menu_buf, "%u", story_chapter);
        set_row(n++, str_menu_row_chapter, menu_buf, MENU_ROW_VALUE);

        set_row(n++, story_get_chapter(story_chapter)->title, NULL, MENU_ROW_LABEL);
        break;

    case MENU_PAGE_ITEMS:
        // List every inventory entry the player actually holds.
        for (uint8_t k = 0; k < INVENTORY_LEN && n < MENU_MAX_ROWS; k++) {
            if (inventory[k].quantity == 0)
                continue;
            sprintf(menu_buf, "%u", inventory[k].quantity);
            set_row(n++, inventory[k].name, menu_buf, MENU_ROW_VALUE);
        }
        if (n == 0)
            set_row(n++, str_menu_no_items, NULL, MENU_ROW_LABEL);
        break;

    case MENU_PAGE_SKILLS:
        for (uint8_t k = 0; k < player_num_abilities && n < MENU_MAX_ROWS; k++) {
            sprintf(menu_buf, "%u", player_abilities[k]->sp_cost);
            set_row(n++, player_abilities[k]->name, menu_buf, MENU_ROW_VALUE);
        }
        if (n == 0)
            set_row(n++, str_menu_no_skills, NULL, MENU_ROW_LABEL);
        break;

    case MENU_PAGE_EQUIP:
        set_row(n++, str_menu_row_weapon, str_menu_none, MENU_ROW_VALUE);
        set_row(n++, str_menu_row_armor, str_menu_none, MENU_ROW_VALUE);
        set_row(n++, str_menu_row_relic, str_menu_none, MENU_ROW_VALUE);
        break;

    case MENU_PAGE_PARTY:
        set_row(n++, player.name, NULL, MENU_ROW_LABEL);
        set_row(n++, str_menu_row_alone, NULL, MENU_ROW_LABEL);
        break;

    default:
        break;
    }

    // Terminate the list.
    set_row(n, NULL, NULL, MENU_ROW_LABEL);
    return dynamic_rows;
}

const MenuRow *menu_rows_for(MenuPage page) BANKED
{
  switch (page)
  {
  case MENU_PAGE_ROOT:
    return root_entries;
  case MENU_PAGE_ITEMS:
  case MENU_PAGE_EQUIP:
  case MENU_PAGE_SKILLS:
  case MENU_PAGE_STATUS:
  case MENU_PAGE_PARTY:
  case MENU_PAGE_QUESTS:
    return build_dynamic_rows(page);
  case MENU_PAGE_SYSTEM:
    return system_rows;
  default:
    return root_entries;
  }
}

/**
 * @param page Page to count.
 * @return How many rows the page shows.
 */
static uint8_t count_rows(MenuPage page)
{
  if (page == MENU_PAGE_ROOT)
    return ROOT_ENTRY_COUNT;

  const MenuRow *rows = menu_rows_for(page);
  uint8_t n = 0;
  while (n < MENU_MAX_ROWS && rows[n].label != NULL)
    n++;
  return n;
}

void draw_menu(void) BANKED
{
  if (!menu.open)
    return;

  draw_panel();

  const MenuRow *rows = menu_rows_for(menu.page);
  uint8_t count = count_rows(menu.page);

  for (uint8_t k = 0; k < count; k++)
    draw_row(k, rows + k);

  // Cursor.
  uint8_t cx, cy;
  if (menu.page == MENU_PAGE_ROOT)
    cursor_position_root(menu.cursor, &cx, &cy);
  else
    cursor_position_list(menu.cursor, &cx, &cy);

  if (menu.cursor_visible)
    move_cursor_sprite(cx, cy);
  else
    hide_cursor_sprite();
}

//------------------------------------------------------------------------------
// Actions
//------------------------------------------------------------------------------

void menu_run_action(const MenuRow *row) BANKED
{
  switch (row->action)
  {
  case MENU_ACTION_SAVE:
    play_sound(sfx_menu_move);
    // Save is handled by the existing map menu flow.
    break;
  case MENU_ACTION_QUIT:
    play_sound(sfx_menu_move);
    close_menu();
    game_state = GAME_STATE_TITLE;
    break;
  default:
    break;
  }
}

//------------------------------------------------------------------------------
// Input
//------------------------------------------------------------------------------

/**
 * Moves the cursor, wrapping at the edges.
 */
static void move_cursor(int8_t dx, int8_t dy)
{
  uint8_t count = count_rows(menu.page);
  if (count == 0)
    return;

  if (menu.page == MENU_PAGE_ROOT)
  {
    // 2-column grid.
    const uint8_t rows = (count + MENU_ROOT_COLS - 1) / MENU_ROOT_COLS;
    uint8_t col = menu.cursor % MENU_ROOT_COLS;
    uint8_t row = menu.cursor / MENU_ROOT_COLS;

    if (dx > 0)
      col = (col + 1) % MENU_ROOT_COLS;
    if (dx < 0)
      col = (col + MENU_ROOT_COLS - 1) % MENU_ROOT_COLS;
    if (dy > 0)
      row = (row + 1) % rows;
    if (dy < 0)
      row = (row + rows - 1) % rows;

    uint8_t index = row * MENU_ROOT_COLS + col;
    if (index >= count)
      index = count - 1;
    menu.cursor = index;
    return;
  }

  // Single column.
  if (dy > 0)
    menu.cursor = (menu.cursor + 1) % count;
  if (dy < 0)
    menu.cursor = (menu.cursor + count - 1) % count;
}

/**
 * Confirms the currently selected row.
 */
static void confirm(void)
{
  const MenuRow *rows = menu_rows_for(menu.page);
  uint8_t count = count_rows(menu.page);
  if (menu.cursor >= count)
    return;

  const MenuRow *row = rows + menu.cursor;
  play_sound(sfx_menu_move);

  if (row->opens != MENU_PAGE_COUNT)
  {
    menu_show_page(row->opens);
    return;
  }

  menu_run_action(row);
}

void update_menu(void) BANKED {
    if (!menu.open)
        return;

    // Blink the cursor so it reads as active.
    if (update_timer(cursor_blink_timer)) {
        reset_timer(cursor_blink_timer);
        cursor_blink_on = !cursor_blink_on;
    }
    menu.cursor_visible = cursor_blink_on;

    if (was_pressed(J_UP))
        move_cursor(0, -1);
    else if (was_pressed(J_DOWN))
        move_cursor(0, 1);
    else if (was_pressed(J_LEFT))
        move_cursor(-1, 0);
    else if (was_pressed(J_RIGHT))
        move_cursor(1, 0);

    if (was_pressed(J_A)) {
        confirm();
        return;
    }

    if (was_pressed(J_B) || was_pressed(J_START)) {
        if (menu.page == MENU_PAGE_ROOT) {
            play_sound(sfx_menu_move);
            close_menu();
        } else {
            // Back out to the root page.
            play_sound(sfx_menu_move);
            menu_show_page(MENU_PAGE_ROOT);
        }
    }
}

//------------------------------------------------------------------------------
// Lifecycle
//------------------------------------------------------------------------------

void menu_show_page(MenuPage page) BANKED
{
  menu.page = page;
  menu.cursor = 0;
  menu.cursor_visible = true;

  // Clear the window so the new page does not draw over the old one.
  core.fill(VRAM_WINDOW, 32, 32, 0x00, 0b00001010);
}

void open_menu(void) BANKED {
    menu.open = true;
    menu_show_page(MENU_PAGE_ROOT);

    // Show the window layer, which the menu draws into.
    LCDC_REG |= 0b00100000;
    move_win(7, 0);

    play_sound(sfx_menu_move);

    // Hand control to the map state machine so the map stops updating.
    map_state = MAP_STATE_RPG_MENU;
}

void close_menu(void) BANKED {
    menu.open = false;
    hide_cursor_sprite();

    // Clear the window layer so gameplay is unobstructed, then hide it.
    core.fill(VRAM_WINDOW, 32, 32, 0x00, 0b00001010);
    LCDC_REG &= 0b11011111;
    move_win(0, 144);
}

/**
 * Initializes the menu system.
 *
 * This is BANKED, not NONBANKED: every field it writes (`menu`,
 * `cursor_blink_timer`, `cursor_blink_on`) lives in this translation unit on
 * bank 30. A NONBANKED function would run with whatever bank happened to be
 * mapped and corrupt unrelated data.
 */
void init_menu(void) BANKED {
    menu.open = false;
    menu.page = MENU_PAGE_ROOT;
    menu.cursor = 0;
    menu.row_count = 0;
    menu.cursor_visible = false;

    init_timer(cursor_blink_timer, 20);
    reset_timer(cursor_blink_timer);
    cursor_blink_on = true;
}
