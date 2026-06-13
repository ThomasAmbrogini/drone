#pragma once

namespace coco {

enum class pwm_instance {
    _1,
};

struct pwm_configuration {
    int PulseWidthUs {};
};

void* pwm_retrieve_instance(pwm_instance Instance);

int pwm_init(void* PWMInstance);

int pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs);

int pwm_get(void* PWMInstance, pwm_configuration& PWMConfiguration);

} /* namespace coco */

