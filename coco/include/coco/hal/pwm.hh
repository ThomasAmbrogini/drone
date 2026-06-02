#pragma once

namespace hal {

enum class pwm_instance {
    _1,
};

void* pwm_retrieve_instance(pwm_instance Instance);

int pwm_init(void* PWMInstance);

int pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs);

} /* namespace hal */

