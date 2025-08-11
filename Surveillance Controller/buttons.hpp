#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "mbed.h"
#include "toggle.hpp"

extern InterruptIn blue_button;
extern InterruptIn red_button;
extern InterruptIn green_button;
extern InterruptIn black_button;

extern int passcode_digit;
extern int passcode[4];

bool isEqual(int x[], int y[], int size);
void button_trigger(DigitalIn button, int* arr_index, int* input_arr);
void init_buttons(EventQueue &queue);
void handle_button(int value);
void reset_password();

#endif
