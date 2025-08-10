#include "mbed.h"
#include "motor_385.hpp"

extern volatile bool lock_state;
extern PwmOut servo;
bool current_state = false;

void run_motor_control_loop()
{
    while (true) {
        if (current_state != lock_state){
            current_state = lock_state;
            set_motor(lock_state, servo);
        }
    }
}
