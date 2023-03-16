#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GPIO.hpp"
//#include "hardware/spi.h"
#include <array>

#define nonBlockingDelay(delay) \
  const TickType_t currentTick = xTaskGetTickCount(); \
  while((xTaskGetTickCount() - currentTick) < (delay))

#define RED_PHASE 5000
#define RED_YELLOW_PHASE 4000
#define GREEN_PHASE 4000
#define GREEN_BLINK_PHASE 500
#define YELLOW_PHASE 4000
#define YELLOW_BLINK_PHASE 500

static pico_cpp::GPIO_Pin redPin(18,pico_cpp::PinType::Output);
static pico_cpp::GPIO_Pin greenPin(19,pico_cpp::PinType::Output);
static pico_cpp::GPIO_Pin bluePin(20,pico_cpp::PinType::Output);
TaskHandle_t xTaskMainHandle = NULL;
TaskHandle_t xTaskRedHandle = NULL;
TaskHandle_t xTaskRedYellowHandle = NULL;
TaskHandle_t xTaskGreenHandle = NULL;
TaskHandle_t xTaskGreenBlinkHandle = NULL;
TaskHandle_t xTaskYellowHandle = NULL;
TaskHandle_t xTaskYellowBlinkHandle = NULL;
TaskHandle_t xTaskNextHandle = NULL;

void red_on() {
    redPin.set_low();
}

void red_off() {
    redPin.set_high();
}

void green_on() {
    greenPin.set_low();
}

void green_off() {
    greenPin.set_high();
}

void yellow_on() {
    redPin.set_low();
    greenPin.set_low();
}

void yellow_off() {
    redPin.set_high();
    greenPin.set_high();
}

void vTaskCodeMain( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("Main\n");
    printf("priorityNext: %i priorityMaster: %i\r\n", int(uxTaskPriorityGet(xTaskNextHandle)), int(uxTaskPriorityGet(NULL)));
    vTaskPrioritySet( NULL, 4 );
    vTaskPrioritySet( xTaskNextHandle, 3 );
    vTaskPrioritySet( NULL, 2 );
  }
}

void vTaskCodeRed( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("red\n");
    red_on();

    nonBlockingDelay(RED_PHASE);

    red_off();
    xTaskNextHandle = xTaskRedYellowHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void vTaskCodeRedYellow( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("red-yellow\n");
    red_on();
    yellow_on();

    nonBlockingDelay(RED_YELLOW_PHASE);

    red_off();
    yellow_off();
    xTaskNextHandle = xTaskGreenHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void vTaskCodeGreen( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("green\n");
    green_on();

    nonBlockingDelay(GREEN_PHASE);

    green_off();
    xTaskNextHandle = xTaskGreenBlinkHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void vTaskCodeGreenBlink( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("green-blink\n");
    for (int i = 0; i < 4; i++) {
    {
            green_on();
            nonBlockingDelay(GREEN_BLINK_PHASE);
    }
    {
            green_off();
            nonBlockingDelay(GREEN_BLINK_PHASE);
    }
    }
    xTaskNextHandle = xTaskYellowHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void vTaskCodeYellow( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("yellow\n");
    yellow_on();

    nonBlockingDelay(YELLOW_PHASE);

    yellow_off();
    xTaskNextHandle = xTaskRedHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void vTaskCodeYellowBlink( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
     pvParameters value in the call to xTaskCreate() below.
     configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
     */
  for( ;; )
  {
    printf("yellow-blink\n");
    for (int i = 0; i < 4; i++) {
    {
            yellow_on();
            nonBlockingDelay(YELLOW_BLINK_PHASE);
    }
    {
            yellow_off();
            nonBlockingDelay(YELLOW_BLINK_PHASE);
    }
    }
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

int main() {
  stdio_init_all();
  redPin.set_high();
  greenPin.set_high();
  bluePin.set_high();
  xTaskCreate(
      vTaskCodeMain,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      3,            /* Priority at which the task is created. */
      &xTaskMainHandle );
  xTaskCreate(
      vTaskCodeRed,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskRedHandle );
  xTaskCreate(
      vTaskCodeRedYellow,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskRedYellowHandle );
  xTaskCreate(
      vTaskCodeGreen,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskGreenHandle );
  xTaskCreate(
      vTaskCodeGreenBlink,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskGreenBlinkHandle );
  xTaskCreate(
      vTaskCodeYellow,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskYellowHandle );
  xTaskCreate(
      vTaskCodeRedYellow,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 1, /* Parameter passed into the task. */
      1,            /* Priority at which the task is created. */
      &xTaskRedYellowHandle );
  xTaskNextHandle = xTaskRedHandle;
  vTaskStartScheduler();
  while(1)
  {
    configASSERT(0);    /* We should never get here */
  }

}
