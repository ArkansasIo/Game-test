/**
 * RPG menu system.
 *
 * A Pokémon-style menu: a root list that opens submenus, with a cursor the
 * player moves with the d-pad and confirms with A. Each submenu is a list of
 * rows; a row may be a plain label, a label with a value, or a selectable
 * action.
 *
 * The menu is drawn into the window layer so it can overlay gameplay without
 * disturbing the map. Only one menu page is live at a time, so the whole system
 * costs a fixed amount of RAM regardless of how many pages exist.
 *
 * Pages:
 *   ROOT      - Items / Equip / Skills / Status / Party / Quests / System
 *   ITEMS     - the player's inventory, usable outside battle
 *   EQUIP     - weapon and armour slots
 *   SKILLS    - the player's abilities
 *   STATUS    - level, HP/SP, attributes, experience
 *   PARTY     - roster of characters
 *   QUESTS    - chapter progress from the story system
 *   SYSTEM    - save, options, quit
 */
#ifndef _MENU_H
#define _MENU_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Maximum number of rows a menu page can show at once.
 */
#define MENU_MAX_ROWS 8

/**
 * Number of columns in the root menu (the root is laid out as a 2-wide grid,
 * like Pokémon's start menu).
 */
#define MENU_ROOT_COLS 2

/**
 * Identifies a menu page.
 */
typedef enum MenuPage
{
  MENU_PAGE_ROOT,
  MENU_PAGE_ITEMS,
  MENU_PAGE_EQUIP,
  MENU_PAGE_SKILLS,
  MENU_PAGE_STATUS,
  MENU_PAGE_PARTY,
  MENU_PAGE_QUESTS,
  MENU_PAGE_SYSTEM,
  MENU_PAGE_COUNT,
} MenuPage;

/**
 * What a menu row does.
 */
typedef enum MenuRowKind
{
  /**
   * A label with no action. Used for headings and read-only values.
   */
  MENU_ROW_LABEL,
  /**
   * A label the player can select. Selecting it may open a submenu or run an
   * action.
   */
  MENU_ROW_ACTION,
  /**
   * A label with a value shown on the right, e.g. "Level    12".
   */
  MENU_ROW_VALUE,
} MenuRowKind;

/**
 * A single row on a menu page.
 */
typedef struct MenuRow
{
  /**
   * Row label, drawn on the left.
   */
  const char *label;
  /**
   * Value drawn on the right. Only used by MENU_ROW_VALUE.
   */
  const char *value;
  /**
   * What kind of row this is.
   */
  MenuRowKind kind;
  /**
   * Page to open when this row is confirmed. MENU_PAGE_COUNT means "no page".
   */
  MenuPage opens;
  /**
   * Action id passed to the menu action handler. 0 means "no action".
   */
  uint8_t action;
} MenuRow;

/**
 * Action ids passed to `menu_run_action` for rows that do something rather
 * than open another page.
 */
typedef enum MenuAction {
  /**
   * No action; the row only opens a page or is read-only.
   */
  MENU_ACTION_NONE = 0,
  /**
   * Save the game.
   */
  MENU_ACTION_SAVE,
  /**
   * Open the options page.
   */
  MENU_ACTION_OPTIONS,
  /**
   * Return to the title screen.
   */
  MENU_ACTION_QUIT,
} MenuAction;

/**
 * The menu system state.
 */
typedef struct Menu {
  /**
   * Whether the menu is currently visible.
   */
  bool open;
  /**
   * Page currently being shown.
   */
  MenuPage page;
  /**
   * Selected row on the current page.
   */
  uint8_t cursor;
  /**
   * Number of rows on the current page.
   */
  uint8_t row_count;
  /**
   * Whether the cursor is currently drawn.
   */
  bool cursor_visible;
} Menu;

/**
 * The global menu instance.
 */
extern Menu menu;

/**
 * Initializes the menu system. Call once at startup.
 */
void init_menu(void) NONBANKED;

/**
 * Opens the menu at the root page.
 */
void open_menu(void) BANKED;

/**
 * Closes the menu.
 */
void close_menu(void) BANKED;

/**
 * Switches to a different page, resetting the cursor.
 * @param page Page to show.
 */
void menu_show_page(MenuPage page) BANKED;

/**
 * Runs one frame of menu input and animation. Call every frame while the menu
 * is open.
 */
void update_menu(void) BANKED;

/**
 * Draws the menu. Call from the VBLANK draw routine.
 */
void draw_menu(void) BANKED;

/**
 * @param page Page to inspect.
 * @return The rows for a page, terminated by a row with a NULL label.
 */
const MenuRow *menu_rows_for(MenuPage page) BANKED;

/**
 * @param page Page to inspect.
 * @return A human readable title for the page.
 */
const char *menu_title_for(MenuPage page) BANKED;

/**
 * Called when the player confirms an action row. Handles the built-in actions;
 * anything unrecognised is ignored.
 * @param row The row that was confirmed.
 */
void menu_run_action(const MenuRow *row) BANKED;

#endif
