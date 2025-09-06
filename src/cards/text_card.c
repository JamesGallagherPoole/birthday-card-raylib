#include "text_card.h"
#include "arena.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"
#include <string.h>

Card *CreateTextCard(Arena *arena, CardParams params) {
  TextCard *textCard = arena_alloc(arena, sizeof(TextCard));

  textCard->card.texture = params.cardTexture;
  textCard->card.contentRec = (Rectangle){0};
  textCard->card.showState = INVISIBLE;
  textCard->card.isFinished = false;

  strcpy(textCard->text, "I NEED TO BE PASSED IN");

  return (Card *)textCard;
}

void UpdateTextCard(Letter *letter, Card *card) {}

void DrawTextCard(Letter *letter, Card *card) {}
