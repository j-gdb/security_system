#ifndef CSC385_MOTOR_HPP
#define CSC385_MOTOR_HPP
#include "mbed.h"


/*
 * @brief Move the motor to lock or unlock the depending on the state.
 */
void set_motor(bool is_alert, PwmOut &servo);

#endif //CSC385_MOTOR_HPP