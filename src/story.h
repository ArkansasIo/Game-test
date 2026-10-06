/**
 * Story system.
 *
 * The story is organised as five acts of ten chapters each, for fifty chapters
 * total. Each chapter is a sequence of story beats; each beat is one textbox.
 * The sequencer walks the beats, opening the textbox for each one and waiting
 * for the player to advance, so a chapter is authored as data rather than as
 * hand-written cutscene code.
 *
 * Chapter text lives in `strings.js` under the `story` namespace, so it goes
 * through the same line-wrapping and encoding pipeline as every other string
 * in the game.
 */
#ifndef _STORY_H
#define _STORY_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Number of acts in the game.
 */
#define STORY_ACTS 5

/**
 * Number of chapters in each act.
 */
#define STORY_CHAPTERS_PER_ACT 10

/**
 * Total number of chapters.
 */
#define STORY_CHAPTERS (STORY_ACTS * STORY_CHAPTERS_PER_ACT)

/**
 * Maximum number of text beats in a single chapter.
 */
#define STORY_MAX_BEATS 6

/**
 * A single chapter of the story.
 */
typedef struct Chapter
{
  /**
   * Global chapter number, 1..STORY_CHAPTERS.
   */
  uint8_t number;
  /**
   * Chapter title, shown when the chapter opens.
   */
  const char *title;
  /**
   * Story beats. Each entry is one textbox. A NULL entry ends the list.
   */
  const char *beats[STORY_MAX_BEATS];
} Chapter;

/**
 * The chapter the player has reached, 1..STORY_CHAPTERS.
 */
extern uint8_t story_chapter;

/**
 * Index of the beat currently being shown within the chapter.
 */
extern uint8_t story_beat;

/**
 * Whether a chapter is currently playing.
 */
extern bool story_active;

/**
 * @param chapter Global chapter number, 1..STORY_CHAPTERS.
 * @return The chapter, or the first chapter if the number is out of range.
 */
const Chapter *story_get_chapter(uint8_t chapter) BANKED;

/**
 * @param chapter Global chapter number.
 * @return Which act the chapter belongs to, 1..STORY_ACTS.
 */
uint8_t story_act_of(uint8_t chapter) BANKED;

/**
 * Begins playing a chapter from its first beat.
 * @param chapter Global chapter number, 1..STORY_CHAPTERS.
 */
void story_play_chapter(uint8_t chapter) BANKED;

/**
 * Advances to the next beat, or ends the chapter if it was the last one.
 */
void story_advance(void) BANKED;

/**
 * Called each frame while a chapter is playing. Opens the textbox for the
 * current beat and advances when the player presses A.
 */
void story_update(void) BANKED;

/**
 * Stops the current chapter.
 */
void story_stop(void) BANKED;

#endif
