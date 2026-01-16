#include "30010_io.h" // Input/output library for this course
#include "world.h"
#include "ansi.h"
#include "tank.h"


static inline int iabs(int x) {
    return x < 0 ? -x : x;
}

void drawLine(Point P1, Point P2)
{
    if (P1.x == P2.x) 
    {
        // Vert
        uint16_t x = P1.x;
        for (uint16_t y = (P1.y < P2.y ? P1.y : P2.y); y <= (P1.y > P2.y ? P1.y : P2.y); y++) 
        {
            gotoxy(x, y);
            printf("%c", 219);
        }
    }
    if (P1.y == P2.y) 
    {
        // Hor
        uint16_t y = P1.y;
        for (uint16_t x = (P1.x < P2.x ? P1.x : P2.x); x <= (P1.x > P2.x ? P1.x : P2.x); x++) 
        {
            gotoxy(x, y);
            printf("%c", 219);
        }
    }
}


void drawWalls(Point* positions, uint8_t numPoints, uint8_t closePath, World* world) { 
    for (uint8_t i = 0; i < numPoints - 1; i++) { 
        if (world->count < MAX_WALL_SEGMENTS) { 
            world->segments[world->count].p1 = positions[i]; 
            world->segments[world->count].p2 = positions[i + 1]; 
            world->count++; 
        } 
        drawLine(positions[i], positions[i + 1]); 
    } 
    if (closePath) { 
        if (world->count < MAX_WALL_SEGMENTS) { 
            world->segments[world->count].p1 = positions[numPoints - 1]; 
            world->segments[world->count].p2 = positions[0]; 
            world->count++; 
        } 
        drawLine(positions[numPoints - 1], positions[0]); 
    } 
}

void drawObstacle(Point position, const char* sprite, uint8_t diameter, World* world)
{
    printCp850At(position.x, position.y, sprite);

    Point topLeft     = position;
    Point topRight    = (Point){ position.x + diameter, position.y };
    Point bottomLeft  = (Point){ position.x, position.y + diameter };
    Point bottomRight = (Point){ position.x + diameter, position.y + diameter };

    // Add the 4 edges as wall segments
    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (Segment){ topLeft, topRight };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (Segment){ topRight, bottomRight };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (Segment){ bottomRight, bottomLeft };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (Segment){ bottomLeft, topLeft };
}



CollisionSide segmentCollision(Point pos, uint8_t radius, WallSegment seg)
{
    uint16_t x1 = seg.p1.x;
    uint16_t y1 = seg.p1.y;
    uint16_t x2 = seg.p2.x;
    uint16_t y2 = seg.p2.y;

    // Vertical wall (x = constant)
    if (x1 == x2) {
        uint16_t wallX = x1;

        uint16_t minY = (y1 < y2 ? y1 : y2);
        uint16_t maxY = (y1 > y2 ? y1 : y2);

        // Check if ball overlaps vertically
        if (pos.y >= minY && pos.y <= maxY) {

            // Ball hits from left
            if (pos.x + radius >= wallX && pos.x < wallX)
                return COLLISION_RIGHT;

            // Ball hits from right
            if (pos.x - radius <= wallX && pos.x > wallX)
                return COLLISION_LEFT;
        }
    }

    // Horizontal wall (y = constant)
    if (y1 == y2) {
        uint16_t wallY = y1;

        uint16_t minX = (x1 < x2 ? x1 : x2);
        uint16_t maxX = (x1 > x2 ? x1 : x2);

        // Check if ball overlaps horizontally
        if (pos.x >= minX && pos.x <= maxX) {

            // Ball hits from above
            if (pos.y + radius >= wallY && pos.y < wallY)
                return COLLISION_BOTTOM;

            // Ball hits from below
            if (pos.y - radius <= wallY && pos.y > wallY)
                return COLLISION_TOP;
        }
    }

    return COLLISION_NONE;
}



CollisionSide checkWallCollision(Point pos, uint8_t radius, World* world)
{
    for (uint16_t i = 0; i < world->count; i++) {
        CollisionSide side = segmentCollision(pos, radius, world->segments[i]);
        if (side != COLLISION_NONE)
            return side;
    }
    return COLLISION_NONE;
}