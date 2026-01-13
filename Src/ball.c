/*
 * ball.c
 *
 *  Created on: 7. jan. 2026
 *      Author: adambenali
 */

#include "ansi.h"
#include "ball.h"

void makeScore(){
	uint8_t C1[2] = {50,14};
			uint8_t C2[2] = {60,16};
			window(C1, C2, "");
}

void scoreUpdate(uint8_t i){
	makeScore();
	gotoxy(51,15);
	printf("score: %d", i);
}

int8_t startBallGame(){
	startGame();
	makeScore();
	scoreUpdate(0);
	return 0;
}

void initBall(ball_t* b, int32_t xSet, int32_t ySet, int32_t xvSet, int32_t yvSet){
	b->x = xSet;
	b->y = ySet;
	b->xv = xvSet;
	b->yv = yvSet;
}

int8_t ballUpdate(ball_t* b){
	int32_t xOld;
	int32_t yOld;
	int32_t xvOld;
	int32_t yvOld;
	int32_t speed;
	int8_t hitwall;
	hitwall = 0;
	xOld = b->x;
	yOld = b->y;
	xvOld = b->xv;
	yvOld = b->yv;
	speed = 1;

	// deletes old ball
	gotoxy(xOld, yOld);
	printf("%c", 32);

	// chceck for if ball hit wall
	if ((2 - xvOld*speed) > b->x || b->x > (109 - xvOld*speed)) {
		b->xv = -xvOld;
		xvOld = b->xv;
		hitwall = 1;
	}
	if ((2 - yvOld*speed) > b->y || b->y > (29 - yvOld*speed)) {
		b->yv = -yvOld;
		yvOld = b->yv;
		hitwall = 1;
	}

	// calculate new position
	b->x = xOld + xvOld*speed;
	b->y = yOld + yvOld*speed;

	// render new ball
	gotoxy(b->x,b->y);
	printf("o");

	return hitwall;
}
