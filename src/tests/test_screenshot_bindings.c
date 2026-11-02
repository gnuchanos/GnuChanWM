#include "../wm_config.h"

#include <assert.h>
#include <string.h>
#include <X11/X.h>

int main(int argc, char **argv) {
    assert(argc == 2);

    WmConfig config;
    wm_config_defaults(&config);
    assert(wm_config_load(&config, argv[1]) == 0);

    int found_alt_p = 0;
    int found_print = 0;
    for (int i = 0; i < config.binding_count; i++) {
        const WmBinding *binding = &config.bindings[i];
        if (binding->modifiers == Mod1Mask &&
            strcmp(binding->key, "p") == 0) {
            assert(strcmp(binding->command,
                          "GnuChanScreenShoot --once") == 0);
            found_alt_p = 1;
        } else if (binding->modifiers == 0 &&
                   strcmp(binding->key, "Print") == 0) {
            assert(strcmp(binding->command,
                          "GnuChanScreenShoot --settings") == 0);
            found_print = 1;
        }
    }

    assert(found_alt_p);
    assert(found_print);
    return 0;
}
