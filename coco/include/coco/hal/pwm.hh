#pragma once

namespace coco {

enum class pwm_instance {
    _1,
};

struct pwm_configuration {
    bool Enabled {};
    int PeriodUs {};
    int PulseWidthUs {};
};

void* pwm_retrieve_instance(pwm_instance Instance);

int pwm_init(void* PWMInstance);

pwm_configuration pwm_get(void* PWMInstance);

void pwm_enable(void* PWMInstance);

void pwm_disable(void* PWMInstance);

void pwm_change_period(void* PWMInstance, int PeriodUs);

void pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs);

} /* namespace coco */

