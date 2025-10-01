
#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "utils.h"

void UpdateBoatCard(Letter *letter, Card *card, float dt) {
  AskoyBoatData *data = &card->cardData.askoyBoatData;

  switch (data->state) {
  case BOAT: {
    int maxX = card->contentRec.width - (card->contentRec.width / 2);

    if (data->boatPosX >= maxX &&
        (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
      data->state = ARRIVED_ASKOY;
    }

    if ((IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) &&
        data->boatPosX < maxX) {
      if (!IsSoundPlaying(letter->sounds.boatBop)) {
        SetSoundVolume(letter->sounds.intro, 0.1);
        PlaySound(letter->sounds.boatBop);
      }
      data->boatVelX += data->accel * dt;
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
      data->boatVelX -= sign * data->friction * dt;
    }

    data->boatVelX = Clamp(data->boatVelX, 0, data->boatTopSpeed);
    data->boatPosX += data->boatVelX * dt;
    break;
  }
  case ARRIVED_ASKOY:
    if (!IsSoundPlaying(letter->sounds.boatBop)) {
      StopSound(letter->sounds.boatBop);
    }
    data->timer += dt;
    if (data->timer > 3.0f) {
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        data->state = RELAXED_ASKOY;
        data->timer = 0.0f;
      }
    }
    break;
  case RELAXED_ASKOY:
    if (!IsSoundPlaying(letter->sounds.ahhh)) {
      PlaySound(letter->sounds.ahhh);
    }
    data->timer += dt;
    if (data->timer > 3.0f) {
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        card->isFinished = true;
        data->timer = 0.0f;
      }
    }
    break;
  }
}

void DrawBoatCard(Letter *letter, Card *card) {
  AskoyBoatData *data = &card->cardData.askoyBoatData;

  Texture2D tex = data->oceanBackground;
  Rectangle bgDst =
      FitInto(card->contentRec, (float)tex.width, (float)tex.height);
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
      int maxX = card->contentRec.width - (card->contentRec.width / 2);
      if (data->boatPosX >= maxX) {
        DrawText("Trykk til å gå til hytten...", 40, GetScreenHeight() - 30, 20,
                 DARKGRAY);
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
