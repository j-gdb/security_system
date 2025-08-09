#ifndef TOF_HPP
#define TOF_HPP

#include "mbed.h"
#include "VL53L0X.h"
#include "toggle.hpp"

extern DevI2C i2c;
extern DigitalOut xshut;
extern VL53L0X sensor;
extern VL53L0X_DEV dev;

extern EventQueue queue;
extern Thread event_thread;

extern volatile uint16_t last_distance;

int init_sensor(EventQueue &queue);
void process_distance();
void irq_handler();

#endif 
