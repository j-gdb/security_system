#include "mbed.h"
#include "buttons.hpp"
#include "tof.hpp"
#include "toggle.hpp"
#include "mic.hpp"

void run_led_control_loop();
void setup_wifi_and_wait();

InterruptIn button(BUTTON1); 

int main()
{
    EventQueue queue;
    Thread event_thread;
    Thread http_post_thread;
    
    event_thread.start(callback(&queue, &EventQueue::dispatch_forever));
    
    // initialize the sensor and external buttons
    init_sensor(queue);
    init_buttons(queue);
    init_mic_response();
    
    button.fall(&toggle_lock);
    
    http_post_thread.start(setup_wifi_and_wait);
    
    run_led_control_loop();
}
