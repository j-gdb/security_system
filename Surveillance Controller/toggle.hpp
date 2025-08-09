#ifndef TOGGLE_HPP
#define TOGGLE_HPP

#include "mbed.h"

extern volatile bool lock_state;
extern volatile bool state_changed;

void toggle_lock();

#endif 
