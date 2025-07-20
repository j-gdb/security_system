#include "mbed.h"


void run_led_control_loop();
void setup_mqtt_and_wait();


volatile bool lock_state = true;

int main()
{
    Thread mqtt_thread;
    mqtt_thread.start(setup_mqtt_and_wait);
    
    run_led_control_loop();
}
