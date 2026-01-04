#include "Ball.h"
#include "raymath.h"
#include <algorithm>

Ball::Ball(float x, float y, float r, Color c)
    : pos{x, y}, vel{0, 0}, radius{r}, color{c} {}

void Ball::Update(float dt, int screenWidth, int screenHeight)
{
    vel.y += 980 * dt; // gravity
    pos = Vector2Add(pos, Vector2Scale(vel, dt));
    if(Vector2Length(vel) > 1000.f){
        vel = Vector2Scale(Vector2Normalize(vel), 1000.f); // drag
    }
    // floor
    if (pos.y + radius > screenHeight)
    {
        pos.y = screenHeight - radius;
        vel.y *= -0.75f;
    }

    // walls
    if (pos.x - radius < 0)
    {
        pos.x = radius;
        vel.x *= -0.6f;
    }

    if (pos.x + radius > screenWidth)
    {
        pos.x = screenWidth - radius;
        vel.x *= -0.6f;
    }
}

void Ball::SetPos(Vector2 p) { pos = p; }
void Ball::SetVel(Vector2 v) { vel = v; }

void Ball :: Draw() const
{
    DrawCircleV(pos, radius, color);
}

Vector2 Ball::GetPos() const { return pos; }
Vector2 Ball::GetVel() const { return vel; }
float Ball::GetRadius() const { return radius; }

void ResolveCollision(Ball &b1, Ball &b2)
{
    Vector2 pos1 = b1.GetPos();
    Vector2 pos2 = b2.GetPos();

    Vector2 normal = Vector2Subtract(pos2, pos1);
    float dist = Vector2Length(normal);
    float minDist = b1.GetRadius() + b2.GetRadius();

    if (dist <= 0.0f || dist >= minDist)
        return;

    normal = Vector2Scale(normal, 1.0f / dist);

    Vector2 relVel = Vector2Subtract(b2.GetVel(), b1.GetVel());
    float velAlongNormal = Vector2DotProduct(relVel, normal);

    if (velAlongNormal > 0.0f)
        return;

    const float restitution = 0.8f;

    float j = -(1.0f + restitution) * velAlongNormal;
    j /= 2.0f; // equal mass

    Vector2 impulse = Vector2Scale(normal, j);

    b1.SetVel(Vector2Subtract(b1.GetVel(), impulse));
    b2.SetVel(Vector2Add(b2.GetVel(), impulse));

    // positional correction (stable)
    const float percent = 0.8f;
    const float slop = 0.01f;

    float penetration = minDist - dist;
    float correction = std::max(penetration - slop, 0.0f) * percent / 2.0f;

    b1.SetPos(Vector2Subtract(pos1, Vector2Scale(normal, correction)));
    b2.SetPos(Vector2Add(pos2, Vector2Scale(normal, correction)));
}
