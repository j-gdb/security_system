#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "mbed.h"

extern DigitalIn blue_button;
extern DigitalIn red_button;
extern DigitalIn green_button;
extern DigitalIn black_button;

extern int passcode_digit;
extern int passcode[4];

int isEqual(int x[], int y[], int size);
void button_trigger(DigitalIn button, int* arr_index, int* input_arr);
void init_buttons(EventQueue &queue);
void handle_button();

#endif
