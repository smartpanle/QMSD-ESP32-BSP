#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "mp3_player.h"
#include "qmsd_utils.h"

#define BOARD_I2S_NUM I2S_NUM_0
#define I2S_SCLK_PIN 36
#define I2S_LRCK_PIN 35
#define I2S_DOUT_PIN 37

#define I2S_CONFIG_DEFAULT() { \
    .mode                   = I2S_MODE_MASTER | I2S_MODE_TX, \
    .sample_rate            = 16000, \
    .bits_per_sample        = I2S_BITS_PER_SAMPLE_16BIT, \
    .channel_format         = I2S_CHANNEL_FMT_RIGHT_LEFT, \
    .communication_format   = I2S_COMM_FORMAT_STAND_I2S, \
    .intr_alloc_flags       = ESP_INTR_FLAG_LEVEL1, \
    .dma_buf_count          = 2, \
    .dma_buf_len            = 160, \
    .use_apll               = false, \
    .tx_desc_auto_clear     = true, \
    .fixed_mclk             = 0, \
    .bits_per_chan          = I2S_BITS_PER_CHAN_16BIT, \
}

esp_err_t board_i2s_init() {
    esp_err_t ret_val = ESP_OK;
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCLK_PIN,
        .ws_io_num = I2S_LRCK_PIN,
        .data_out_num = I2S_DOUT_PIN,
        .data_in_num = -1,
        .mck_io_num = -1,
    };

    i2s_config_t i2s_config = I2S_CONFIG_DEFAULT();
    i2s_config.sample_rate = 44100;
    i2s_config.use_apll = false;
    ret_val |= i2s_driver_install(BOARD_I2S_NUM, &i2s_config, 0, NULL);
    ret_val |= i2s_set_pin(BOARD_I2S_NUM, &pin_config);
    return ret_val;
}

void mp3plyaer_info_update(int samprate, int chans, int bits_per_sample) {
    i2s_set_clk(I2S_NUM_0, samprate, bits_per_sample, chans == 1 ? I2S_CHANNEL_MONO : I2S_CHANNEL_STEREO);
}

void audio_task(void* arg) {
    board_i2s_init();
    EMBED_FILES_DECLARE(test_mp3);
    mp3player_init(mp3plyaer_info_update, 0, 2);
    mp3player_start(_test_mp3_ptr, _test_mp3_size);
    uint8_t buffer[512];
    uint32_t bytes_write;
    for (;;) {
        uint32_t read_length = mp3player_read_data(buffer, 512, portMAX_DELAY);
        i2s_write(BOARD_I2S_NUM, buffer, read_length, &bytes_write, portMAX_DELAY);
    }
    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreatePinnedToCore(audio_task, "audio", 4 * 1024, NULL, 5, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(2000));
    mp3player_pause();
    vTaskDelay(pdMS_TO_TICKS(2000));
    mp3player_resume();
}
