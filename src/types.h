#ifndef TYPES_H
#define TYPES_H

#include "animation.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>
#include <stdint.h>

/*
 * Cards
 */
typedef enum {
  CARD_ENVELOPE,
  CARD_TEXT,
  CARD_IMAGE,
  CARD_BOAT,
} CardType;

typedef struct {
  char text[256];
} CardTextData;

typedef struct {
  char title[128];
  char subtitle[128];
} CardEnvelopeData;

typedef struct {
  Texture2D texture;
  char text[128];
  Color textColour;
} CardImageData;

typedef enum { BOAT, ARRIVED_ASKOY, RELAXED_ASKOY } AskoyState;

typedef struct {
  Texture2D oceanBackground;
  Texture2D boatTex;
  Texture2D hytteOne;
  Texture2D hytteTwo;
  AskoyState state;
  float_t boatPosX;
  float_t boatAccelX;
  float_t boatVelX;
  float_t boatTopSpeed;
  float_t accel;
  float_t friction;
  float timer;
} AskoyBoatData;

typedef union {
  CardTextData cardTextData;
  CardEnvelopeData cardEnvelopeData;
  CardImageData cardImageData;
  AskoyBoatData askoyBoatData;
} CardData;

typedef struct {
  Texture2D texture;
  Vector2 pos;
  CardType cardType;
  CardData cardData;
  ShowState showState;
  bool isFinished;
  Rectangle contentRec;
} Card;

typedef struct {
  Card *items;
  int32_t length;
  int32_t capacity;
} CardArray;

typedef struct {
  Sound intro;
  Sound tear;
  Sound boatBop;
  Sound ahhh;
} Sounds;

/*
 * Letter
 */
typedef struct {
  Vector2 pos;
  float slideSpeed;
  ShowState showState;
  Animation *animation; // TODO: To be moved inside the envelope data
  int current_card_index;
  int numberOfCards;
  CardArray *cards;
  Sounds sounds;
} Letter;

#endif
