/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

#include "event_groups.h"
#include "tim.h"
#include "remote_control.h"
#include "trig_task.h"
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

EventGroupHandle_t my_shootEventGroupHandle;

/* USER CODE END Variables */
osThreadId TEST_TASKHandle;
osThreadId CHASSIS_TASKHandle;
osThreadId INS_TASKHandle;
osThreadId DETECT_TASKHandle;
osThreadId VOFA_TASKHandle;
osThreadId COMMUNICATE_TASHandle;
osThreadId TRIG_TASKHandle;
osTimerId ShootTimerHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void test_task(void const * argument);
void chassis_task(void const * argument);
void INS_task(void const * argument);
void detect_task(void const * argument);
void vofa_task(void const * argument);
void communicate_task(void const * argument);
void trig_task(void const * argument);
void ShootTimer_Callback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

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

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	
	#ifdef CASCADE
	my_shootEventGroupHandle = xEventGroupCreate();
	if(my_shootEventGroupHandle == NULL)
	{
		return;
	}
	#endif
	HAL_TIM_Base_Start_IT(&htim5);
	
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of ShootTimer */
  osTimerDef(ShootTimer, ShootTimer_Callback);
  ShootTimerHandle = osTimerCreate(osTimer(ShootTimer), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
	
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of TEST_TASK */
  osThreadDef(TEST_TASK, test_task, osPriorityLow, 0, 128);
  TEST_TASKHandle = osThreadCreate(osThread(TEST_TASK), NULL);

  /* definition and creation of CHASSIS_TASK */
  osThreadDef(CHASSIS_TASK, chassis_task, osPriorityNormal, 0, 256);
  CHASSIS_TASKHandle = osThreadCreate(osThread(CHASSIS_TASK), NULL);

  /* definition and creation of INS_TASK */
  osThreadDef(INS_TASK, INS_task, osPriorityRealtime, 0, 1024);
  INS_TASKHandle = osThreadCreate(osThread(INS_TASK), NULL);

  /* definition and creation of DETECT_TASK */
  osThreadDef(DETECT_TASK, detect_task, osPriorityBelowNormal, 0, 256);
  DETECT_TASKHandle = osThreadCreate(osThread(DETECT_TASK), NULL);

  /* definition and creation of VOFA_TASK */
  osThreadDef(VOFA_TASK, vofa_task, osPriorityNormal, 0, 128);
  VOFA_TASKHandle = osThreadCreate(osThread(VOFA_TASK), NULL);

  /* definition and creation of COMMUNICATE_TAS */
  osThreadDef(COMMUNICATE_TAS, communicate_task, osPriorityNormal, 0, 256);
  COMMUNICATE_TASHandle = osThreadCreate(osThread(COMMUNICATE_TAS), NULL);

  /* definition and creation of TRIG_TASK */
  osThreadDef(TRIG_TASK, trig_task, osPriorityNormal, 0, 128);
  TRIG_TASKHandle = osThreadCreate(osThread(TRIG_TASK), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_test_task */
/**
  * @brief  Function implementing the TEST_TASK thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_test_task */
__weak void test_task(void const * argument)
{

  /* USER CODE BEGIN test_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END test_task */
}

/* USER CODE BEGIN Header_chassis_task */
/**
* @brief Function implementing the CHASSIS_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_chassis_task */
__weak void chassis_task(void const * argument)
{
  /* USER CODE BEGIN chassis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chassis_task */
}

/* USER CODE BEGIN Header_INS_task */
/**
* @brief Function implementing the INS_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_INS_task */
__weak void INS_task(void const * argument)
{
  /* USER CODE BEGIN INS_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END INS_task */
}

/* USER CODE BEGIN Header_detect_task */
/**
* @brief Function implementing the DETECT_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_detect_task */
__weak void detect_task(void const * argument)
{
  /* USER CODE BEGIN detect_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END detect_task */
}

/* USER CODE BEGIN Header_vofa_task */
/**
* @brief Function implementing the VOFA_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vofa_task */
__weak void vofa_task(void const * argument)
{
  /* USER CODE BEGIN vofa_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vofa_task */
}

/* USER CODE BEGIN Header_communicate_task */
/**
* @brief Function implementing the COMMUNICATE_TAS thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_communicate_task */
__weak void communicate_task(void const * argument)
{
  /* USER CODE BEGIN communicate_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END communicate_task */
}

/* USER CODE BEGIN Header_trig_task */
/**
* @brief Function implementing the TRIG_TASK thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_trig_task */
__weak void trig_task(void const * argument)
{
  /* USER CODE BEGIN trig_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END trig_task */
}

/* ShootTimer_Callback function */
void ShootTimer_Callback(void const * argument)
{
  /* USER CODE BEGIN ShootTimer_Callback */
	#ifndef CASCADE
	if(switch_is_up(trig_control.rc_data->rc_sl))
	{
		//xEventGroupSetBits(my_shootEventGroupHandle,ShootEvent_1);
		trig_control.trig_fire_mode = Serial_fire;
	}
	#endif
  /* USER CODE END ShootTimer_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
