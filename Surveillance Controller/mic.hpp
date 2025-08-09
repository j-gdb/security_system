#ifndef MIC_HPP
#define MIC_HPP

#include "mbed.h"
#include "toggle.hpp"

extern InterruptIn mic;

void signal();
void init_mic_response();

#endif 
