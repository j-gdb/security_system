#include "mbed.h"
#include "motor_385.hpp"

PwmOut servo(PA_3);

void run_motor_control_loop();
void setup_wifi_and_poll();

volatile bool lock_state = true;  
volatile bool state_updated = false;

int main()
{
    set_motor(false, servo);
    Thread http_poll_thread;
    http_poll_thread.start(setup_wifi_and_poll);
    
    run_motor_control_loop();
}
