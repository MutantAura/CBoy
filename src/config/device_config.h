#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_

#include "../loader/cart.h"
#include "../models/device.h"

typedef enum console_variant {
    DMG,
    CGB
} console_variant;

typedef struct device_ctx {
    cart_type cart_meta;
    console_variant console_ver;
} device_ctx;

device_ctx create_device_context(device_t*, cart_header*);

#endif