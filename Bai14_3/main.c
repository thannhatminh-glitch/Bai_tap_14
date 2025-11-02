#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>

void UART1_Init(void);
void UART1_SendString(char *str);
void RTC_Config(void);
void Delay(volatile uint32_t nCount);

int main(void)
{
    // Cho phép xóa c? reset t? Standby
    if (RCC_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {
        RCC_ClearFlag();
        PWR_ClearFlag(PWR_FLAG_WU);
    }

    SystemInit();
    UART1_Init();
    RTC_Config();

    UART1_SendString("Wake up, sending data...\r\n");
    Delay(1000000);
    UART1_SendString("Hello from STM32\r\n");

    // C?u hình RTC báo th?c sau 5 giây
    RTC_SetCounter(0);
    RTC_WaitForLastTask();

    // Cho phép Standby
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_EnterSTANDBYMode();

    while (1);
}

//---------------- UART -----------------
void UART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // PA9 (TX)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10 (RX)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void UART1_SendString(char *str)
{
    while (*str)
    {
        USART_SendData(USART1, *str++);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

//---------------- RTC -----------------
void RTC_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);

        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        RTC_WaitForSynchro();
    }
}

//---------------- Delay -----------------
void Delay(volatile uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}
