
#include "mbed.h"

// lock_state (modified via mqtt) 
extern volatile bool lock_state;

// LED pins (theyre both green)
DigitalOut led1(LED1);
DigitalOut led2(LED2);  

void run_led_control_loop()
{
    // blink both on locked and one on unlocked
    while (true) {
        led1 = !led1;

        if (lock_state) {

            led2 = !led2;
        } else {

            led2 = 0;
        }

        ThisThread::sleep_for(500ms);
    }
}


