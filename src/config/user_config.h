#ifndef CONFIG_STATE_H_
#define CONFIG_STATE_H_

typedef struct config_t {
    int display_scale;
} config_t;

config_t* create_config(int, char**);

#endif