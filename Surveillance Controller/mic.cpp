#include "mic.hpp"

InterruptIn mic(PA_15);

void signal(){
    printf("Signal Detected");
    if (lock_state){
        toggle_lock();
    }
}

void init_mic_response(){
    mic.rise(&signal);
}