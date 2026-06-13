#include "coco/hal/pwm.hh"

#include <cstdio>

namespace coco {

struct pwm_mock {
    const char* Name {};
    int PulseWidthUs {};
};

static pwm_mock PWMInstances [] {
    {
        .Name {"PWM Instance 1"},
        .PulseWidthUs {},
    },
    {
        .Name {"PWM Instance 2"},
        .PulseWidthUs {},
    },
};

void* pwm_retrieve_instance(pwm_instance Instance) {
    switch (Instance) {
        case pwm_instance::_1: {
            return &PWMInstances[static_cast<int>(Instance)];
        }
    }

    return nullptr;
}

int pwm_init(void* PWMInstance) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    printf("PWM Instance name: %s\n", MockPWMInstance->Name);
    return 0;
}

int pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    MockPWMInstance->PulseWidthUs = PulseWidthUs;
    return 0;
}

int pwm_get(void* PWMInstance, pwm_configuration& PWMConfiguration) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    PWMConfiguration.PulseWidthUs = MockPWMInstance->PulseWidthUs;
    return 0;
}

} /* namespace coco */

