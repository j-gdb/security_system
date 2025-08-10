#include "tof.hpp"

DevI2C i2c(PB_11, PB_10);//SDA, SCL
DigitalOut xshut(PC_6);//Reset pin

VL53L0X sensor(&i2c, &xshut, PC_7);
VL53L0X_DEV dev;

extern volatile bool lock_state;

bool initial_measurement = true;

EventQueue *global_queue = nullptr;

// track previous readings for consistency checking
volatile uint16_t prev_distance_measured = 0;
const uint16_t MAX_ALLOWED_CHANGE = 20; // max change allowed between readings

// checks if distance on the sensor changed and toggles the state based on if distance changed
void process_distance() {
    VL53L0X_RangingMeasurementData_t data;
    sensor.handle_irq(range_continuous_interrupt, &data);

    uint16_t distance_measured = data.RangeMilliMeter;
    
    // debugging
    // printf("Distance: %d mm\n", distance_measured);
    
    // range for ToF
    const uint16_t MIN_VALID_DISTANCE = 50;   
    const uint16_t MAX_VALID_DISTANCE = 350;
    
    // do nothing for first reading, store it and return
    if (initial_measurement) {
        prev_distance_measured = distance_measured;
        initial_measurement = false;
        return;
    }
    
    // abs diff between current / previous
    uint16_t distance_change = abs((int)distance_measured - (int)prev_distance_measured);
    
    // check:
    // -reading is within the valid range for detection
    // -change from the previous reading is small
    // -system is currently unlocked
    if (distance_measured >= MIN_VALID_DISTANCE && 
        distance_measured <= MAX_VALID_DISTANCE && 
        distance_change <= MAX_ALLOWED_CHANGE &&
        !lock_state) {
        
        printf("Valid detection at %d mm (change: %d mm) - locking\n", 
              distance_measured, distance_change);
        toggle_lock();
    }

    // update previous distance for next reading
    prev_distance_measured = distance_measured;
}

// Interrupt Request Handler to avoid errors in the process distance function
void irq_handler() {
    if (global_queue) {
        global_queue->call(process_distance);
    }
}

// initialize the VL53L0X sensor and the interrupts connected to the sensor
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
    
    // setup the ToF sensor
    sensor.vl53l0x_get_device(&dev);
    sensor.VL53L0X_set_gpio_config(dev, 0, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING, VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY, VL53L0X_INTERRUPTPOLARITY_LOW);

    // attach and enable the sensor to our interrupts
    sensor.attach_interrupt_measure_detection_irq(irq_handler);
    sensor.enable_interrupt_measure_detection_irq();

    // start the sensor
    sensor.start_measurement(range_continuous_interrupt, irq_handler);
    return 0;
}