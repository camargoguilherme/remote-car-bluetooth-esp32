#include "board/board.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "app_config.h"

static adc_oneshot_unit_handle_t s_adc_handle;
static adc_cali_handle_t s_adc_cali_handle = NULL;

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) calibrated = true;
    }
#endif

    *out_handle = handle;
    return calibrated;
}

esp_err_t board_init(void)
{
    // ADC Init
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = BOARD_ADC_UNIT,
    };
    esp_err_t ret = adc_oneshot_new_unit(&init_config, &s_adc_handle);
    if (ret != ESP_OK) return ret;

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    ret = adc_oneshot_config_channel(s_adc_handle, BOARD_ADC_CHANNEL, &config);
    if (ret != ESP_OK) return ret;

    adc_calibration_init(BOARD_ADC_UNIT, BOARD_ADC_CHANNEL, ADC_ATTEN_DB_12, &s_adc_cali_handle);
    return ESP_OK;
}

float board_get_battery_voltage(void)
{
    int adc_raw, voltage_mv;
    ESP_ERROR_CHECK(adc_oneshot_read(s_adc_handle, BOARD_ADC_CHANNEL, &adc_raw));
    
    if (s_adc_cali_handle) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(s_adc_cali_handle, adc_raw, &voltage_mv));
    } else {
        voltage_mv = (adc_raw * 3300) / 4095; // Fallback simples
    }

    return (voltage_mv / 1000.0f) * BOARD_BATTERY_VOLTAGE_DIVIDER;
}
