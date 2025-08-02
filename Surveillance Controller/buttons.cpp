#include "mbed.h"

InterruptIn blue_button(PC_0); 
InterruptIn red_button(PC_1);
InterruptIn green_button(PC_2);
InterruptIn black_button(PC_3);



int passcode_digit = 4;
int passcode[4] = {2, 3, 0, 1};
int arr_index = 0;
int input_arr[4];
// 0 = blue, 1 = red, 2 = green, 3 = black

int isEqual(int x[], int y[], int size){
    for (int i = 0; i < size; ++i) {
        if (x[i] != y[i])
            return false;
    }
    return true;
}

void check_passcode(){
    if (arr_index == passcode_digit){
        if (isEqual(passcode, input_arr, passcode_digit)){
            printf("Correct Passcode\n");
        } else {
            printf("Incorrect Passcode\n");
        }
        arr_index = 0;
    }
}

void handle_button(int value) {
    printf("Button %d pressed!\n", value);
    input_arr[arr_index] = value;
    arr_index++;
    check_passcode();
    ThisThread::sleep_for(300ms);
}

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