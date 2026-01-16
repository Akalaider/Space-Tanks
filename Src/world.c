#include "30010_io.h" // Input/output library for this course
#include <stdlib.h>
#include "world.h"
#include "ansi.h"
#include "tank.h"

Rect getTankRect(Point pos)
{
    Rect r;
    r.left   = pos.x - TANK_WIDTH  / 2;
    r.right  = pos.x + TANK_WIDTH  / 2;
    r.top    = pos.y - TANK_HEIGHT / 2;
    r.bottom = pos.y + TANK_HEIGHT / 2;
    return r;
}


Rect getObstacleRect(Point pos, Point size)
{
    Rect r;
    r.left   = pos.x;
    r.right  = pos.x + size.x - 1;
    r.top    = pos.y;
    r.bottom = pos.y + size.y - 1;
    return r;
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

Point getSpriteSize(const char *sprite)
{
    Point size = {0, 1}; // width, height
    int16_t currentWidth = 0;

    for (const char *p = sprite; *p; p++) {
        if (*p == '\n') {
            size.y++;
            if (currentWidth > size.x)
                size.x = currentWidth;
            currentWidth = 0;
        } else {
            currentWidth++;
        }
    }

    if (currentWidth > size.x)
        size.x = currentWidth;

    return size;
}


void drawObstacle(Point position, const char* sprite, World* world)
{
    printCp850At(position.x, position.y, sprite);
    Point size = getSpriteSize(sprite);

    // corners
    Point topLeft     = position;
    Point topRight    = (Point){ position.x + size.x - 1, position.y };
    Point bottomLeft  = (Point){ position.x, position.y + size.y - 1 };
    Point bottomRight = (Point){ position.x + size.x - 1, position.y + size.y - 1 };

    // Add 4 wall segments
    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (WallSegment){ topLeft, topRight };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (WallSegment){ topRight, bottomRight };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (WallSegment){ bottomRight, bottomLeft };

    if (world->count < MAX_WALL_SEGMENTS)
        world->segments[world->count++] = (WallSegment){ bottomLeft, topLeft };
}

uint8_t rectOverlap(Rect a, Rect b)
{
    return !(a.right < b.left ||
             a.left > b.right ||
             a.bottom < b.top ||
             a.top > b.bottom);
}

CollisionSide getCollisionSide(Rect tank, Rect wall)
{
    int overlapLeft   = tank.right  - wall.left;
    int overlapRight  = wall.right  - tank.left;
    int overlapTop    = tank.bottom - wall.top;
    int overlapBottom = wall.bottom - tank.top;

    int min = abs(overlapLeft);
    CollisionSide side = COLLISION_RIGHT;

    if (abs(overlapRight) < min) {
        min = abs(overlapRight);
        side = COLLISION_LEFT;
    }
    if (abs(overlapTop) < min) {
        min = abs(overlapTop);
        side = COLLISION_BOTTOM;
    }
    if (abs(overlapBottom) < min) {
        min = abs(overlapBottom);
        side = COLLISION_TOP;
    }

    return side;
}


CollisionSide rectCollision(Rect tank, Rect wall)
{
    if (!rectOverlap(tank, wall))
        return COLLISION_NONE;

    return getCollisionSide(tank, wall);
}


CollisionSide checkWallCollisionAABB(Point tankPos, World *world)
{
    Rect tank = getTankRect(tankPos);

    for (uint16_t i = 0; i < world->count; i++) {
        WallSegment seg = world->segments[i];

        Rect wall = {
            .left   = seg.p1.x,
            .right  = seg.p2.x,
            .top    = seg.p1.y,
            .bottom = seg.p2.y
        };

        // Normalize (in case p1 > p2)
        if (wall.left > wall.right) {
            int16_t tmp = wall.left; wall.left = wall.right; wall.right = tmp;
        }
        if (wall.top > wall.bottom) {
            int16_t tmp = wall.top; wall.top = wall.bottom; wall.bottom = tmp;
        }

        CollisionSide side = rectCollision(tank, wall);
        if (side != COLLISION_NONE)
            return side;
    }

    return COLLISION_NONE;
}
