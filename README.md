# CSC385 Security System

## Surveillance Controller (ToF Sensor & Buttons)

## Description
This microcontroller is tasked to detecting any changes on the Time of Flight Sensor (VL53L0X) and also unlocking the Security System by typing the password on the connected external buttons.

## Libraries
- We used a modified version of the STM32 VL53L0X library which is included in the Surviellance Folder. However if the there are problems with the IDE not recognizing the library folder you can manually import the library via the link here: http://os.mbed.com/teams/ST/code/VL53L0X/ and replacing all wait_ms in any file with thread_sleep_for. 
- 

### Prerequisites
- Use the Mbed Studio as the reccomended IDE
- Connect the wires from the bread board to the STM32 microcontroller
    - GND is connected on CN2 (6) but any GND pin should do
    - connect the black button to PA_4 (CN3)
    - connect the green button to PA_3 (CN3)
    - connect the red button to PD_14 (CN3)
    - connect the blue button to PB_2 (CN1)
- Connect the wires from the Surveillance Controller to this microcontroller
    - connect the Microphone microcontroller to PA_15 (CN1). See Surviellance Controller (Microphone) for more information below.
- Modify the htttp_post_req.cpp file to the IP, port, SSD and PWD of your wifi router

### Steps
1. open the Surviellance Controller Folder in the Mbed Studio as your active program
2. select the DISCO-L475VG-IOT01A (B-L475E-IOT01A) microcontroller as the target board
3. make sure that this microcontroller is connected with the breadboard and the other Surviellance Controller
4. connect the microcontroller with the usb cord onto your PC
5. place the microcontroller towards the ceiling to avoid triggering the device when started
6. build and run on the program
7. hovering your hand over the ToF sensor should trigger the sensor and lock the system
8. presssing a sequence of buttons in the correct order should unlock the system. (Red - Blue - Black - Green)

## Surveillance Controller (Microphone)


## Command Controller 