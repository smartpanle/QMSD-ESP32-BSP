#pragma once

#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_idf_version.h"
#include "esp_log.h"
#include "qmsd_err.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0))
#define gpio_pad_select_gpio esp_rom_gpio_pad_select_gpio
#define portTICK_RATE_MS portTICK_PERIOD_MS
#endif

#define QMSD_ERROR_CHECK(con, err, format, ...) do { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, "<%s> " format, "QMSD_FAILED", ##__VA_ARGS__); \
            return err; \
        } \
    } while(0)

#define QMSD_ERROR_ASSERT(err) do { \
        qmsd_err_t __err_rc = (err); \
        if (__err_rc != QMSD_ERR_OK) { \
            ESP_LOGW(TAG, " QMSD_ERROR_ASSERT failed, at 0x%08x, expression: %s", \
                     (intptr_t)__builtin_return_address(0) - 3, __ASSERT_FUNC); \
            assert(0 && #err); \
        } \
    } while(0)

#define QMSD_ERROR_GOTO(con, lable, format, ...) do { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, format, ##__VA_ARGS__); \
            goto lable; \
        } \
    } while(0)

#define QMSD_ERROR_CONTINUE(con, format, ...) { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, format, ##__VA_ARGS__); \
            continue; \
        } \
    }

#define QMSD_ERROR_BREAK(con, format, ...) { \
        if (con) { \
            if(*format != '\0') \
                ESP_LOGW(TAG, format, ##__VA_ARGS__); \
            break; \
        } \
    }

#define QMSD_PARAM_CHECK(con) do { \
        if (!(con)) { \
            ESP_LOGE(TAG, "<QMSD_ERR_INVALID_ARG> !(%s)", #con); \
            return QMSD_ERR_INVALID_ARG; \
        } \
    } while(0)

#define QMSD_MALLOC(x)          heap_caps_malloc((x), MALLOC_CAP_INTERNAL | MALLOC_CAP_DEFAULT)
#define QMSD_MALLOC_PSRAM(x)    heap_caps_malloc((x), MALLOC_CAP_SPIRAM | MALLOC_CAP_DEFAULT)
#define QMSD_FREE(x)            free((x))

#ifndef RGB888TO565
#define RGB888TO565(r, g, b)    ((((r) & 0b11111000) << 8) | (((g) & 0b11111100) << 3) | ((b) >> 3))
#endif

#define EMBED_FILES_DECLARE(file_name) \
extern uint8_t _binary_##file_name##_start; \
extern uint8_t _binary_##file_name##_end; \
const uint8_t* _##file_name##_ptr = &_binary_##file_name##_start; \
uint32_t _##file_name##_size = (uint32_t)&_binary_##file_name##_end - (uint32_t)&_binary_##file_name##_start

// time -> base <sys/time.h>
uint64_t qmsd_get_time_ms();

uint32_t qmsd_get_time_sec();

void qmsd_time_set_timezone_cst_8();

struct tm* qmsd_time_get_tm();

void qmsd_time_set_time(time_t timestamp);

void qmsd_debug_task_print(uint32_t interval_time);

void qmsd_debug_heap_print(uint32_t caps, uint32_t interval_time);

uint16_t crc16tablefast(uint8_t *data, uint32_t len);

uint16_t crc16tablefast_muti(uint8_t *data, uint32_t len, uint8_t *data1, uint32_t len1);

void* qmsd_malloc(size_t size);

void qmsd_free(void* p);

// Note: If stack_in_ext is true, deleting tasks will cause memory leaks, so it can only be used for threads that never end !!!!
esp_err_t qmsd_thread_create(TaskFunction_t main_func, const char* const name, const uint32_t stack, void* const arg,
                             UBaseType_t prio, TaskHandle_t* const p_handle, const BaseType_t core_id, uint8_t stack_in_ext);

#ifdef __cplusplus
}
#endif
