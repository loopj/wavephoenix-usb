#include <stdio.h>
#include <string.h>

#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "tusb.h"

#define UART_ID         uart0
#define UART_TX_PIN     PICO_DEFAULT_UART_TX_PIN
#define UART_RX_PIN     PICO_DEFAULT_UART_RX_PIN
#define BAUD_RATE       115200

#define POLL_CMD        0x01
#define POLL_REPLY_LEN  8

#define POLL_DELAY_MS   4

static bool awaiting_reply = false;
static uint8_t hid_report[8];

static uint8_t rx_buf[32];
static size_t rx_count = 0;

static absolute_time_t next_poll_time;

void poll_input(void)
{
  // Check for incoming data on the UART
  if (awaiting_reply) {
    while (uart_is_readable(UART_ID) && rx_count < sizeof(rx_buf)) {
      rx_buf[rx_count++] = uart_getc(UART_ID);
    }

    if (rx_count == POLL_REPLY_LEN) {
      // Copy buttons directly to HID report
      hid_report[0] = rx_buf[0];
      hid_report[1] = rx_buf[1];

      // Copy sticks, inverting the Y-axis
      hid_report[2] = rx_buf[2];
      hid_report[3] = 255 - rx_buf[3];
      hid_report[4] = rx_buf[4];
      hid_report[5] = 255 - rx_buf[5];

      // Copy triggers
      hid_report[6] = rx_buf[6];
      hid_report[7] = rx_buf[7];

      if (tud_hid_ready())
        tud_hid_report(0, hid_report, sizeof(hid_report));

      // After processing, return to idle
      awaiting_reply = false;
      rx_count       = 0;
    }
  }

  // Check if it's time to send the next poll command
  if (absolute_time_diff_us(get_absolute_time(), next_poll_time) <= 0) {
    // Drain the UART receive buffer
    while (uart_is_readable(UART_ID))
      uart_getc(UART_ID);

    // Send poll command
    uint8_t command = POLL_CMD;
    uart_write_blocking(UART_ID, &command, 1);

    // Await a response
    rx_count       = 0;
    awaiting_reply = true;

    // Schedule next poll
    next_poll_time = delayed_by_ms(next_poll_time, POLL_DELAY_MS);
  }
}

int main(void)
{
  // Initialize UART for communication with the WavePhoenix SoC
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
  uart_set_fifo_enabled(UART_ID, false);

  // Initialize TinyUSB
  tusb_init();

  // Initialize stdio over USB
  stdio_usb_init();

  // Initialize input polling
  next_poll_time = get_absolute_time();

  while (true) {
    // Handle USB events
    tud_task();

    // Poll for input state from the WavePhoenix SoC
    poll_input();
  }
}
