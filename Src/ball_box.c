#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "ball_box.h"
#define ESC 0x1B
#define hide_cursor() printf("\x1B[?25l")


void ball_window(uint8_t P1[2], uint8_t P2[2], uint8_t style)
{
    printf("%c[36m", ESC);

	uint8_t hor_char;
	uint8_t ver_char;
	uint8_t tl_char;
	uint8_t tr_char;
	uint8_t bl_char;
	uint8_t br_char;

    if (style) {
    	hor_char = 205;
    	ver_char = 186;
    	tl_char = 201;
    	tr_char = 187;
    	bl_char = 200;
    	br_char = 188;
    }
    else {
    	hor_char = 196;
		ver_char = 179;
		tl_char = 218;
		tr_char = 191;
		bl_char = 192;
		br_char = 217;
    }


    int width  = P2[0] - P1[0];
    int height = P2[1] - P1[1];

    gotoxy(P1[0], P1[1]);

    printf("%c", tl_char);

    for (int i = 0; i < width; i++)
        printf("%c", hor_char);

    printf("%c", tr_char);

    printf("\n");

    for (int row = 1; row < height - 1; row++) {
    	printf("%c[%dC", ESC, P1[0] - 1); // Go forward x-coordinate spaces
        printf("%c", ver_char);
        for (int col = 0; col < width; col++)
            printf(" ");
        printf("%c\n", ver_char);
    }
    printf("%c[%dC", ESC, P1[0] - 1); // Go forward x-coordinate spaces
    printf("%c", bl_char);
    for (int i = 0; i < width; i++) {
        printf("%c", hor_char);
    }
    printf("%c", br_char);
}

void ball_pos(struct ball *b,
              uint8_t PO1[2], uint8_t PO2[2],
              uint8_t PI1[2], uint8_t PI2[2])
{
    static uint32_t collisions = 0;

    gotoxy(b->position[0], b->position[1]);
    printf(" ");

    b->position[0] += b->velocity[0];
    b->position[1] += b->velocity[1];

    if (b->position[0] <= PO1[0] + 1 || b->position[0] >= PO2[0] - 1) // Vertical outer
        {
        b->velocity[0] = -b->velocity[0];
        collisions++;
    }

    if (b->position[1] <= PO1[1] + 1 || b->position[1] >= PO2[1] - 2) // Horizontal outer
        {
        b->velocity[1] = -b->velocity[1];
        collisions++;
        }


    if (b->position[0] == PI1[0] - 1 && PI1[1] <= b->position[1] && b->position[1] <= PI2[1] && b->velocity[0] > 0) { // Left inner
        b->velocity[0] = -b->velocity[0];
        collisions++;
    }

    if (b->position[0] == PI2[0] && PI1[1] <= b->position[1] && b->position[1] <= PI2[1] && b->velocity[0] < 0) { // Right inner
        b->velocity[0] = -b->velocity[0];
        collisions++;
    }

    if (b->position[1] == PI1[1] - 1 && PI1[0] <= b->position[0] && b->position[0] <= PI2[0] && b->velocity[1] > 0) { // Top inner
        b->velocity[1] = -b->velocity[1];
        collisions++;
    }

    if (b->position[1] == PI2[1] && PI1[0] <= b->position[0] && b->position[0] <= PI2[0] && b->velocity[1] < 0) { // Bottom inner
        b->velocity[1] = -b->velocity[1];
        collisions++;
    }


    gotoxy(b->position[0], b->position[1]);
    printf("o");

    gotoxy(74, 30);
    printf("Hits: %lu ", collisions);
}


void ball_box(void)
{
    clrscr();
    hide_cursor();

    uint8_t PO1[2] = {50, 15};
    uint8_t PO2[2] = {110, 45};

    uint8_t PI1[2] = {72, 28};
    uint8_t PI2[2] = {88, 32};

    ball_window(PO1, PO2, 1);
    ball_window(PI1, PI2, 0);

    struct ball ball1 = {
        .position = {60, 40},
        .velocity = {1, 1}
    };

    while (1) {
        ball_pos(&ball1, PO1, PO2, PI1, PI2);
    }
}
