#include <inttypes.h>
#include "esp_system.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_idf_version.h"

#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "qmsd_button.h"

#define TAG "BTN_ADC"

#define ADC_BUTTON_WIDTH ADC_WIDTH_MAX - 1
#define ADC_BUTTON_ATTEN ADC_ATTEN_DB_6 + 1 // ADC_ATTEN_DB_12 or ADC_ATTEN_DB_11 (old sdk version)
#define ADC_BUTTON_ADC_UNIT ADC_UNIT_1
#define ADC_DEFAULT_VREF 1100
#define ADC_NUM_OF_SAMPLES 1

typedef struct {
    adc_cali_handle_t adc1_cali_handle;
    adc_oneshot_unit_handle_t adc1_handle;
    uint8_t is_configured;
    struct {
        uint8_t is_init;
        uint16_t volt_mv;
    } channel[ADC1_CHANNEL_MAX];
} adc_data_t;

typedef struct {
    uint8_t channel;
    uint16_t max_volt_mv;
    uint16_t min_volt_mv;
} btn_hw_info_t;

static adc_data_t g_btn = {0};

static esp_err_t adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t* out_handle) {
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BUTTON_WIDTH,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BUTTON_WIDTH,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated ? ESP_OK : ESP_FAIL;
}

uint16_t get_volt_mv(adc1_channel_t channel) {
    uint32_t adc_reading = 0;
    int adc_raw = 0;

    for (int i = 0; i < ADC_NUM_OF_SAMPLES; i++) {
        adc_oneshot_read(g_btn.adc1_handle, channel, &adc_raw);
        adc_reading += adc_raw;
    }
    adc_reading /= ADC_NUM_OF_SAMPLES;

    //Convert adc_reading to voltage in mV
    int voltage;
    adc_cali_raw_to_voltage(g_btn.adc1_cali_handle, adc_reading, &voltage);
    return voltage;
}

uint8_t qmsd_button_adc_read(uint8_t scan_start, uint8_t wait_press, void* hardware_data) {
    if (scan_start) {
        for (uint8_t i = 0; i < ADC1_CHANNEL_MAX; i++) {
            if (g_btn.channel[i].is_init) {
                g_btn.channel[i].volt_mv = get_volt_mv(i);
            }
        }
    }
    btn_hw_info_t* hw_info = hardware_data;
    return g_btn.channel[hw_info->channel].volt_mv > hw_info->max_volt_mv || g_btn.channel[hw_info->channel].volt_mv < hw_info->min_volt_mv;
}

// ESP32S3 only support ADC1: 10 channels: GPIO1 - GPIO10
btn_handle_t qmsd_button_create_adc(adc1_channel_t adc_channel, uint16_t middle_volt_mv, uint16_t diff_volt_mv, void* user_data) {
    if (g_btn.is_configured == 0) {
        //ADC1 Config
        adc_oneshot_chan_cfg_t oneshot_config = {
            .bitwidth = ADC_BUTTON_WIDTH,
            .atten = ADC_BUTTON_ATTEN,
        };
        esp_err_t ret = adc_oneshot_config_channel(g_btn.adc1_handle, adc_channel, &oneshot_config);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "adc oneshot config channel fail!");
            return NULL;
        }
        ret = adc_calibration_init(ADC_BUTTON_ADC_UNIT, ADC_BUTTON_ATTEN, &g_btn.adc1_cali_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "ADC1 Calibration Init False");
            return NULL;
        }
        g_btn.is_configured = 1;
    }

    if (g_btn.channel[adc_channel].is_init == 0) {
        adc1_config_channel_atten(adc_channel, ADC_BUTTON_ATTEN);
        g_btn.channel[adc_channel].is_init = 1;
    }
    btn_hw_info_t* hw_info = (btn_hw_info_t*)malloc(sizeof(btn_hw_info_t));
    hw_info->channel = adc_channel;
    hw_info->max_volt_mv = middle_volt_mv + diff_volt_mv;
    if (diff_volt_mv > middle_volt_mv) {
        hw_info->min_volt_mv = 0;
    } else {
        hw_info->min_volt_mv = middle_volt_mv - diff_volt_mv;
    }

    return qmsd_button_create(qmsd_button_adc_read, (void*)hw_info, 0, user_data);
}
