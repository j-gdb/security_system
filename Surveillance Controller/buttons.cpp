#include "buttons.hpp"

InterruptIn blue_button(PA_4);
InterruptIn red_button(PA_3);
InterruptIn green_button(PD_14);
InterruptIn black_button(PB_2);

extern volatile bool lock_state;

int passcode_digit = 4;
int passcode[4] = {2, 3, 0, 1};// 0 = black, 1 = green, 2 = red, 3 = blue
int button_pressed[4] = {0, 0, 0, 0};

int arr_index = 0;
int input_arr[4];

// For EventQueue cancelling
int event_ids[4] = {0, 0, 0, 0};
EventQueue* queue_ptr = nullptr;

// Compare two arrays
bool isEqual(int x[], int y[], int size) {
    for (int i = 0; i < size; ++i) {
        if (x[i] != y[i]){
            return false;
        }
    }
    return true;
}

// Disable all IRQs
void disable_all_irqs() {
    blue_button.disable_irq();
    red_button.disable_irq();
    green_button.disable_irq();
    black_button.disable_irq();
}

// Enable all IRQs
void enable_all_irqs() {
    blue_button.enable_irq();
    red_button.enable_irq();
    green_button.enable_irq();
    black_button.enable_irq();
}

// Cancel all queued button events
void cancel_all_events() {
    for (int i = 0; i < 4; i++) {
        if (event_ids[i] != 0) {
            queue_ptr->cancel(event_ids[i]);
            event_ids[i] = 0;
        }
    }
}

// Passcode check
void check_passcode() {
    if (arr_index == passcode_digit) {
        disable_all_irqs();
        cancel_all_events();

        if (isEqual(passcode, input_arr, passcode_digit)) {
            if (lock_state) {
                unlock();
            }
            printf("Correct Passcode: system unlocked\n");
        } else {
            printf("Incorrect Passcode\n");
        }

        ThisThread::sleep_for(500ms);

        arr_index = 0;
        for (int i = 0; i < 4; ++i) {
            button_pressed[i] = 0;
        }

        enable_all_irqs();
    }
}

// Button press handler
void handle_button(int value) {
    if (button_pressed[value] == 0) {
        printf("Button %d pressed!\n", value);
        button_pressed[value] = 1;
        input_arr[arr_index] = value;
        arr_index++;
        check_passcode();
    }
}

void blue_handler(){
    event_ids[3] = queue_ptr->call(handle_button, 3);
}

void red_handler(){
    event_ids[2] = queue_ptr->call(handle_button, 2);
}

void green_handler(){
    event_ids[1] = queue_ptr->call(handle_button, 1);
}

void black_handler(){
    event_ids[0] = queue_ptr->call(handle_button, 0);
}

// Setup buttons and interrupts
void init_buttons(EventQueue &queue) {
    queue_ptr = &queue;

    blue_button.mode(PullUp);
    red_button.mode(PullUp);
    green_button.mode(PullUp);
    black_button.mode(PullUp);

    blue_button.fall(blue_handler);
    red_button.fall(red_handler);
    green_button.fall(green_handler);
    black_button.fall(black_handler);
}
