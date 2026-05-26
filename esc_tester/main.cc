#include "servo_control.hh"
#include "stm32f4xx_ll_tim.h"

int main() {
    servo::state {.PWMTimer {TIM5}};
}

