/*
 * sinus.h
 *
 *  Created on: 6. jan. 2026
 *      Author: adambenali
 */

#ifndef SINUS_H_
#define SINUS_H_
#include <stdint.h>
#include <stdio.h>

typedef struct {
int32_t x, y;
} vector_t;

void printFix(int32_t i);
int32_t expand(int32_t i);
int32_t sinus(int32_t i);
int32_t rdiv(int32_t t, int32_t n);
void initVector(vector_t* v, int32_t xSet, int32_t ySet);
void rotateVector(vector_t* v, int32_t deg);
#endif /* SINUS_H_ */

