#include "mic.hpp"

InterruptIn mic(PA_15);
extern volatile bool lock_state;

void signal(){
    if(!lock_state){
        toggle_lock();
        }
    
}

void init_mic_response(){
    mic.rise(&signal);
}