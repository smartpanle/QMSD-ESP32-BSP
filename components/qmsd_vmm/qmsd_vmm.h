#pragma once

#include "stdint.h"
#include "qmsd_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QMSD_VMM_NVS_SPACE_NAME "QMSD_VMM_NVS"
#define QMSD_VMM_KEY_END (-2)
#define QMSD_VMM_KEY_SYNC_END (qmsd_vmm_key_info_t) {(QMSD_VMM_KEY_END), QMSD_TYPE_ANY}

typedef enum {
    QMSD_TYPE_U8,
    QMSD_TYPE_U16,
    QMSD_TYPE_U32,
    QMSD_TYPE_I8,
    QMSD_TYPE_I16,
    QMSD_TYPE_I32,
    QMSD_TYPE_FLOAT,
    QMSD_TYPE_STR,
    QMSD_TYPE_BLOB,
    QMSD_TYPE_ANY,
} qmsd_vmm_item_type_t;

typedef union {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    float f32;
    void* point;
    const void* const_point;
} storage_data_t;

typedef qmsd_err_t (*qmsd_vmm_cb_t)(int32_t key, void* data, uint32_t length, uint32_t timeout_ticks);

qmsd_err_t qmsd_vmm_init(uint8_t mem_in_psram);

qmsd_err_t qmsd_vmm_delete(int32_t key);

qmsd_err_t qmsd_vmm_delete_all();

qmsd_err_t qmsd_vmm_set_update_callback(qmsd_vmm_cb_t cb);

qmsd_err_t qmsd_vmm_set_value(int32_t key, qmsd_vmm_item_type_t type, storage_data_t value, uint32_t length);

qmsd_err_t qmsd_vmm_set_i8(int32_t key, int8_t value);

qmsd_err_t qmsd_vmm_set_i16(int32_t key, int16_t value);

qmsd_err_t qmsd_vmm_set_i32(int32_t key, int32_t value);

qmsd_err_t qmsd_vmm_set_u8(int32_t key, uint8_t value);

qmsd_err_t qmsd_vmm_set_u16(int32_t key, uint16_t value);

qmsd_err_t qmsd_vmm_set_u32(int32_t key, uint32_t value);

qmsd_err_t qmsd_vmm_set_float(int32_t key, float value);

qmsd_err_t qmsd_vmm_set_blob(int32_t key, const void* value, uint32_t length);

qmsd_err_t qmsd_vmm_set_str(int32_t key, const char* value);

qmsd_err_t qmsd_vmm_set_str_format(int32_t key, const char *fmt, ...);

qmsd_err_t qmsd_vmm_get_value(int32_t key, qmsd_vmm_item_type_t type, storage_data_t* out_value, uint32_t* out_length);

qmsd_err_t qmsd_vmm_get_i8(int32_t key, int8_t* out_value);

qmsd_err_t qmsd_vmm_get_i16(int32_t key, int16_t* out_value);

qmsd_err_t qmsd_vmm_get_i32(int32_t key, int32_t* out_value);

qmsd_err_t qmsd_vmm_get_u8(int32_t key, uint8_t* out_value);

qmsd_err_t qmsd_vmm_get_u16(int32_t key, uint16_t* out_value);

qmsd_err_t qmsd_vmm_get_u32(int32_t key, uint32_t* out_value);

qmsd_err_t qmsd_vmm_get_float(int32_t key, float* out_value);

int8_t qmsd_vmm_get_i8_value(int32_t key, int8_t default_value);

int16_t qmsd_vmm_get_i16_value(int32_t key, int16_t default_value);

int32_t qmsd_vmm_get_i32_value(int32_t key, int32_t default_value);

uint8_t qmsd_vmm_get_u8_value(int32_t key, uint8_t default_value);

uint16_t qmsd_vmm_get_u16_value(int32_t key, uint16_t default_value);

uint32_t qmsd_vmm_get_u32_value(int32_t key, uint32_t default_value);

float qmsd_vmm_get_float_value(int32_t key, float default_value);

qmsd_err_t qmsd_vmm_get_blob(int32_t key, void* out_value, uint32_t* length);

qmsd_err_t qmsd_vmm_get_str(int32_t key, char* out_value, uint32_t* length);

// Not recommended for human use, not thread safe !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char* qmsd_vmm_get_string(int32_t key, const char* default_value);

// Not recommended for human use, not thread safe !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void qmsd_vmm_free_all_string();

qmsd_err_t qmsd_vmm_sync_to_nvs_multi(const int32_t* keys);

qmsd_err_t qmsd_vmm_sync_to_nvs(uint32_t key);

qmsd_err_t qmsd_vmm_sync_from_nvs_multi(const int32_t* keys);

qmsd_err_t qmsd_vmm_sync_from_nvs(int32_t key);

#ifdef __cplusplus
}
#endif
