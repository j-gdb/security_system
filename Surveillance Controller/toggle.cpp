#include "toggle.hpp"

volatile bool lock_state = false;
volatile bool state_changed = false;


void unlock(){
    lock_state = false;
    state_changed = true;
}
void toggle_lock() {
    lock_state = true;
    state_changed = true;
}