#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "joystick.h"


void setupRight(void)
{
    GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 – Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 - No pull, 0x01 - Pull-up, 0x02 - Pull-down)
}

void setupLeft(void)
{
    GPIOC->MODER &= ~(0x00000003 << (1 * 2));
    GPIOC->MODER |= (0x00000000 << (1 * 2));
    GPIOC->PUPDR &= ~(0x00000003 << (1 * 2));
    GPIOC->PUPDR |= (0x00000002 << (1 * 2));
}

void setupUp(void)
{
    GPIOA->MODER &= ~(0x00000003 << (4 * 2));
    GPIOA->MODER |= (0x00000000 << (4 * 2));
    GPIOA->PUPDR &= ~(0x00000003 << (4 * 2));
    GPIOA->PUPDR |= (0x00000002 << (4 * 2));
}

void setupDown(void)
{
    GPIOB->MODER &= ~(0x00000003 << (0 * 2));
    GPIOB->MODER |= (0x00000000 << (0 * 2));
    GPIOB->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOB->PUPDR |= (0x00000002 << (0 * 2));
}

void setupCenter(void)
{
    GPIOB->MODER &= ~(0x00000003 << (5 * 2));
    GPIOB->MODER |= (0x00000000 << (5 * 2));
    GPIOB->PUPDR &= ~(0x00000003 << (5 * 2));
    GPIOB->PUPDR |= (0x00000002 << (5 * 2));
}

int8_t readJoy(void)
{
    return 0x1F & (((GPIOA->IDR & (0x0001 << 4)) >> (-0+4))
            | ((GPIOB->IDR & (0x0001 << 0)) << (1-0))
            | ((GPIOC->IDR & (0x0001 << 1)) << (2-1))
            | ((GPIOC->IDR & (0x0001 << 0)) << (3-0))
            | ((GPIOB->IDR & (0x0001 << 5)) >> (-4+5)));
}

void LED_init(void)
{
    // PA9 (Blue) 
    GPIOA->MODER &= ~(3 << (9 * 2)); 
    GPIOA->MODER |= (1 << (9 * 2)); 
    // 
    // PC7 (Green) 
    GPIOC->MODER &= ~(3 << (7 * 2)); 
    GPIOC->MODER |= (1 << (7 * 2));
    // PB4 (Red) 
    GPIOB->MODER &= ~(3 << (4 * 2)); 
    GPIOB->MODER |= (1 << (4 * 2));
}

void setLED(uint8_t color)
{
    // Turn all LEDs OFF (set pins HIGH)
    GPIOA->ODR |= (1 << 9); // Blue OFF
    GPIOB->ODR |= (1 << 4); // Red OFF
    GPIOC->ODR |= (1 << 7); // Green OFF

    switch (color)
    {
        case blue:
            GPIOA->ODR &= ~(1 << 9); // Blue ON
            break;
        case green:
            GPIOC->ODR &= ~(1 << 7); // Green ON
            break;
        case red:
            GPIOB->ODR &= ~(1 << 4); // Red ON
            break;
        case cyan:
            GPIOA->ODR &= ~(1 << 9);
            GPIOC->ODR &= ~(1 << 7);
            break;
        case pink:
            GPIOA->ODR &= ~(1 << 9);
            GPIOB->ODR &= ~(1 << 4);
            break;
        case yellow:
            GPIOB->ODR &= ~(1 << 4);
            GPIOC->ODR &= ~(1 << 7);
            break;
        case white:
            GPIOA->ODR &= ~(1 << 9);
            GPIOB->ODR &= ~(1 << 4);
            GPIOC->ODR &= ~(1 << 7);
            break;
        case black:
        default:
            break;
    }
}


void controlLED(uint8_t direction)
{
    if (direction == up)
        setLED(blue);
    else if (direction == down)
        setLED(red);
    else if (direction == left)
        setLED(green);
    else if (direction == right)
        setLED(cyan);
    else if (direction == center)
        setLED(white);
    else
        setLED(black);
}


void joystick_init(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C
}

void debug_joy_raw(void)
{
    uint16_t ida = GPIOA->IDR;
    uint16_t idb = GPIOB->IDR;
    uint16_t idc = GPIOC->IDR;

    printf("A: %04X  B: %04X  C: %04X\n", ida, idb, idc);
}
