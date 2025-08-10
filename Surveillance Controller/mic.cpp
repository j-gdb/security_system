#include "mic.hpp"

InterruptIn mic(PA_15);

void signal(){
    // printf("Signal Detected");
    unlock();
}

void init_mic_response(){
    mic.rise(&signal);
}