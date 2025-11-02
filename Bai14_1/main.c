#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// Ð?nh nghia bit cho t?ng task
#define BIT_TASK2   (1 << 0)
#define BIT_TASK3   (1 << 1)
#define BIT_TASK4   (1 << 2)

EventGroupHandle_t xEventGroup;

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio;

    // PA2, PA3, PA4: LED
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOA, &gpio);

    // PA0: nút nh?n
    gpio.GPIO_Mode = GPIO_Mode_IPU; // kéo lên n?i
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio);

    // T?t t?t c? LED ban d?u
    GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
}

void vTaskButton(void *pv)
{
    uint8_t mode = 0;
    uint8_t lastState = 1;

    while (1)
    {
        uint8_t now = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

        if (now == 0 && lastState == 1) // phát hi?n nh?n
        {
            vTaskDelay(20); // ch?ng d?i phím
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
            {
                mode = (mode + 1) % 4; // 0: t?t, 1: LED1, 2: LED2, 3: LED3

                // Xóa toàn b? bit tru?c
                xEventGroupClearBits(xEventGroup, BIT_TASK2 | BIT_TASK3 | BIT_TASK4);

                // B?t bit tuong ?ng
                if (mode == 1)
                    xEventGroupSetBits(xEventGroup, BIT_TASK2);
                else if (mode == 2)
                    xEventGroupSetBits(xEventGroup, BIT_TASK3);
                else if (mode == 3)
                    xEventGroupSetBits(xEventGroup, BIT_TASK4);
            }
        }

        lastState = now;
        vTaskDelay(50);
    }
}

void vTaskLED1(void *pv)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(xEventGroup, BIT_TASK2, pdFALSE, pdFALSE, portMAX_DELAY);

        if (bits & BIT_TASK2)
        {
            GPIOA->ODR ^= GPIO_Pin_2; // LED1 nháy
            vTaskDelay(300 / portTICK_RATE_MS);
        }
        else
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_2); // t?t LED
            vTaskDelay(10);
        }
    }
}


void vTaskLED2(void *pv)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(xEventGroup, BIT_TASK3, pdFALSE, pdFALSE, portMAX_DELAY);

        if (bits & BIT_TASK3)
        {
            GPIOA->ODR ^= GPIO_Pin_3; // LED2 nháy
            vTaskDelay(500 / portTICK_RATE_MS);
        }
        else
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_3);
            vTaskDelay(10);
        }
    }
}


void vTaskLED3(void *pv)
{
    while (1)
    {
        EventBits_t bits = xEventGroupWaitBits(xEventGroup, BIT_TASK4, pdFALSE, pdFALSE, portMAX_DELAY);

        if (bits & BIT_TASK4)
        {
            GPIOA->ODR ^= GPIO_Pin_4; // LED3 nháy
            vTaskDelay(700 / portTICK_RATE_MS);
        }
        else
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
            vTaskDelay(10);
        }
    }
}


int main(void)
{
    SystemInit();
    GPIO_Config();

    xEventGroup = xEventGroupCreate();

    xTaskCreate(vTaskButton, "Button", 128, NULL, 2, NULL);
    xTaskCreate(vTaskLED1, "LED1", 128, NULL, 1, NULL);
    xTaskCreate(vTaskLED2, "LED2", 128, NULL, 1, NULL);
    xTaskCreate(vTaskLED3, "LED3", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
