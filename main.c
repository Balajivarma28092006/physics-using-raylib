#include "raylib.h"

int main() {
  InitWindow(800, 600, "My window");
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		ClearBackground(BLACK);
		EndDrawing();
}
CloseWindow();
  return 0;
}