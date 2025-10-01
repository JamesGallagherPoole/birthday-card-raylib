#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
typedef enum { INVISIBLE, ENTER, VISIBLE, EXIT, DONE } ShowState;

Vector2 ScalePointBasedOnRef(int startWidth, Vector2 pointToScale);
Vector2 GetScaledUpDimensions(int original_width, int original_height);
int GetWindowPadding(void);
Rectangle FitInto(Rectangle bounds, float srcW, float srcH);

#endif
