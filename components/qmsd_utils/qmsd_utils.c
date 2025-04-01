#include <sys/time.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_mac.h"

#define TAG "qmsd_utils"
#define REF_TIME        1577808000 /* 2020-01-01 00:00:00 */

uint64_t qmsd_get_time_ms() {
    struct timeval tv = { 0 };
    gettimeofday(&tv, NULL);
    if (tv.tv_sec < REF_TIME) {
        return 0;
    }

    return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

uint32_t qmsd_get_time_sec() {
    struct timeval tv = { 0 };
    gettimeofday(&tv, NULL);
    if (tv.tv_sec < REF_TIME) {
        return 0;
    }

    return (uint32_t)tv.tv_sec;
}

void qmsd_time_set_timezone_cst_8() {
    setenv("TZ", "CST-8", 1);
    tzset();
}

struct tm* qmsd_time_get_tm() {
    time_t current_time;
    current_time = time(NULL);
    return localtime(&current_time);
}

void qmsd_time_set_time(time_t timestamp) {
    struct timeval tv;

    tv.tv_sec = timestamp;
    tv.tv_usec = 0;

    settimeofday(&tv, NULL);
}

char* qmsd_get_device_mac_id() {
    static char mac_id[13] = {0x0};
    if (mac_id[0]) {
        return mac_id;
    }

    uint8_t mac[6] = {0};
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(mac_id, "%02x%02x%02x%02x%02x%02x", MAC2STR(mac));
    return mac_id;
}

static void esp_mem_print_task() {
    TaskStatus_t *pxTaskStatusArray = NULL;
    volatile UBaseType_t uxArraySize = 0;
    uint32_t ulTotalRunTime = 0, ulStatsAsPercentage = 0, ulRunTimeCounte = 0;
    const char task_status_char[] = {'r', 'R', 'B', 'S', 'D'};

    /* Take a snapshot of the number of tasks in case it changes while this
    function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = malloc(uxTaskGetNumberOfTasks() * sizeof(TaskStatus_t));

    if (!pxTaskStatusArray) {
        return ;
    }

    /* Generate the (binary) data. */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
    ulTotalRunTime /= 100UL;

    ESP_LOGI(TAG, "---------------- The State Of Tasks ----------------");
    ESP_LOGI(TAG, "- HWM   : usage high water mark (Byte)");
    ESP_LOGI(TAG, "- Status: blocked ('B'), ready ('R'), deleted ('D') or suspended ('S')\n");
    ESP_LOGI(TAG, "TaskName\t\tStatus\tPrio\tHWM\tTaskNum\tCoreID\tRunTimeCounter\tPercentage");

    for (int i = 0; i < uxArraySize; i++) {
#if( configGENERATE_RUN_TIME_STATS == 1 )
        ulRunTimeCounte = pxTaskStatusArray[i].ulRunTimeCounter;
        ulStatsAsPercentage = ulRunTimeCounte / ulTotalRunTime;
#endif


        int core_id = -1;
        char precentage_char[4] = {0};

#if ( configTASKLIST_INCLUDE_COREID == 1 )
        core_id = (int) pxTaskStatusArray[ i ].xCoreID;
#endif

        /* Write the rest of the string. */
        ESP_LOGI(TAG, "%-16s\t%c\t%" PRIu32 "\t%" PRIu32 "\t%" PRIu32 "\t%hd\t%-16" PRIu32 "%-s%%",
                 pxTaskStatusArray[i].pcTaskName, task_status_char[pxTaskStatusArray[i].eCurrentState],
                 (uint32_t) pxTaskStatusArray[i].uxCurrentPriority,
                 (uint32_t) pxTaskStatusArray[i].usStackHighWaterMark,
                 (uint32_t) pxTaskStatusArray[i].xTaskNumber, core_id,
                 ulRunTimeCounte, (ulStatsAsPercentage <= 0) ? "<1" : itoa(ulStatsAsPercentage, precentage_char, 10));
    }

    free(pxTaskStatusArray);
}

void qmsd_debug_task_print(uint32_t interval_time) {
    static uint32_t time_last = 0;
    if (xTaskGetTickCount() - time_last > interval_time) {
        time_last = xTaskGetTickCount();
        esp_mem_print_task();
    }
}

void qmsd_debug_heap_print(uint32_t caps, uint32_t interval_time) {
    static uint32_t time_last = 0;
    if (xTaskGetTickCount() - time_last > interval_time) {
        time_last = xTaskGetTickCount();
        multi_heap_info_t info;
        heap_caps_get_info(&info, caps);
        ESP_LOGI(TAG, "------------- ಥ_ಥ Memory ಥ_ಥ ------------- ");
        ESP_LOGI(TAG, "ಥ_ಥ total_free_bytes -> %d", info.total_free_bytes);
        ESP_LOGI(TAG, "ಥ_ಥ total_allocated_bytes -> %d", info.total_allocated_bytes);
        ESP_LOGI(TAG, "ಥ_ಥ largest_free_block -> %d", info.largest_free_block);
        ESP_LOGI(TAG, "ಥ_ಥ minimum_free_bytes -> %d", info.minimum_free_bytes);
        ESP_LOGI(TAG, "ಥ_ಥ allocated_blocks -> %d", info.allocated_blocks);
        ESP_LOGI(TAG, "ಥ_ಥ free_blocks -> %d", info.free_blocks);
        ESP_LOGI(TAG, "ಥ_ಥ total_blocks -> %d", info.total_blocks);
        ESP_LOGI(TAG, "ಥ_ಥ total_mem -> %d", info.total_free_bytes + info.total_allocated_bytes);
    }
}

//static int __malloc_cont =0;

void* qmsd_malloc(size_t size) {
    return heap_caps_malloc_prefer(size, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

void qmsd_free(void* p)
{
    free(p);
}

esp_err_t qmsd_thread_create(TaskFunction_t main_func, const char* const name, const uint32_t stack_size, void* const arg,
                             UBaseType_t prio, TaskHandle_t* const p_handle, BaseType_t core_id, uint8_t stack_in_ext) {
#if CONFIG_SOC_CPU_CORES_NUM < 2
    core_id = 0;
#endif
#ifndef CONFIG_SPIRAM
    stack_in_ext = false;
#endif

    if (stack_in_ext) {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
        assert(false && "thread in psarm only support idf version >= 5.2.0 !");
#else
#if (configSUPPORT_STATIC_ALLOCATION == 1 && CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY == 1)
        if (xTaskCreatePinnedToCoreWithCaps(main_func, name, stack_size, arg, prio, p_handle, core_id, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM) != pdPASS) {
            ESP_LOGE(TAG, "Error creating task %s", name);
            goto qmsd_thread_create_error;
        } else {
            ESP_LOGI(TAG, "The %s task allocate stack on external memory", name);
        }
#else
        ESP_LOGE(TAG, "The %s task allocate stack on external memory failed, please enable CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY", name);
        return ESP_FAIL;
#endif
#endif
    } else {
        if (xTaskCreatePinnedToCore(main_func, name, stack_size, arg, prio, p_handle, core_id) != pdPASS) {
            ESP_LOGE(TAG, "Error creating task %s", name);
            goto qmsd_thread_create_error;
        } else {
            ESP_LOGI(TAG, "The %s task allocate stack on internal memory", name);
        }
    }
    return ESP_OK;

qmsd_thread_create_error:
    return ESP_FAIL;
}
