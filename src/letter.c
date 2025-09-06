#include "letter.h"
#include "animation.h"
#include "card.h"
#include "custom_arrays.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"
#include <stddef.h>
#include <string.h>

Letter *CreateLetter(Arena *arena) {
  size_t numberOfCards = 6;
  Letter *letter = arena_alloc(arena, sizeof(Letter));

  letter->pos = (Vector2){0, -200};
  letter->slideSpeed = 450;
  letter->showState = INVISIBLE;
  letter->current_card_index = 0;
  letter->cards;
  letter->sounds.intro = LoadSound("assets/bad_penny.ogg");
  letter->sounds.tear = LoadSound("assets/tear.ogg");
  letter->sounds.boatBop = LoadSound("assets/boat bop.ogg");
  letter->sounds.ahhh = LoadSound("assets/ahhh.ogg");

  letter->numberOfCards = numberOfCards;
  AnimationParams animationParams = {{0, 0, 200, 120}, 26, 0.03, false};
  letter->animation = CreateAnimation(arena, animationParams);

  // Envelope
  CardData cardData;
  strcpy(cardData.cardEnvelopeData.title, "Til Ørnulf...");
  strcpy(cardData.cardEnvelopeData.subtitle, "This is a subtitle text");
  Texture2D envelopeTex = LoadTexture("assets/envelope.png");
  CardParams envelopeParams = {CARD_ENVELOPE, cardData, envelopeTex};

  CardArray_Push(letter->cards, CreateCard(envelopeParams));

  // Second Card
  CardData secondCardData;
  secondCardData.cardImageData.texture = LoadTexture("assets/rory.png");
  secondCardData.cardImageData.textColour = WHITE;
  // strcpy(secondCardData.cardImageData.text, "This is an image caption");
  CardParams secondCardParams = {CARD_IMAGE, secondCardData,
                                 LoadTexture("assets/inner_card.png")};

  CardArray_Push(letter->cards, CreateCard(secondCardParams));

  // Askoy
  CardData boatCardData;
  boatCardData.askoyBoatData.oceanBackground =
      LoadTexture("assets/askoy_ocean.png");
  boatCardData.askoyBoatData.boatTex = LoadTexture("assets/Boat.png");
  boatCardData.askoyBoatData.hytteOne = LoadTexture("assets/Hytte1.png");
  boatCardData.askoyBoatData.hytteTwo = LoadTexture("assets/Hytte2.png");
  boatCardData.askoyBoatData.state = BOAT;
  boatCardData.askoyBoatData.timer = 0.0f;
  boatCardData.askoyBoatData.boatPosX = 0.0f;
  boatCardData.askoyBoatData.boatVelX = 0.0f;
  boatCardData.askoyBoatData.boatAccelX = 0.0f;
  boatCardData.askoyBoatData.boatTopSpeed = 90.0f;
  boatCardData.askoyBoatData.accel = 200.0f;
  boatCardData.askoyBoatData.friction = 100.0f;
  CardParams boatCardParams = {CARD_BOAT, boatCardData,
                               LoadTexture("assets/inner_card.png")};
  CardArray_Push(letter->cards, CreateCard(boatCardParams));

  // Ma Da Card
  CardData madaCardData;
  madaCardData.cardImageData.texture = LoadTexture("assets/mada.png");
  madaCardData.cardImageData.textColour = WHITE;
  // strcpy(secondCardData.cardImageData.text, "This is an image caption");
  CardParams madaCardParams = {CARD_IMAGE, madaCardData,
                               LoadTexture("assets/inner_card.png")};

  CardArray_Push(letter->cards, CreateCard(madaCardParams));

  // end Card
  CardData endCardData;
  endCardData.cardImageData.texture = LoadTexture("assets/endcard.jpeg");
  endCardData.cardImageData.textColour = WHITE;
  // strcpy(secondCardData.cardImageData.text, "This is an image caption");
  CardParams endCardParams = {CARD_IMAGE, endCardData,
                              LoadTexture("assets/inner_card.png")};

  CardArray_Push(letter->cards, CreateCard(endCardParams));

  // End Card
  // TODO: Move card background to a shared handle or pointer
  CardData firstCardData;
  strcpy(firstCardData.cardTextData.text, "Det var det!");
  CardParams firstCardParams = {CARD_TEXT, firstCardData,
                                LoadTexture("assets/inner_card.png")};
  CardArray_Push(letter->cards, CreateCard(firstCardParams));

  return letter;
}

void UpdateLetter(Letter *letter, float dt) {
  UpdateAnimation(letter->animation);

  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();

  Vector2 desired_dimensions = GetScaledUpDimensions(
      200, CardArray_At(letter->cards, 0)->texture.height);

  int center_y = (int)(screen_height / 2) - (int)(desired_dimensions.y / 2);

  letter->pos.x = (int)(screen_width / 2) - (int)(200 / 2);
  switch (letter->showState) {
  case INVISIBLE:
    letter->showState = ENTER;
    break;
  case ENTER:
    if (letter->pos.y < center_y) {
      letter->pos.y += letter->slideSpeed * dt;
    } else {
      letter->showState = VISIBLE;
      // Set the first card (envelope) as visible
      CardArray_At(letter->cards, 0)->showState = VISIBLE;
    }
    break;
  case VISIBLE:
    if (letter->current_card_index < letter->numberOfCards - 1) {
      if (CardArray_At(letter->cards, letter->current_card_index + 1)
              ->showState == ENTER) {
        UpdateCard(letter,
                   CardArray_At(letter->cards, letter->current_card_index + 1),
                   dt);
      }
    }

    UpdateCard(letter, CardArray_At(letter->cards, letter->current_card_index),
               dt);
    break;
  case EXIT:
    break;
  case DONE:
    break;
  }
}

void DrawLetter(Letter *letter) {
  if (letter->current_card_index < letter->numberOfCards - 1) {
    if (CardArray_At(letter->cards, letter->current_card_index + 1)
            ->showState == ENTER) {
      DrawCard(letter,
               CardArray_At(letter->cards, letter->current_card_index + 1));
    }
  }

  DrawCard(letter, CardArray_At(letter->cards, letter->current_card_index));
}

void DrawLetterUi(Letter *letter) {
  int screenHeight = GetScreenHeight();
  if (CardArray_At(letter->cards, letter->current_card_index)->showState ==
          VISIBLE &&
      CardArray_At(letter->cards, letter->current_card_index)->isFinished) {
    DrawText("Trykk til å forsette...", 40, screenHeight - 30, 20, DARKGRAY);
  }

  // TODO: Call Card specific Ui
}
