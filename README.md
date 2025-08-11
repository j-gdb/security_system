# CSC385 Security System

## Surveillance Controller (ToF Sensor & Buttons)
This microcontroller is tasked to detecting any changes on the Time of Flight Sensor (VL53L0X) and also unlocking the Security System by typing the password on the connected external buttons.

### Libraries
- We used a modified version of the STM32 VL53L0X library which is included in the Surviellance Folder. However if the there are problems with the IDE not recognizing the library folder you can manually import the library via the link here: http://os.mbed.com/teams/ST/code/VL53L0X/ and replacing all wait_ms in any file with thread_sleep_for. 
- We also used a custom driver for the on-board ISM43362 WiFi module. This driver overrwites some functions in the MbedOS library allowing 
our specific boards to connect to the internet. The driver is lightweight and included within this codebase and its repository link is 
the following: https://github.com/ARMmbed/wifi-ism43362 

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
1. Open the Surviellance Controller Folder in the Mbed Studio as your active program
2. Select the DISCO-L475VG-IOT01A (B-L475E-IOT01A) microcontroller as the target board
3. Make sure that this microcontroller is connected with the breadboard and the Microphone Surviellance Controller
4. Connect the microcontroller with the usb cord onto your PC
5. Place the microcontroller towards the ceiling to avoid triggering the device when started
6. Build and run on the program
7. Hovering your hand over the ToF sensor should trigger the sensor and lock the system
8. Presssing a sequence of buttons in the correct order should unlock the system. (Red - Blue - Black - Green)

## Surveillance Controller (Microphone)
The microphone sensor component integrates with the main Surveillance Controller to provide an audio-based method of unlocking the security system. When a specific sound level is detected, it sends a signal to the main Surveillance Controller to unlock the system. Unlike the other boards, the code for the microphone was written in the STM32Cube framework, rather than the Mebd framework. The specific hardware set up is included in the "Microphone Controller.ioc" file. For more information about the hardware setup of the microphone, this is the tutorial that was followed: https://www.youtube.com/watch?v=MdDqVeIGhec

### Prerequisites
- Use the STM32Cube IDE as the reccomended IDE
- Connect the wires the Microphone Controller to the main Surveillance Controller.
    - Create a ground connection between the Microphone Controller and the Surveillance Controller.
    - Connect PB_8 on the Microphone Controller to PA_15 on the main Surveillance Controller.

### Steps
1. Open the Microphone Controller folder and navigate to the .project.
2. Double click on the .project file. This should open up the project in the STM32Cube IDE.
3. Connect the microcontroller with the usb cord onto your PC.
4. Build and run the program.
5. Any loud noises should be picked up by the microphone, and send a digial signal will be sent out.

## Command Controller 
This microcontroller manages the physical actuator (servo motor) of the security system based on commands received from the server. It polls a central server for the current lock state and controls the servo motor accordingly. The servo motor used was an sg90. However, since it is likely a knockoff, the pulsewidths are different from what the datasheet says they need to be.

### Prerequisites
- Use the Mbed IDE as the reccomended IDE.
- Connect the wires between the power source and the motor
- Connect the motor to the microcontroller through PA_3, and create a common ground with the microcontroller and ground

### Steps
1. Open the Command Controller Folder in the Mbed Studio as your active program.
2. Select the DISCO-L475VG-IOT01A (B-L475E-IOT01A) microcontroller as the target board.
3. Connect the microcontroller with the usb cord onto your PC.
4. Build and run on the program.
5. When the program is notified to unlock or lock, you will see the motor change accordingly.
