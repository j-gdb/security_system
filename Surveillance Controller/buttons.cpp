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

// checks if two arrays are the same
bool isEqual(int x[], int y[], int size){
    for (int i = 0; i < size; ++i) {
        if (x[i] != y[i])
            return false;
    }
    return true;
}

// checks if the users combination is the same as the password
void check_passcode(){
    if (arr_index == passcode_digit){
        if (isEqual(passcode, input_arr, passcode_digit)){
            if (lock_state){
                toggle_lock();
            }
            printf("Correct Passcode: system unlocked\n");
        } else {
            printf("Incorrect Passcode\n");
        }
        arr_index = 0;
        for (int i = 0; i < 4; ++i) {
            button_pressed[i] = 0;
        }
    }
}

// interrupt handler for the button press
void handle_button(int value) {
    if (button_pressed[value] == 0){
        printf("Button %d pressed!\n", value);
        button_pressed[value] = 1;
        input_arr[arr_index] = value;
        arr_index++;
        check_passcode();
        ThisThread::sleep_for(300ms);
    }
}

// initializes the connection between the external buttons and the interrupts
void init_buttons(EventQueue &queue){
    
    blue_button.mode(PullUp);
    red_button.mode(PullUp);
    green_button.mode(PullUp);
    black_button.mode(PullUp);

    blue_button.fall([&]{ queue.call(handle_button, 0); });
    red_button.fall([&]{ queue.call(handle_button, 1); });
    green_button.fall([&]{ queue.call(handle_button, 2); });
    black_button.fall([&]{ queue.call(handle_button, 3); });

}