#include "coco/hal/pwm.hh"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_tim.h"

namespace hal {

void* pwm_retrieve_instance(pwm_instance Instance) {
    switch (Instance) {
        case pwm_instance::_1: return TIM5;
    }
    return nullptr;
}

int pwm_init(void* PWMInstance) {
    TIM_TypeDef* Timer = static_cast<TIM_TypeDef*>(PWMInstance);

    //TODO: this should not be done in here.
    if (Timer == TIM5) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
    }

    //TODO: I should have some type of configuration to pass.
    // 50 Hz PWM: prescaler=15 -> 1 MHz tick (16 MHz HSI, APB1 div 1)
    // autoreload=19999 -> 20 ms period
    LL_TIM_InitTypeDef TIMInit {};
    TIMInit.Prescaler         = 15;
    TIMInit.Autoreload        = 19999;
    TIMInit.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
    TIMInit.CounterMode       = LL_TIM_COUNTERMODE_UP;
    TIMInit.RepetitionCounter = 0;

    if (LL_TIM_Init(Timer, &TIMInit) != SUCCESS) {
        return -1;
    }

    // CH1: PWM mode 1, initial compare = 1000 ticks (1 ms = 0% throttle)
    LL_TIM_OC_InitTypeDef OCInit {};
    OCInit.OCMode       = LL_TIM_OCMODE_PWM1;
    OCInit.OCState      = LL_TIM_OCSTATE_ENABLE;
    OCInit.OCNState     = LL_TIM_OCSTATE_DISABLE;
    OCInit.CompareValue = 1000;
    OCInit.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;
    OCInit.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
    OCInit.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
    OCInit.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;

    if (LL_TIM_OC_Init(Timer, LL_TIM_CHANNEL_CH1, &OCInit) != SUCCESS) {
        return -1;
    }

    LL_TIM_CC_EnableChannel(Timer, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(Timer);

    return 0;
}

// Assumes the timer input clock equals SystemCoreClock (APB prescaler = 1).
// If the timer is on an APB bus with a prescaler > 1, replace SystemCoreClock
// with the actual timer input clock (2 * APBx clock on STM32F4).
int pwm_change_pulse_width(void* PWMInstance, int PulseWidthUs) {
    TIM_TypeDef* Timer = static_cast<TIM_TypeDef*>(PWMInstance);
    uint32_t prescaler = LL_TIM_GetPrescaler(Timer) + 1;
    uint32_t compare = static_cast<uint32_t>(
        static_cast<uint64_t>(PulseWidthUs) * SystemCoreClock / prescaler / 1000000UL
    );
    LL_TIM_OC_SetCompareCH1(Timer, compare);
    return 0;
}

} /* namespace hal */
