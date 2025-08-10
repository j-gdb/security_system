#include "motor_385.hpp"


void set_motor(bool is_alert, PwmOut &servo){
    servo.period_ms(20); //some setup
    if (is_alert){
        servo.pulsewidth_us(2500);
    }
    else{
        servo.pulsewidth_us(500);
    }
}