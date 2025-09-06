#ifndef TEXT_CARD_H
#define TEXT_CARD_H

#include "card.h"
#include "types.h"

Card *CreateTextCard(Arena *arena, CardParams params);
void UpdateTextCard(Letter *letter, Card *card);
void DrawTextCard(Letter *letter, Card *card);

#endif // !TEXT_CARD_H
