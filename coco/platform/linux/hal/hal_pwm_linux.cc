#include "coco/hal/pwm.hh"

#include <cstdio>

namespace coco {

struct pwm_mock {
    const char* Name {};
    bool Enabled {};
    int PeriodUs {};
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

pwm_configuration pwm_get(void* PWMInstance) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    return pwm_configuration {
        .Enabled {MockPWMInstance->Enabled},
        .PeriodUs {MockPWMInstance->PeriodUs},
        .PulseWidthUs {MockPWMInstance->PulseWidthUs},
    };
}

void pwm_enable(void* PWMInstance) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    MockPWMInstance->Enabled = true;
}

void pwm_disable(void* PWMInstance) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    MockPWMInstance->Enabled = false;
}

void pwm_change_period(void* PWMInstance, int PeriodUs) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    MockPWMInstance->PeriodUs = PeriodUs;
}

void pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs) {
    pwm_mock* MockPWMInstance {reinterpret_cast<pwm_mock*>(PWMInstance)};
    MockPWMInstance->PulseWidthUs = PulseWidthUs;
}

} /* namespace coco */

