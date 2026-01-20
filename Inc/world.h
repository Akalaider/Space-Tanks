#ifndef WORLD_H
#define WORLD_H
#include "30010_io.h" // Input/output library for this course
#include "object.h"
#define MAX_WALL_SEGMENTS 256


typedef struct 
{
    int16_t x; 
    int16_t y; 
} Point;

typedef struct {
    Point p1;
    Point p2;
} WallSegment;

struct World{
    WallSegment segments[MAX_WALL_SEGMENTS];
    uint16_t count;
};

typedef struct {
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
} Hitbox;


typedef enum {
    COLLISION_NONE,
    COLLISION_LEFT,    // hit wall on its left side
    COLLISION_RIGHT,   // hit wall on its right side
    COLLISION_TOP,     // hit wall from above
    COLLISION_BOTTOM   // hit wall from below
} CollisionSide;


Hitbox getTankHitbox(object_t tank);
Hitbox getObstacleHitbox(Point pos, Point size);
uint8_t HitboxOverlap(Hitbox a, Hitbox b);
void drawWalls(Point* positions, uint8_t numPoints, uint8_t closePath, World* world);
void drawLine(Point P1, Point P2);
void drawObstacle(Point position, const char* sprite, uint8_t width, uint8_t height, World* world);
CollisionSide getCollisionSide(Hitbox tank, Hitbox wall);
CollisionSide HitboxCollision(Hitbox tank, Hitbox wall);
CollisionSide checkWallCollisionAABB(object_t tank, World *world);

#endif
