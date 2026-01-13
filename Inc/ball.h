/*
 * ball.h
 *
 *  Created on: 7. jan. 2026
 *      Author: adambenali
 */

#ifndef BALL_H_
#define BALL_H_

typedef struct {
int32_t x, y, xv, yv;
} ball_t;

void makeScore();
void scoreUpdate(uint8_t i);
int8_t startBallGame();
void initBall(ball_t* b, int32_t xSet, int32_t ySet, int32_t xvSet, int32_t yvSet);
int8_t ballUpdate(ball_t* b);

#endif /* BALL_H_ */
