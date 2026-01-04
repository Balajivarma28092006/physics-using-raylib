#ifndef BALL_H
#define BALL_H

#include "raylib.h"

class Ball {
    private:
    Vector2 pos;
    Vector2 vel;
    float radius;
    Color color;

    public:
    Ball(float x, float y, float radius, Color color);

    void Update(float dt, int screenWidth, int screenHeight);
    void Draw() const;

    // getters (needed for collision)
    Vector2 GetPos() const;
    Vector2 GetVel() const;
    float GetRadius() const;

    // setters (used during collision resoultion)
    void SetPos(Vector2 p);
    void SetVel(Vector2 v);
};

// free function for collision resolution
void ResolveCollision(Ball &b1, Ball &b2);

#endif

