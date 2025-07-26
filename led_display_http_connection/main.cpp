#include "mbed.h"


void run_led_control_loop();
void setup_wifi_and_wait();


volatile bool lock_state = true;
volatile bool state_changed = false;

InterruptIn button(BUTTON1); 

void toggle_lock() {
    lock_state = !lock_state;
    state_changed = true;
}


int main()
{
    button.fall(&toggle_lock);
    
    Thread http_post_thread;
    http_post_thread.start(setup_wifi_and_wait);
    
    run_led_control_loop();
}
