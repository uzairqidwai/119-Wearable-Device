#include <driver/i2s.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define PIN_I2S_BCLK 3
#define PIN_I2S_LRC 10
#define PIN_I2S_DIN 11
#define PI 3.14159265

void setup() {
  i2s_config_t i2s_config = {
    mode: (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    sample_rate: SAMPLE_RATE,
    bits_per_sample: I2S_BITS_PER_SAMPLE_16BIT,
    channel_format: I2S_CHANNEL_FMT_RIGHT_LEFT,
    communication_format: I2S_COMM_FORMAT_I2S,
    intr_alloc_flags: ESP_INTR_FLAG_LEVEL1,
    dma_buf_count: 8,
    dma_buf_len: 64,
    use_apll: false
  };

  i2s_pin_config_t pin_config = {
    bck_io_num: PIN_I2S_BCLK,
    ws_io_num: PIN_I2S_LRC,
    data_out_num: PIN_I2S_DIN,
    data_in_num: I2S_PIN_NO_CHANGE
  };

  i2s_driver_install((i2s_port_t)I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)I2S_NUM_0, &pin_config);
}

void loop() {
  int low_frequency = 440; // Hz
  int high_frequency = 880; // Hz
  int frequency = low_frequency;
  int samples_per_wave = SAMPLE_RATE / frequency;
  int16_t sample;
  size_t bytes_written;
  float radian_increment = 2.0 * PI / samples_per_wave;
  float volume = 1.0; // 100%

  for (int i = 0; i < SAMPLE_RATE; i++) {
    float radian = radian_increment * i;
    sample = (int16_t)(sin(radian) * 0x7FFF * volume);

    i2s_write(I2S_NUM_0, &sample, 2, &bytes_written, portMAX_DELAY);
    i2s_write(I2S_NUM_0, &sample, 2, &bytes_written, portMAX_DELAY);
    
    // change frequency to create wailing siren sound
    if (i % (SAMPLE_RATE / 2) == 0) {
      frequency = (frequency == low_frequency) ? high_frequency : low_frequency;
      samples_per_wave = SAMPLE_RATE / frequency;
      radian_increment = 2.0 * PI / samples_per_wave;
    }
  }
}
