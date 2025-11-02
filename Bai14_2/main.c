#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"

#define USE_WFI 1

void Button_wakeup(void);
void delay_ms(uint32_t ms)
{
    for(uint32_t i=0; i < ms * 8000; i++) {
        __NOP(); 
    }
}
int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpio);

    for (int k = 0; k < 5; k++) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        delay_ms(300);
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
        delay_ms(300);
    }

    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    GPIO_DeInit(GPIOC);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);

    Button_wakeup();
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

#if USE_WFI
    __WFI();
#else
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
    __SEV();
    __WFE();
    __WFE();
#endif

    while (1) {
			
			__NOP();
    }
}

void Button_wakeup(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitTypeDef exti;
    EXTI_StructInit(&exti);
    exti.EXTI_Line = EXTI_Line0;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    EXTI_ClearITPendingBit(EXTI_Line0);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}