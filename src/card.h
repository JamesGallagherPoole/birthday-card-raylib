#ifndef CARD_H
#define CARD_H

#include "raylib.h"
#include "types.h"

typedef struct {
  CardType cardType;
  CardData cardData;
  Texture2D cardTexture;
} CardParams;

Card CreateCard(CardParams cardParams);
void UpdateCard(Letter *letter, Card *card, float dt);
void DrawCard(Letter *letter, Card *card);

#endif
