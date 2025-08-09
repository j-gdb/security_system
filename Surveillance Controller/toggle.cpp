#include "toggle.hpp"

volatile bool lock_state = false;
volatile bool state_changed = false;

void toggle_lock() {
    lock_state = !lock_state;
    state_changed = true;
    if (lock_state){
        printf("locked\n");
    } else {
        printf("unlocked\n");
    }
}