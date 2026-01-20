#ifndef OBJECT_H_
#define OBJECT_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

#define OBJECTHANDLER_SIZE 64

typedef enum {empty, bullet, player, enemy} objecttype;
typedef struct {
	objecttype type;
	int32_t position_x, position_y, a, b, c;
} object_t;

typedef struct World World;
void initObjecthandler(object_t* objecthandler);
void spawnObject(object_t* objecthandler, object_t object);
void deleteObject(object_t* objecthandler, uint8_t id);
void updateObject(object_t* objecthandler, World* world);



#endif /* OBJECT_H_ */
