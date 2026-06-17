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

struct pwm_handle {
    void* PWMInstance {};
    int PWMChannel {};
};

pwm_handle* pwm_retrieve_instance(pwm_instance Instance);

int pwm_init(pwm_handle* PWMHandle);

pwm_configuration pwm_get(pwm_handle* PWMHandle);

void pwm_enable(pwm_handle* PWMHandle);

void pwm_disable(pwm_handle* PWMHandle);

void pwm_change_period(pwm_handle* PWMHandle, int PeriodUs);

void pwm_change_pulse_width(pwm_handle* PWMHandle, int PulseWidthUs);

} /* namespace coco */

