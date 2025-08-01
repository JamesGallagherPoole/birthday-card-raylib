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

static void UpdateDrawFrame(void); // Update and one frame

int main() {
  // 1kB
  arena = arena_create(1024);

  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "happy birthday again!");

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

  // De-Initialization
  arena_destroy(arena);
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void) {
  UpdateLetter(letter);

  //----------------------------------------------------------------------------------
  BeginDrawing();

  DrawLetter(letter);

  ClearBackground(RAYWHITE);

  BeginMode2D(camera);

  EndMode2D();

  DrawText("Happy Birthday", 10, 40, 20, DARKGRAY);

  DrawFPS(10, 10);

  EndDrawing();
  //----------------------------------------------------------------------------------
}
