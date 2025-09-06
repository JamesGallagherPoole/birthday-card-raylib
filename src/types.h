#ifndef TYPES_H
#define TYPES_H

#include "animation.h"
#include "raylib.h"
#include "utils.h"
#include <math.h>
#include <stdint.h>

typedef struct Letter Letter;
typedef struct Card Card;

/*
 * Cards
 */
typedef enum {
  CARD_ENVELOPE,
  CARD_TEXT,
  CARD_IMAGE,
  CARD_BOAT,
} CardType;

typedef enum { BOAT, ARRIVED_ASKOY, RELAXED_ASKOY } AskoyState;

struct Card {
  void (*Update)(Letter *letter, Card *card, float dt);
  void (*Draw)(Letter *letter, Card *card);
  Texture2D texture;
  Vector2 pos;
  ShowState showState;
  bool isFinished;
  Rectangle contentRec;
};

typedef struct {
  Card card;
  char text[256];
} TextCard;

typedef struct {
  Card card;
  char title[128];
  char subtitle[128];
} EnvelopeCard;

typedef struct {
  Card card;
  Texture2D texture;
  char text[128];
  Color textColour;
} ImageCard;

typedef struct {
  Card card;
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
} BoatCard;

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
struct Letter {
  Vector2 pos;
  float slideSpeed;
  ShowState showState;
  Animation *animation; // TODO: To be moved inside the envelope data
  int current_card_index;
  int numberOfCards;
  Card *cards[4];
  Sounds sounds;
};

typedef struct {
  void (*UpdateC)(Letter *letter, Card *card);
} CardProcs;

#endif
