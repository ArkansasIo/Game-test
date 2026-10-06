#pragma bank 7

/**
 * Chapter data.
 *
 * Act I is written out in full. Acts II-V have their chapter titles and
 * opening beats in place; the remaining beats are left as single lines so the
 * sequencer has something valid to walk, and can be filled in without touching
 * any code.
 *
 * All text is original to this project. See `docs/SETTING_BIBLE.md`.
 */
#include <gb/gb.h>
#include <stdint.h>

#include "core.h"
#include "story.h"
#include "strings.h"
#include "textbox.h"

    /**
     * Chapter table, indexed by chapter number - 1.
     */
    static const Chapter chapters[STORY_CHAPTERS] = {
        //---------------------------------------------------------------------------
        // ACT I - The Waking Dark
        //---------------------------------------------------------------------------
        {
            1, str_story_ch1_title, {str_story_ch1_b1, str_story_ch1_b2, str_story_ch1_b3, NULL}},
        {2, str_story_ch2_title, {str_story_ch2_b1, str_story_ch2_b2, str_story_ch2_b3, NULL}},
        {3, str_story_ch3_title, {str_story_ch3_b1, str_story_ch3_b2, str_story_ch3_b3, NULL}},
        {4, str_story_ch4_title, {str_story_ch4_b1, str_story_ch4_b2, str_story_ch4_b3, NULL}},
        {5, str_story_ch5_title, {str_story_ch5_b1, str_story_ch5_b2, str_story_ch5_b3, NULL}},
        {6, str_story_ch6_title, {str_story_ch6_b1, str_story_ch6_b2, str_story_ch6_b3, NULL}},
        {7, str_story_ch7_title, {str_story_ch7_b1, str_story_ch7_b2, str_story_ch7_b3, NULL}},
        {8, str_story_ch8_title, {str_story_ch8_b1, str_story_ch8_b2, str_story_ch8_b3, NULL}},
        {9, str_story_ch9_title, {str_story_ch9_b1, str_story_ch9_b2, str_story_ch9_b3, NULL}},
        {10, str_story_ch10_title, {str_story_ch10_b1, str_story_ch10_b2, str_story_ch10_b3, NULL}},

        //---------------------------------------------------------------------------
        // ACT II - The Iron Choir
        //---------------------------------------------------------------------------
        {11, str_story_ch11_title, {str_story_ch11_b1, NULL}},
        {12, str_story_ch12_title, {str_story_ch12_b1, NULL}},
        {13, str_story_ch13_title, {str_story_ch13_b1, NULL}},
        {14, str_story_ch14_title, {str_story_ch14_b1, NULL}},
        {15, str_story_ch15_title, {str_story_ch15_b1, NULL}},
        {16, str_story_ch16_title, {str_story_ch16_b1, NULL}},
        {17, str_story_ch17_title, {str_story_ch17_b1, NULL}},
        {18, str_story_ch18_title, {str_story_ch18_b1, NULL}},
        {19, str_story_ch19_title, {str_story_ch19_b1, NULL}},
        {20, str_story_ch20_title, {str_story_ch20_b1, NULL}},

        //---------------------------------------------------------------------------
        // ACT III - The Sunken Crown
        //---------------------------------------------------------------------------
        {21, str_story_ch21_title, {str_story_ch21_b1, NULL}},
        {22, str_story_ch22_title, {str_story_ch22_b1, NULL}},
        {23, str_story_ch23_title, {str_story_ch23_b1, NULL}},
        {24, str_story_ch24_title, {str_story_ch24_b1, NULL}},
        {25, str_story_ch25_title, {str_story_ch25_b1, NULL}},
        {26, str_story_ch26_title, {str_story_ch26_b1, NULL}},
        {27, str_story_ch27_title, {str_story_ch27_b1, NULL}},
        {28, str_story_ch28_title, {str_story_ch28_b1, NULL}},
        {29, str_story_ch29_title, {str_story_ch29_b1, NULL}},
        {30, str_story_ch30_title, {str_story_ch30_b1, NULL}},

        //---------------------------------------------------------------------------
        // ACT IV - The Nameless Throne
        //---------------------------------------------------------------------------
        {31, str_story_ch31_title, {str_story_ch31_b1, NULL}},
        {32, str_story_ch32_title, {str_story_ch32_b1, NULL}},
        {33, str_story_ch33_title, {str_story_ch33_b1, NULL}},
        {34, str_story_ch34_title, {str_story_ch34_b1, NULL}},
        {35, str_story_ch35_title, {str_story_ch35_b1, NULL}},
        {36, str_story_ch36_title, {str_story_ch36_b1, NULL}},
        {37, str_story_ch37_title, {str_story_ch37_b1, NULL}},
        {38, str_story_ch38_title, {str_story_ch38_b1, NULL}},
        {39, str_story_ch39_title, {str_story_ch39_b1, NULL}},
        {40, str_story_ch40_title, {str_story_ch40_b1, NULL}},

        //---------------------------------------------------------------------------
        // ACT V - The Dragon's Dream
        //---------------------------------------------------------------------------
        {41, str_story_ch41_title, {str_story_ch41_b1, NULL}},
        {42, str_story_ch42_title, {str_story_ch42_b1, NULL}},
        {43, str_story_ch43_title, {str_story_ch43_b1, NULL}},
        {44, str_story_ch44_title, {str_story_ch44_b1, NULL}},
        {45, str_story_ch45_title, {str_story_ch45_b1, NULL}},
        {46, str_story_ch46_title, {str_story_ch46_b1, NULL}},
        {47, str_story_ch47_title, {str_story_ch47_b1, NULL}},
        {48, str_story_ch48_title, {str_story_ch48_b1, NULL}},
        {49, str_story_ch49_title, {str_story_ch49_b1, NULL}},
        {50, str_story_ch50_title, {str_story_ch50_b1, str_story_ch50_b2, NULL}},
};

uint8_t story_chapter = 1;
uint8_t story_beat = 0;
bool story_active = false;

/**
 * Whether the textbox for the current beat has been opened yet.
 */
static bool beat_opened = false;

const Chapter *story_get_chapter(uint8_t chapter) BANKED
{
  if (chapter < 1 || chapter > STORY_CHAPTERS)
    return chapters;
  return chapters + (chapter - 1);
}

uint8_t story_act_of(uint8_t chapter) BANKED
{
  if (chapter < 1)
    return 1;
  if (chapter > STORY_CHAPTERS)
    return STORY_ACTS;
  return ((chapter - 1) / STORY_CHAPTERS_PER_ACT) + 1;
}

void story_play_chapter(uint8_t chapter) BANKED
{
  if (chapter < 1)
    chapter = 1;
  if (chapter > STORY_CHAPTERS)
    chapter = STORY_CHAPTERS;

  story_chapter = chapter;
  story_beat = 0;
  story_active = true;
  beat_opened = false;
}

void story_stop(void) BANKED
{
  story_active = false;
  story_beat = 0;
  beat_opened = false;
  if (textbox.state != TEXT_BOX_CLOSED)
    textbox.state = TEXT_BOX_CLOSING;
}

void story_advance(void) BANKED
{
  story_beat++;
  beat_opened = false;
}

void story_update(void) BANKED
{
  if (!story_active)
    return;

  const Chapter *chapter = story_get_chapter(story_chapter);
  const char *beat = chapter->beats[story_beat];

  // Ran off the end of the beat list: the chapter is finished.
  if (beat == NULL)
  {
    story_stop();
    return;
  }

  // Open the textbox for this beat the first time we reach it.
  if (!beat_opened)
  {
    textbox.open(beat);
    beat_opened = true;
  }

  textbox.update();

  // Advance on A once the textbox has finished printing.
  if (textbox.state == TEXT_BOX_OPEN && was_pressed(J_A))
    story_advance();
}
