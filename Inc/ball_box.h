#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course

struct ball { 
    uint8_t position[2]; 
    int8_t velocity[2]; }; 
    void ball_window(uint8_t P1[2], uint8_t P2[2], uint8_t style); 
    void ball_box(void); 
    void ball_pos(struct ball *b, uint8_t PO1[2], uint8_t PO2[2], uint8_t PI1[2], uint8_t PI2[2]);