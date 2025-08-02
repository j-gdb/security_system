#include "mbed.h"
#include "buttons.hpp"
#include "tof.hpp"

int main() {
    EventQueue queue;
    Thread event_thread;
    
    event_thread.start(callback(&queue, &EventQueue::dispatch_forever));
    
    init_sensor(queue);
    init_buttons(queue);

    while (true) {
        ThisThread::sleep_for(1s);
        printf("Running\n");
        fflush(stdout);
    }

}