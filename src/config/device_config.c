#include "device_config.h"

device_ctx create_device_context(device_t* device, cart_header* header) {
    device_ctx ctx;

    if (header->cgb_flag) {
        ctx.console_ver = CGB;
    } else ctx.console_ver = DMG;
    ctx.cart_meta = header->type;

    return ctx;
}