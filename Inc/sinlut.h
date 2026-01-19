#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

typedef struct {
	int32_t x, y;
} vector_t;

int32_t sinus(int32_t deg);
int32_t cosinus(int32_t deg);
int32_t rounddiff(int32_t t, int32_t n);

int32_t expand(int32_t i);
void printFix(int32_t i);
void rotateVector(vector_t *v, int deg);
