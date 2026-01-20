#include "30010_io.h" // Input/output library for this course
#define MAX_WALL_SEGMENTS 256
#ifndef WORLD_H 
#define WORLD_H

typedef struct 
{
    int16_t x; 
    int16_t y; 
} Point;

typedef struct {
    Point p1;
    Point p2;
} WallSegment;

typedef struct {
    WallSegment segments[MAX_WALL_SEGMENTS];
    uint16_t count;
} World;

typedef struct {
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
} Rect;


typedef enum {
    COLLISION_NONE,
    COLLISION_LEFT,    // hit wall on its left side
    COLLISION_RIGHT,   // hit wall on its right side
    COLLISION_TOP,     // hit wall from above
    COLLISION_BOTTOM   // hit wall from below
} CollisionSide;


Rect getTankRect(Point pos);
Rect getObstacleRect(Point pos, Point size);
uint8_t rectOverlap(Rect a, Rect b);
void drawWalls(Point* positions, uint8_t numPoints, uint8_t closePath, World* world);
void drawLine(Point P1, Point P2);
void drawObstacle(Point position, const char* sprite, uint8_t width, uint8_t height, World* world);
CollisionSide getCollisionSide(Rect tank, Rect wall);
CollisionSide rectCollision(Rect tank, Rect wall);
CollisionSide checkWallCollisionAABB(Point tankPos, World *world);

#endif