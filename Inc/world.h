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

typedef enum {
    COLLISION_NONE,
    COLLISION_LEFT,    // hit wall on its left side
    COLLISION_RIGHT,   // hit wall on its right side
    COLLISION_TOP,     // hit wall from above
    COLLISION_BOTTOM   // hit wall from below
} CollisionSide;




void drawWalls(Point* positions, uint8_t numPoints, uint8_t closePath, World* world);
void drawLine(Point P1, Point P2);
Point getSpriteSize(const char *sprite);
void drawObstacle(Point position, const char* sprite, World* world);
CollisionSide segmentCollision(Point pos, uint8_t radius, WallSegment seg);
CollisionSide checkWallCollision(Point pos, uint8_t radius, World* world);

#endif