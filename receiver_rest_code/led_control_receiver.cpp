#include "mbed.h"

extern volatile bool lock_state;


// led pins (both green)
DigitalOut led1(LED1);
DigitalOut led2(LED2);  

void run_led_control_loop()
{
    // Blink both when locked, one LED unlocked
    while (true) {
        led1 = !led1;

        if (lock_state) {
            led2 = !led2;
        } else {
            led2 = 0;
        }

        thread_sleep_for(500);
    }
}
