#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "utils/Ball.h"

int main()
{
    const int NumBalls = 500;

    // Window flags MUST be before InitWindow
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "Bouncing Balls");

    // Window size constraints MUST be after InitWindow
    SetWindowMinSize(400, 300);

    SetTargetFPS(60);
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<Ball> balls;
    balls.reserve(NumBalls);

    for (int i = 0; i < NumBalls; i++)
    {
        float r = 10 + rand() % 15;
        Color c = {
            (unsigned char)GetRandomValue(100, 255),
            (unsigned char)GetRandomValue(100, 255),
            (unsigned char)GetRandomValue(100, 255),
            255
        };

        balls.emplace_back(
            rand() % (GetScreenWidth() - 20) + 10,
            rand() % 100,
            r,
            c
        );

        balls.back().SetVel({
            (rand() % 400 - 200) * 1.0f,
            (rand() % 200 - 100) * 1.0f
        });
    }

    while (!WindowShouldClose())
    {
        int screenWidth  = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        float dt = GetFrameTime();

        for (auto& b : balls)
            b.Update(dt, screenWidth, screenHeight);

        for (int iter = 0; iter < 4; ++iter)
        {
            for (size_t i = 0; i < balls.size(); ++i)
            {
                for (size_t j = i + 1; j < balls.size(); ++j)
                {
                    ResolveCollision(balls[i], balls[j]);
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto& b : balls)
            b.Draw();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
