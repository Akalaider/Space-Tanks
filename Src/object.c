#include "object.h"
#include "world.h"

void initObjecthandler(object_t* objecthandler){
	for(uint8_t i = 0; i < OBJECTHANDLER_SIZE; i++){
		objecthandler[i].type = empty;
	}
}

void spawnObject(object_t* objecthandler, object_t object){
	for(uint8_t i = 0; i < OBJECTHANDLER_SIZE; i++){
		if(objecthandler[i].type == empty){
			objecthandler[i].type = object.type;
			objecthandler[i].position_x = object.position_x;
			objecthandler[i].position_y = object.position_y;
			objecthandler[i].a = object.a;
			objecthandler[i].b = object.b;
			objecthandler[i].c = object.c;
			return;
		}
	}
}

void deleteObject(object_t* objecthandler, uint8_t id){
	objecthandler[id].type = empty;
}

void updateObject(object_t* objecthandler, World* world, char *buf){ 
	for(uint8_t i = 0; i < OBJECTHANDLER_SIZE; i++){ 
		switch (objecthandler[i].type){ 
			case empty: break; 
			case bullet: 
				updateBullet(&objecthandler[i], objecthandler, world); 
				break; 
			case player: 
				controlTank(objecthandler, world, &objecthandler[i], buf); 
				push_info_lcd(&objecthandler[i]); 
				buf[0] = '\0';
				break; 
			case enemy: 
				controlAITank(&objecthandler[i], objecthandler, world); 
				break; 
			case powerup: 
				updatePowerup(&objecthandler[i], objecthandler, world); 
				break; 
		} 
	} 
}