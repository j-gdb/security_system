#include "toggle.hpp"

volatile bool lock_state = true;
volatile bool state_changed = false;

void toggle_lock() {
    lock_state = !lock_state;
    state_changed = true;
}