#include <stdlib.h>
#include <string.h>

#include "user_config.h"

config_t default_config = {
    .display_scale = 5,
    .frequency_override = 0,
    .variant = DMG,
    .cart_path = NULL
};

config_t* create_config(const int argc, char** args) {
    config_t* config_ptr = malloc(sizeof(config_t));
    *config_ptr = default_config;
    config_ptr->cart_path = args[1];

    // User has provided additional CLI args beyond program + ROM dest.
    // Edit default values in this case.
    if (argc > 3) {
        for (int i = 2; i < argc; i++) {
            int arg_chars = strlen(args[i]);

            int current_char = 0;
            while (args[i][current_char] != '\0' && current_char < arg_chars) {
                if (args[i][current_char] == '-') {
                    switch (args[i][current_char + 1]) {
                        // TODO: handle arg codes?
                    }
                }
            }
        }
    }

    return config_ptr;
}