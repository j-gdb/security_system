#include "mbed.h"
#include "VL53L0X.h"
#include "toggle.hpp"

DevI2C i2c(PB_11, PB_10);//SDA, SCL
DigitalOut xshut(PC_6);//Reset pin

VL53L0X sensor(&i2c, &xshut, PC_7);
VL53L0X_DEV dev;

extern volatile bool lock_state;

EventQueue *global_queue = nullptr;

uint16_t last_distance = 0;

void process_distance() {
    VL53L0X_RangingMeasurementData_t data;
    sensor.handle_irq(range_continuous_interrupt, &data);

    uint16_t distance_measured = data.RangeMilliMeter;

    if (distance_measured > last_distance+10 || distance_measured < last_distance-10) {
        printf("Distance: %d mm\n", distance_measured);
        last_distance = distance_measured;
        if (lock_state){
            toggle_lock();
        }
    }
}

void irq_handler() {
    if (global_queue) {
        global_queue->call(process_distance);//queue calls the function process_distance
    }
}

int init_sensor(EventQueue &queue){
    
    global_queue = &queue;

    //restart the VL53L0X Sensor
    sensor.VL53L0X_off();
    ThisThread::sleep_for(10ms);
    sensor.VL53L0X_on();

    //initialize the sensor
    if (sensor.init(nullptr) != 0) {
        printf("VL53L0X init failed!\n");
        return 1;
    }

    //prepare the sensor
    if (sensor.prepare() != 0) {
        printf("VL53L0X prepare failed!\n");
        return 1;
    }

    sensor.vl53l0x_get_device(&dev);//get the device that were using (STM32 B-L475E)

    sensor.VL53L0X_set_gpio_config(dev, 0, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY, VL53L0X_INTERRUPTPOLARITY_LOW);//set the config for our device
    sensor.attach_interrupt_measure_detection_irq(irq_handler);//attach our irq for our sensor
    sensor.enable_interrupt_measure_detection_irq();//enable the attached irq
    sensor.start_measurement(range_continuous_interrupt, irq_handler);//start measuring
    return 0;
}