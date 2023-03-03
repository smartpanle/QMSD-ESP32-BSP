#include <stdio.h>
#include "string.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "qmsd_button.h"
#include "qmsd_vmm.h"
#include "qmsd_utils.h"

#define TAG "QMSD-VMM"

#define TEST_SET_GET(key, type, value) do {\
    qmsd_vmm_set_##type(key, value); \
    data.type = qmsd_vmm_get_##type##_value(key, 0); \
    printf(""#type" -> key: "#key", write: "#value", read: %d \r\n", data.type); \
} while(0)

#define TEST_GET(key, type) do {\
    data.type = qmsd_vmm_get_##type##_value(key, 0); \
    printf(""#type" -> key: "#key", read: %d \r\n", data.type); \
} while(0)

void app_main(void)
{   
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    printf("----START----\r\n");
    qmsd_debug_heap_print(MALLOC_CAP_INTERNAL, 0);
    printf("----_----\r\n");

    qmsd_vmm_init(0);
    storage_data_t data;
    TEST_SET_GET(80, i8, -80);
    TEST_SET_GET(160, i16, -160);
    TEST_SET_GET(320, i32, -320);

    TEST_SET_GET(8, u8, 8);
    TEST_SET_GET(16, u16, 16);
    TEST_SET_GET(120, u32, 32);

    qmsd_vmm_set_float(100, 100.100);
    data.f32 = qmsd_vmm_get_float_value(100, 0.0);
    printf("f32 -> key: 100, write: 100.100, read: %.02f \r\n", data.f32);

    char* test_string = "Hi, QMSD VMM";
    qmsd_vmm_set_str(120, test_string);

    uint32_t size = 100;
    char str_read[size];
    qmsd_vmm_get_str(120, str_read, &size);
    printf("str -> key: 120, write: %s, read: %s \r\n", test_string, str_read);

    qmsd_vmm_set_blob(121, test_string, strlen(test_string) + 1);
    str_read[0] = '0';
    qmsd_vmm_get_blob(121, str_read, &size);
    printf("blob -> key: 121, write: %s, read: %s, len: %d \r\n", test_string, str_read, size);

    int32_t keys[] = {
        80, 160, 320, 8, 16, 32, 100, 120, 121, QMSD_VMM_KEY_END
    };
    printf("- Sync to nvs -\r\n");
    qmsd_vmm_sync_to_nvs_multi(keys);

    printf("- Delete to nvs -\r\n");
    qmsd_vmm_delete_all();
    
    printf("- Sync from nvs -\r\n");
    qmsd_vmm_sync_from_nvs_multi(keys);

    TEST_GET(80, i8);
    TEST_GET(160, i16);
    TEST_GET(320, i32);
    TEST_GET(8, u8);
    TEST_GET(16, u16);
    TEST_GET(32, u32);

    // float not support sync from nvs
    data.f32 = qmsd_vmm_get_float_value(100, 0.0);
    printf("f32 -> key: 100, read: %.02f \r\n", data.f32);

    str_read[0] = '\0';
    qmsd_vmm_get_str(120, str_read, &size);
    printf("str -> key: 120, read: %s \r\n", str_read);

    str_read[0] = '\0';
    qmsd_vmm_get_blob(121, str_read, &size);
    printf("blob -> key: 121, read: %s, len: %d \r\n", str_read, size);

    qmsd_vmm_delete_all();
    printf("----END----\r\n");
    qmsd_debug_heap_print(MALLOC_CAP_INTERNAL, 0);
    printf("----_----\r\n");
}
