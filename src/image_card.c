#include "image_card.h"
#include "raymath.h"

void DrawImageCard(Letter *letter, Card *card) {
  Vector2 globalPos = Vector2Add(letter->pos, card->pos);

  Texture2D tex = card->cardData.cardImageData.texture;

  // Fit the IMAGE into the content box
  Rectangle dst =
      FitInto(card->contentRec, (float)tex.width, (float)tex.height);
  Rectangle src = (Rectangle){0, 0, (float)tex.width, (float)tex.height};
  DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0, WHITE);

  // Caption
  Vector2 textPos =
      Vector2Add(globalPos, ScalePointBasedOnRef(200, (Vector2){0, 100}));
  DrawText(card->cardData.cardImageData.text, textPos.x, textPos.y, 20,
           card->cardData.cardImageData.textColour);
}
