#include <pd_api.h>
#include <pdcpp/pdnewlib.h>

#include "AudioState.h"
#include "KlangwellenPlaydateApplication.h"

// from https://github.com/nstbayless/playdate-cpp

extern KlangwellenPlaydateApplication *get_instance(PlaydateAPI *pd);

KlangwellenPlaydateApplication *fApp;

int update(void *userdata) {
    if (fApp != nullptr) {
        fApp->update();
        return 1;
    }
    return 0;
}

static int update_audio(void *context, int16_t *left, int16_t *right, int len) {
    // Do not use the audio callback for system tasks:
    //   spend as little time here as possible
    if (fApp != nullptr) {
        auto *state = (AudioState *) context;
        return fApp->audioblock(state, left, right, len);
    }
    return 0;
}

/* wrap `eventHandler` in `extern "C" { ... }` block */
#ifdef __cplusplus
extern "C" {
#endif

void setup_audio(PlaydateAPI *pd) {
    // create a new audio source with a state context
    auto *state = (AudioState *) pd->system->realloc(nullptr, sizeof(AudioState));
    state->pd     = pd;
    state->source = pd->sound->addSource(&update_audio, state, 1);
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) {
    /* This is required, otherwise linker errors abound */
    eventHandler_pdnewlib(pd, event, arg);

    if (event == kEventInit) {
        pd->system->logToConsole("setting up ...");
        pd->display->setRefreshRate(20);
        if (fApp == nullptr) {
            fApp = get_instance(pd);
            fApp->setup();
            /* setup callback, turn off lua */
            pd->system->setUpdateCallback(update, pd);
            setup_audio(pd);
        }
    }

    if (fApp != nullptr) {
        if (event == kEventKeyPressed) {
            fApp->keyPressed(arg);
        }

        if (event == kEventKeyReleased) {
            fApp->keyReleased(arg);
        }

        if (event == kEventTerminate) {
            pd->system->logToConsole("shutting down ...");
            fApp->finish();
            fApp = nullptr;
        }
    } else {
        pd->system->logToConsole("application is not intialized");
        return 1;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
