#include "mbed.h"

void run_led_control_loop();
void setup_wifi_and_poll();

volatile bool lock_state = true;  
volatile bool state_updated = false;

int main()
{
    printf("Starting receiver application...\r\n");
    fflush(stdout);
    
    Thread http_poll_thread;
    http_poll_thread.start(setup_wifi_and_poll);
    
    run_led_control_loop();
}
