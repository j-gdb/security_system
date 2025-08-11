#include "buttons.hpp"

InterruptIn blue_button(PA_4); 
InterruptIn red_button(PA_3);
InterruptIn green_button(PD_14);

InterruptIn black_button(PB_2);

extern volatile bool lock_state;

int passcode_digit = 3;
int passcode[4] = {1, 0, 2};// 0 = green, 1 = red, 2 = blue
int volatile button_pressed[4] = {0, 0, 0};

int arr_index = 0;
int input_arr[3];

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
            unlock();
            printf("Correct Passcode: system unlocked\n");
        } else {
            printf("Incorrect Passcode\n");
        }
    }
}

// interrupt handler for the button press
void handle_button(int value) {
    if (button_pressed[value] == 0 && lock_state){
        printf("Button %d pressed\n", value);
        button_pressed[value] = 1;
        input_arr[arr_index] = value;
        arr_index++;
        check_passcode();
    }
}

// resets password
void reset_password(){
    arr_index = 0;
    for (int i = 0; i < 4; ++i) {
        button_pressed[i] = 0;
    }
    printf("Password Reset\n");
}

// initializes the connection between the external buttons and the interrupts
void init_buttons(EventQueue &queue){
    
    blue_button.mode(PullUp);
    red_button.mode(PullUp);
    green_button.mode(PullUp);
    black_button.mode(PullUp);

    blue_button.rise([&]{ queue.call(handle_button, 2); });
    red_button.rise([&]{ queue.call(handle_button, 1); });
    green_button.rise([&]{ queue.call(handle_button, 0); });
    black_button.rise([&]{ queue.call(reset_password); });

}