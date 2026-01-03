#include "raylib.h"
#include <vector>
#include <cstdlib>
#include "raymath.h"

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    Color color;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int NumBalls = 200;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Bouncing Balls");
    SetTargetFPS(60);

    std::vector<Ball> balls;
    for (int i = 0; i < NumBalls; i++) {
        Ball b;
        b.color = RED;
        b.pos = { float(rand() % (screenWidth - 20) + 10), float(rand() % 100) };
        b.vel = { 0, 0 };
        b.radius = 10 + rand() % 15;
        balls.push_back(b);
    }

    while (!WindowShouldClose()) {

        // Update ball positions
        for (auto &b : balls) {
            b.vel.y += 980 * GetFrameTime(); // gravity
            b.pos = Vector2Add(b.pos, Vector2Scale(b.vel, GetFrameTime()));

            // Floor collision
            if (b.pos.y + b.radius > screenHeight) {
                b.pos.y = screenHeight - b.radius;
                b.vel.y *= -0.75f;
            }

            // Wall collisions
            if (b.pos.x - b.radius < 0) {
                b.pos.x = b.radius;
                b.vel.x *= -0.7f;
            }
            if (b.pos.x + b.radius > screenWidth) {
                b.pos.x = screenWidth - b.radius;
                b.vel.x *= -0.7f;
            }
        }

        // Ball-to-ball collisions
        for (int i = 0; i < balls.size(); i++) {
            for (int j = i + 1; j < balls.size(); j++) {
                Ball &b1 = balls[i];
                Ball &b2 = balls[j];

                Vector2 normal = Vector2Subtract(b2.pos, b1.pos);
                float dist = Vector2Length(normal);
                if (dist < b1.radius + b2.radius) {
                    normal = Vector2Scale(normal, 1.0f / dist); // normalize
                    Vector2 relVel = Vector2Subtract(b2.vel, b1.vel);

                    // correct projection onto normal
                    float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;

                    if (velAlongNormal > 0) continue; // already separating

                    // simple elastic collision (equal mass)
                    float impulse = -2 * velAlongNormal / 2;
                    Vector2 impulseVec = Vector2Scale(normal, impulse);

                    b1.vel = Vector2Subtract(b1.vel, impulseVec);
                    b2.vel = Vector2Add(b2.vel, impulseVec);

                    // separate overlapping balls
                    float overlap = 0.5f * (b1.radius + b2.radius - dist);
                    b1.pos = Vector2Subtract(b1.pos, Vector2Scale(normal, overlap));
                    b2.pos = Vector2Add(b2.pos, Vector2Scale(normal, overlap));
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto &b : balls) {
            DrawCircleV(b.pos, b.radius, b.color);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
