#define ARENA_IMPLEMENTATION
#include "arena.h"

#include "letter.h"
#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

Camera2D camera = {0};
Vector3 cubePosition = {0};
Letter *letter = NULL;
Arena *arena = NULL;

bool gameBegan = false;

static void UpdateDrawFrame(void); // Update and one frame

int main() {
  // 1kB
  arena = arena_create(1024 * 1024);

  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  int configFlags = FLAG_WINDOW_RESIZABLE;
  SetConfigFlags(configFlags);
  InitWindow(screenWidth, screenHeight, "Gratulerer med Dagen!");

  InitAudioDevice();

  letter = CreateLetter(arena);

  camera.target = (Vector2){0.0f, 0.0f};

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }
#endif

  UnloadSound(letter->sounds.intro);
  UnloadSound(letter->sounds.tear);
  UnloadSound(letter->sounds.boatBop);
  UnloadSound(letter->sounds.ahhh);
  CloseAudioDevice();

  // De-Initialization
  arena_destroy(arena);
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
  if (!gameBegan) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (GetScreenHeight() > GetScreenWidth()) {
      DrawText("Rotate phone to\nlandscape to begin...", 40,
               GetScreenHeight() / 2, 30, DARKBLUE);

      EndDrawing();
      return;
    }

    DrawText("Turn on the audio!", 50, GetScreenHeight() / 2 - 80, 30,
             DARKBLUE);

    DrawText("Click/Press screen to begin...", 50, GetScreenHeight() / 2, 30,
             DARKBLUE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      gameBegan = 1;
    }
    EndDrawing();
    return;
  }

  UpdateLetter(letter, GetFrameTime());

  //----------------------------------------------------------------------------------
  BeginDrawing();

  DrawLetter(letter);

  ClearBackground(RAYWHITE);

  DrawLetterUi(letter);

  EndDrawing();
  //----------------------------------------------------------------------------------
}
