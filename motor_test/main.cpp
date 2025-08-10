#include "mbed.h"
#include "motor_385.hpp"

PwmOut servo(PA_3); //servo to open and close door

int main() {
    set_motor(true, servo);
    while(true){
        ThisThread::sleep_for(1000ms);
        set_motor(false, servo);
        ThisThread::sleep_for(1000ms);
        set_motor(true, servo);
    }
}