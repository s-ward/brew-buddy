idf_component_register(SRCS "esp_rest_main.c"
                            "rest_server.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "fatfs" "spiffs" "nvs_flash" "mdns" "protocol_examples_common" "esp_http_server" "json" "wps_led" "nvsblob" "ds18b20")


if(CONFIG_EXAMPLE_WEB_DEPLOY_SF)
    set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../front")
    if(EXISTS ${WEB_SRC_DIR}/dist)
        spiffs_create_partition_image(www ${WEB_SRC_DIR}/dist FLASH_IN_PROJECT)
    else()
        message(FATAL_ERROR "${WEB_SRC_DIR}/dist doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
    endif()
endif()