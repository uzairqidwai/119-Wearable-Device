#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_NUM         (1)   // i2s port number
#define WAVE_FREQ_HZ    (100) // sound frequency
#define SAMPLE_RATE     (36000)
#define I2S_BCK_PIN     (43)
#define I2S_WS_PIN      (44)
#define I2S_DATA_PIN    (21)

void setup() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCK_PIN,
    .ws_io_num = I2S_WS_PIN,
    .data_out_num = I2S_DATA_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install((i2s_port_t)I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)I2S_NUM, &pin_config);
  i2s_set_clk((i2s_port_t)I2S_NUM, SAMPLE_RATE, (i2s_bits_per_sample_t)I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

}

void loop() {
  int16_t sample;
  float t = 0;
  int16_t amplitude = 10000; // adjust this value to change the volume
  while (1) {
    for (int i = 0; i < SAMPLE_RATE / WAVE_FREQ_HZ; i++) {
      sample = (int16_t)(amplitude * sin(t));
      i2s_write_sample(sample);
      t += TWO_PI * WAVE_FREQ_HZ / SAMPLE_RATE;
    }
  }
}

void i2s_write_sample(int16_t sample) {
  size_t bytes_written;
  i2s_write((i2s_port_t)I2S_NUM, &sample, sizeof(sample), &bytes_written, portMAX_DELAY);
}
