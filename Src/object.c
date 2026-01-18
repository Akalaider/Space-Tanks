/*
 * object.c
 *
 *  Created on: 18. jan. 2026
 *      Author: adambenali
 */


#include "object.h"

void initObjecthandler(object_t* objecthandler){
	for(uint8_t i = 0; i < 256;i++){
		objecthandler[i] -> type = empty;
	}
}


void spawnObject(object_t* objecthandler, object_t object){
	for(uint8_t i = 0; i < 256;i++){
		if(objecthandler[i] -> type == empty){
			objecthandler[i] -> type = object.type;
			objecthandler[i] -> position_x = object.position_x;
			objecthandler[i] -> position_x = object.position_y;
			objecthandler[i] -> a = object.a;
			objecthandler[i] -> b = object.b;
			objecthandler[i] -> c = object.c;
			return;
		}
	}
}

void deleteObject(object_t* objecthandler, uint8_t id){
	objecthandler[id] -> type = empty;
}

void updateObject(object_t* objecthandler){
	for(uint8_t i = 0; i < 256;i++){
		switch (objecthandler[i] -> type){
			case empty:
				break;
			case bullet:
				//add bullet1 update
				break;
			case player:
				//add playerKeyboard update
				break;
			case enemy:
				//add enemy update
				break;
		}
	}
}



