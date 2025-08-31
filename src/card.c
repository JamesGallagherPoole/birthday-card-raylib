#include "card.h"
#include "animation.h"
#include "custom_arrays.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "utils.h"
#include <stdint.h>

Card CreateCard(CardParams cardParams) {
  Card card;

  card.cardType = cardParams.cardType;
  card.cardData = cardParams.cardData;
  card.texture = cardParams.cardTexture;
  card.pos = (Vector2){0, 0};
  card.showState = INVISIBLE;
  card.isFinished = false;

  return card;
}

void UpdateCard(Letter *letter, Card *card) {
  if (card->showState == DONE) {
    return;
  }
  int screen_height = GetScreenHeight();

  switch (card->cardType) {
  case CARD_ENVELOPE:
    if (card->showState == VISIBLE) {

      if (letter->current_card_index < letter->numberOfCards - 1) {
        CardArray_At(letter->cards, letter->current_card_index + 1)->showState =
            ENTER;
        // letter->cards[letter->current_card_index + 1]->showState = ENTER;
      }

      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlayAnimation(letter->animation);
        if (!card->isFinished) {
          PlaySound(letter->sounds.tear);
        }
      }

      if (IsKeyReleased(KEY_SPACE) ||
          IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        StopAnimation(letter->animation);
        PauseSound(letter->sounds.tear);
      }

      if (letter->animation->is_finished) {
        card->isFinished = true;
        StopSound(letter->sounds.tear);
      }
    }

    break;
  case CARD_TEXT:
    if (!card->isFinished) {
      card->isFinished = true;
    }
    break;
  case CARD_IMAGE:
    if (!card->isFinished) {
      card->isFinished = true;
    }
    break;
  case CARD_BOAT: {
    AskoyBoatData *data = &card->cardData.askoyBoatData;

    // TODO: Make this contentRec global and quit this duplication nonsense
    Vector2 globalPos = Vector2Add(letter->pos, card->pos);
    Vector2 desired_dimensions =
        GetScaledUpDimensions(200, card->texture.height);
    int half_padding = GetWindowPadding() / 2;
    Rectangle contentRec = (Rectangle){
        half_padding, globalPos.y, desired_dimensions.x, desired_dimensions.y};

    switch (data->state) {
    case BOAT: {

      // Todo: Duplicate Code Start - Refactor
      Vector2 globalPos = Vector2Add(letter->pos, card->pos);

      Vector2 desired_dimensions =
          GetScaledUpDimensions(200, card->texture.height);
      int half_padding = GetWindowPadding() / 2;
      Rectangle cardRect =
          (Rectangle){half_padding, globalPos.y, desired_dimensions.x,
                      desired_dimensions.y};
      int32_t inner_pad = 70;
      Rectangle content = (Rectangle){
          cardRect.x + inner_pad, cardRect.y + inner_pad,
          cardRect.width - 2 * inner_pad, cardRect.height - 2 * inner_pad};
      // Duplicate code end

      int maxX = content.width - (content.width / 2);

      if (data->boatPosX >= maxX && (IsKeyPressed(KEY_SPACE) ||
                                     IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        data->state = ARRIVED_ASKOY;
      }

      if ((IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) &&
          data->boatPosX < maxX) {
        if (!IsSoundPlaying(letter->sounds.boatBop)) {
          SetSoundVolume(letter->sounds.intro, 0.1);
          PlaySound(letter->sounds.boatBop);
        }
        data->boatVelX += data->accel * GetFrameTime();
      } else {
        if (IsSoundPlaying(letter->sounds.boatBop)) {
          PauseSound(letter->sounds.boatBop);
        }
        // Apply friction
        int32_t sign;
        if (data->boatVelX >= 0) {
          sign = 1;
        }
        if (data->boatVelX < 0) {
          sign = -1;
        }
        data->boatVelX -= sign * data->friction * GetFrameTime();
      }

      data->boatVelX = Clamp(data->boatVelX, 0, data->boatTopSpeed);
      data->boatPosX += data->boatVelX * GetFrameTime();
      break;
    }
    case ARRIVED_ASKOY:
      if (!IsSoundPlaying(letter->sounds.boatBop)) {
        StopSound(letter->sounds.boatBop);
      }
      data->timer += GetFrameTime();
      if (data->timer > 3.0f) {
        if (IsKeyPressed(KEY_SPACE) ||
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          data->state = RELAXED_ASKOY;
          data->timer = 0.0f;
        }
      }
      break;
    case RELAXED_ASKOY:
      if (!IsSoundPlaying(letter->sounds.ahhh)) {
        PlaySound(letter->sounds.ahhh);
      }
      data->timer += GetFrameTime();
      if (data->timer > 3.0f) {
        if (IsKeyPressed(KEY_SPACE) ||
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          card->isFinished = true;
          data->timer = 0.0f;
        }
      }
      break;
    }
  }
  }

  switch (card->showState) {
  case INVISIBLE:
  case ENTER:
    break;
  case VISIBLE:
    if (card->isFinished) {
      // Press again to dismiss
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        card->showState = EXIT;

        // Little jammed in
        if (card->cardType == CARD_ENVELOPE) {
          if (!IsSoundPlaying(letter->sounds.intro)) {
            SetSoundVolume(letter->sounds.intro, 0.5);
            PlaySound(letter->sounds.intro);
          }
        }

        if (letter->current_card_index < letter->numberOfCards - 1) {
          CardArray_At(letter->cards, letter->current_card_index + 1)
              ->showState = ENTER;
        }
      }
    }
    break;
  case EXIT:
    // Animate Out
    if (card->pos.y < screen_height + 150) {
      card->pos.y += letter->slideSpeed * GetFrameTime();
    } else {
      card->showState = DONE;

      if (letter->current_card_index < letter->numberOfCards - 1) {
        letter->current_card_index++;
        CardArray_At(letter->cards, letter->current_card_index)->showState =
            VISIBLE;
      }
    }
  case DONE:
    break;
  }
}

static Rectangle FitInto(Rectangle bounds, float srcW, float srcH) {
  if (srcW <= 0 || srcH <= 0)
    return (Rectangle){bounds.x, bounds.y, 0, 0};
  float sx = bounds.width / srcW;
  float sy = bounds.height / srcH;
  float s = (sx < sy) ? sx : sy; // contain
  float w = srcW * s;
  float h = srcH * s;
  float x = bounds.x + (bounds.width - w) * 0.5f;
  float y = bounds.y + (bounds.height - h) * 0.5f;
  return (Rectangle){x, y, w, h};
}

void DrawCard(Letter *letter, Card *card) {
  Vector2 globalPos = Vector2Add(letter->pos, card->pos);

  Vector2 desired_dimensions = GetScaledUpDimensions(200, card->texture.height);
  int half_padding = GetWindowPadding() / 2;
  Rectangle cardRect = (Rectangle){half_padding, globalPos.y,
                                   desired_dimensions.x, desired_dimensions.y};

  switch (card->cardType) {
  case CARD_ENVELOPE: {

    DrawTexturePro(card->texture,
                   letter->animation->frame_rec, // source frame
                   cardRect,                     // destination background area
                   (Vector2){0, 0}, 0, WHITE);
    Vector2 scaledPos = ScalePointBasedOnRef(200, (Vector2){0, 50});
    Vector2 titlePos =
        Vector2Add(globalPos, (Vector2){scaledPos.x, scaledPos.y});
    DrawText(card->cardData.cardEnvelopeData.title, titlePos.x, titlePos.y, 30,
             BLACK);
    break;
  }
  case CARD_TEXT: {
    Vector2 textPos =
        Vector2Add(globalPos, ScalePointBasedOnRef(200, (Vector2){0, 30}));
    DrawText(card->cardData.cardTextData.text, textPos.x, textPos.y, 50,
             DARKGRAY);
    break;
  }
  case CARD_IMAGE: {
    Texture2D tex = card->cardData.cardImageData.texture;

    // Inner content area inside the card
    int32_t inner_pad = 70;
    Rectangle content = (Rectangle){
        cardRect.x + inner_pad, cardRect.y + inner_pad,
        cardRect.width - 2 * inner_pad, cardRect.height - 2 * inner_pad};

    // Fit the IMAGE into the content box
    Rectangle dst = FitInto(content, (float)tex.width, (float)tex.height);
    Rectangle src = (Rectangle){0, 0, (float)tex.width, (float)tex.height};
    DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0, WHITE);

    // Caption
    Vector2 textPos =
        Vector2Add(globalPos, ScalePointBasedOnRef(200, (Vector2){0, 100}));
    DrawText(card->cardData.cardImageData.text, textPos.x, textPos.y, 20,
             card->cardData.cardImageData.textColour);
    break;
  case CARD_BOAT: {
    AskoyBoatData *data = &card->cardData.askoyBoatData;

    // Inner content
    int32_t inner_pad = 70;
    Rectangle content = (Rectangle){
        cardRect.x + inner_pad, cardRect.y + inner_pad,
        cardRect.width - 2 * inner_pad, cardRect.height - 2 * inner_pad};

    Texture2D tex = data->oceanBackground;
    Rectangle bgDst = FitInto(content, (float)tex.width, (float)tex.height);
    Rectangle bgSrc = (Rectangle){0, 0, (float)tex.width, (float)tex.height};

    switch (data->state) {
    case BOAT: {
      DrawTexturePro(tex, bgSrc, bgDst, (Vector2){0, 0}, 0, WHITE);

      Vector2 globalBoatPos = (Vector2){-data->boatPosX, 20};

      DrawTexturePro(data->boatTex,
                     (Rectangle){0, 0, (float)data->boatTex.width,
                                 (float)data->boatTex.height},
                     bgDst, globalBoatPos, 0, WHITE);

      if (card->showState == VISIBLE) {
        int maxX = content.width - (content.width / 2);
        if (data->boatPosX >= maxX) {
          DrawText("Trykk til å gå til hytten...", 40, GetScreenHeight() - 30,
                   20, DARKGRAY);
        } else {
          DrawText("Trykk til å kjøre til Askøy...", 40, GetScreenHeight() - 30,
                   20, DARKGRAY);
        }
      }
      break;
    }
    case ARRIVED_ASKOY:
      DrawTexturePro(data->hytteOne, bgSrc, bgDst, (Vector2){0, 0}, 0, WHITE);

      if (data->timer > 2.0f) {
        DrawText("Trykk til å slappe av...", 40, GetScreenHeight() - 30, 20,
                 DARKGRAY);
      }
      break;
    case RELAXED_ASKOY:
      DrawTexturePro(data->hytteTwo, bgSrc, bgDst, (Vector2){0, 0}, 0, WHITE);
      break;
    }
  }
  }
  }
}
