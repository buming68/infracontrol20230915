/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "sys.h"
#include "usart.h"
#include <string.h>
//#include "stdlib.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ledTaskHandle;
osThreadId infraContolTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LedTask(void const * argument);
void InfraControlTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ledTask */
  osThreadDef(ledTask, LedTask, osPriorityBelowNormal, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

  /* definition and creation of infraContolTask */
  osThreadDef(infraContolTask, InfraControlTask, osPriorityNormal, 0, 128);
  infraContolTaskHandle = osThreadCreate(osThread(infraContolTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LedTask */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedTask */
void LedTask(void const * argument)
{
  /* USER CODE BEGIN LedTask */
  /* Infinite loop */
  for(;;)
  {
      
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);    //指示灯  
    printf("infra control  ver 1.0\r\n");
    osDelay(60000);
            
  }
  /* USER CODE END LedTask */
}

/* USER CODE BEGIN Header_InfraControlTask */
/**
* @brief Function implementing the infraContolTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_InfraControlTask */
void InfraControlTask(void const * argument)
{
  /* USER CODE BEGIN InfraControlTask */
//    int i;
    int j; 
    int  channelcount = 0;
    char chstring[] = "168"; 
    uint8_t infradata[10][5] = {{0xFA, 0xFD, 0x02, 0x00, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x01, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x02, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x03, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x04, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x05, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x06, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x07, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x08, 0xDF},
                                {0xFA, 0xFD, 0x02, 0x09, 0xDF}                               
        }; 
    
//    int channel[12] = {1, 2, 3, 4, 5, 6, 88, 95, 152, 153, 154, 155}; //高清+标清  12个频道
    int channel[6] = {1, 2, 3, 4, 5, 6};            //标清  6个频道，或高清机顶盒  6个频道
  for(;;)
  {
         
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);    //指示灯  
    if  (channelcount >=  sizeof(channel)/sizeof(int)){     //channnelcount 为每个频道号的计数，一个循环发送一个频道
        channelcount = 0;    
    }       
        sprintf(chstring, "%d", channel[channelcount]);     //将频道号整数转换成字符串
//        printf(" length=%d\n", strlen(chstring));
        
        for (j = 0; j < strlen(chstring); j++) {            //将频道号字符串逐个字提取，并发送至指定地址
            HAL_UART_Transmit(&huart1, infradata[chstring[j]-48], 5, 1000);    //发往com1,测试数据,字符数字的ascii值-48（0x30）就是十进制数字
            HAL_UART_Transmit(&huart3, infradata[chstring[j]-48], 5, 1000);    //发往com1,测试数据
            osDelay(100);
            HAL_UART_Transmit(&huart3, infradata[chstring[j]-48], 5, 1000);    //发往com1,测试数据
            osDelay(100);
            HAL_UART_Transmit(&huart3, infradata[chstring[j]-48], 5, 1000);    //发往com1,测试数据
            osDelay(1000);
        } 
    channelcount++;  
    
    osDelay(3000);
  }
  /* USER CODE END InfraControlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

