#ifndef CONFIG_STATE_H_
#define CONFIG_STATE_H_

#include "../models/device.h"

typedef struct config_t {
    unsigned int display_scale;
    unsigned int frequency_override;
    device_variant variant;
    char* cart_path;
} config_t;

config_t* create_config(int, char**);

#endif