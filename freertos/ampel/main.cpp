#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GPIO.hpp"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "pico/binary_info.h"
#include <array>

#define nonBlockingDelay(delay) \
  const TickType_t currentTick = xTaskGetTickCount(); \
  while((xTaskGetTickCount() - currentTick) < (delay))

#define mosi_pin 16
#define miso_pin 19
#define sck_pin 18
#define cs_pin 17

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
static uint8_t txbuf[1];
static uint8_t rxbuf[1];
const uint dma_tx = dma_claim_unused_channel(true);
const uint dma_rx = dma_claim_unused_channel(true);

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
    vTaskPrioritySet( xTaskNextHandle, 3 );
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

    xTaskNextHandle = xTaskYellowBlinkHandle;
    vTaskPrioritySet( NULL, 1 );
//    printf("priorityOn: %i priorityOff: %i\r\n", int(uxTaskPriorityGet(xTaskOnHandle)), int(uxTaskPriorityGet(xTaskOffHandle)));
  }
}

void writeSPI(uint8_t message) {
  txbuf[0] = message;
  printf("Starting DMAs...\n");
  // start them exactly simultaneously to avoid races (in extreme cases the FIFO could overflow)
  dma_start_channel_mask((1u << dma_tx) | (1u << dma_rx));
  printf("Wait for RX complete...\n");
  dma_channel_wait_for_finish_blocking(dma_rx);
  if (dma_channel_is_busy(dma_tx)) {
      panic("RX completed before TX");
  }
}

int main() {
  stdio_init_all();
  redPin.set_high();
  greenPin.set_high();
  bluePin.set_high();

  printf("SPI DMA example\n");
  // Enable SPI at 1 MHz and connect to GPIOs
  spi_init(spi_default, 1000 * 1000);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  // Make the SPI pins available to picotool
  bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));
  // Make the CS pin available to picotool
  bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
  // Grab some unused dma channels
  const uint dma_tx = dma_claim_unused_channel(true);
  const uint dma_rx = dma_claim_unused_channel(true);
  // Force loopback for testing (I don't have an SPI device handy)
  hw_set_bits(&spi_get_hw(spi_default)->cr1, SPI_SSPCR1_LBM_BITS);

  for (uint i = 0; i < 1; ++i) {
      txbuf[i] = rand();
  }

  printf("Configure TX DMA\n");
  dma_channel_config c = dma_channel_get_default_config(dma_tx);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
  channel_config_set_dreq(&c, spi_get_dreq(spi_default, true));
  dma_channel_configure(dma_tx, &c,
                        &spi_get_hw(spi_default)->dr, // write address
                        txbuf, // read address
                        1, // element count (each element is of size transfer_data_size)
                        false); // don't start yet

  printf("Configure RX DMA\n");
  // We set the inbound DMA to transfer from the SPI receive FIFO to a memory buffer paced by the SPI RX FIFO DREQ
  // We configure the read address to remain unchanged for each element, but the write
  // address to increment (so data is written throughout the buffer)
  c = dma_channel_get_default_config(dma_rx);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
  channel_config_set_dreq(&c, spi_get_dreq(spi_default, false));
  channel_config_set_read_increment(&c, false);
  channel_config_set_write_increment(&c, true);
  dma_channel_configure(dma_rx, &c,
                        rxbuf, // write address
                        &spi_get_hw(spi_default)->dr, // read address
                        1, // element count (each element is of size transfer_data_size)
                        false); // don't start yet

  xTaskCreate(
      vTaskCodeMain,  /* Function that implements the task. */
      "red",     /* Text name for the task. */
      1024,          /* Stack size in words, not bytes. */
      ( void * ) 2, /* Parameter passed into the task. */
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
//  xTaskNextHandle = xTaskRedHandle;
//  vTaskStartScheduler();
  printf("5");
  {nonBlockingDelay(1000);}
  printf("4");
  {nonBlockingDelay(1000);}
  printf("3");
  {nonBlockingDelay(1000);}
  printf("2");
  {nonBlockingDelay(1000);}
  printf("1");
  {nonBlockingDelay(1000);}
  writeSPI(0x00);
  printf("Done. Checking...");
  for (uint i = 0; i < 1; ++i) {
    printf(" %02x", rxbuf[i]);
    if (rxbuf[i] != txbuf[i]) {
      panic("Mismatch at %d/%d: expected %02x, got %02x",
        i, 1, txbuf[i], rxbuf[i]
      );
    }
  }
  while(1)
  {
    configASSERT(0);    /* We should never get here */
  }
}
