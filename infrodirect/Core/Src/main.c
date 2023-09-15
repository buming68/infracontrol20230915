/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stdio.h"
//#include "delay.h"
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

/* USER CODE BEGIN PV */
uint32_t send_Code = 0;
//uint8_t addr = 0x00;
//uint8_t data = 0x31;

uint8_t userdata1 = 0x57, userdate2 = 0x59, userdata3 = 0x58; 
uint8_t data1 = 0x30, data2 = 0x32;     //test data "2" keycode = 0x30,0x32

uint8_t wxyxdata = 0x33;

int channel[12] = {1, 2, 3, 4, 5, 6, 88, 95, 152, 153, 154, 155}; //����+����  12��Ƶ��
int channel6[6] = {1, 2, 3, 4, 5, 6}; //6�������6������Ƶ��
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        nusȡֵ��Χ : 0~190887435(���ֵ�� 2^32 / fac_us @fac_us = 21)
 * @retval      ��
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * 72;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else 
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
}



// �������ߺ���ң����˽��Э��  40bit
void infrasend_wxyx(uint8_t bdata){
     HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        //start1
     HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);        //start1
    
    delay_us(3640);                                 //TVOS portocol 
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);      
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);   
    
    delay_us(1800);                                 //TVOS portocol 

    send_Code = userdata3<< 16 | userdate2<<8 | userdata1;
    for(int i=0; i<24; i++){        
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);        
        
        delay_us(560);
        HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
        
        if((send_Code>>i) & 0x01){
            delay_us(1120);
        }
        else
            delay_us(560);
    }
    
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        //start2
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);        //start2    
    
    delay_us(3640);                                 //TVOS portocol  
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);   
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1); 
    
    delay_us(1800);                                 //TVOS portocol 
  
    send_Code = bdata<<8 | data1;
    for(int i=0; i<16; i++){  
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
        
        delay_us(560);
        HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);        
        HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
        
        if((send_Code>>i) & 0x01)
            delay_us(1120);
        else
            delay_us(560);
    }
    
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        //stop�����룡������
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);        //stop�����룡������
    
    delay_us(560);        
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);    
    
    delay_us(54000);

    
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        //repeat code
//    delay_us(3640);             //TVOS portocol  
//    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);      
//    delay_us(3640);  
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        
//    delay_us(560);
//    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);  
//    delay_us(50000);
//    delay_us(50160);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    TIM2->ARR = 1894;
    TIM2->CCR2 = TIM2->ARR/2;                           //ռ�ձ�50%
    
    TIM2->CCR1 = TIM2->ARR/2;                           //ռ�ձ�50%
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int  channelcount = 0;
  char chstring[] = "168";    
  int j;
  while (1)
  {
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)){
        if (channelcount >=  sizeof(channel)/sizeof(int)){     //channnelcount Ϊÿ��Ƶ���ŵļ�����һ��ѭ������һ��Ƶ��
            channelcount = 0;}
    }else{
        if (channelcount >=  sizeof(channel6)/sizeof(int)){     //channnelcount Ϊÿ��Ƶ���ŵļ�����һ��ѭ������һ��Ƶ��
            channelcount = 0;}
    }  
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)){
        sprintf(chstring, "%d", channel[channelcount]);     //��Ƶ��������ת�����ַ���   
    }else{
        sprintf(chstring, "%d", channel6[channelcount]);     //��Ƶ��������ת�����ַ���   
    }
    

  for (j = 0; j < strlen(chstring); j++) {            //��Ƶ�����ַ����������ȡ����������ָ����ַ
    infrasend_wxyx(chstring[j]);                       //�����������ߵ��ӱ�׼�Ļ����п����źţ�Ϊ����
    HAL_Delay(800); 
            
    } 
  
    
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    HAL_Delay(50);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    
    channelcount++;  
    
    
    HAL_Delay(3000);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);  
    HAL_Delay(3000);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);  
    HAL_Delay(3000);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    HAL_Delay(3000);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);     
    HAL_Delay(3000);
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);      

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
       
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);        //test 38KHz ����  1
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);        //start1
//    
//    delay_us(5000);                                 //TVOS portocol 
//    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);         //���� 0  
//    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);   
      
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
