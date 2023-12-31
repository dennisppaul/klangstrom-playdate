#include <pd_api.h>
#include <pdcpp/pdnewlib.h>

#include "AudioState.h"
#include "KlangwellenPlaydateApplication.h"

// from https://github.com/nstbayless/playdate-cpp

extern KlangwellenPlaydateApplication *get_instance(PlaydateAPI *pd);

KlangwellenPlaydateApplication *fApp;

int update(void *userdata) {
    fApp->update();
    return 1;
}

static int update_audio(void *context, int16_t *left, int16_t *right, int len) {
    // Do not use the audio callback for system tasks:
    //   spend as little time here as possible
    auto *state = (AudioState *) context;
    return fApp->audioblock(state, left, right, len);
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
//        pd->system->logToConsole("init");
        pd->display->setRefreshRate(20);
        fApp = get_instance(pd);//std::make_unique<KlangwellenPlaydateApplication>(pd);
        fApp->setup();

        /* setup callback, turn off lua */
        pd->system->setUpdateCallback(update, pd);

        setup_audio(pd);
    }

    if (event == kEventKeyPressed) {
        fApp->keyPressed(arg);
//        pd->system->logToConsole("key pressed : %i", arg);
    }

    if (event == kEventKeyReleased) {
        fApp->keyReleased(arg);
//        pd->system->logToConsole("key released: %i", arg);
    }

    if (event == kEventTerminate) {
//        pd->system->logToConsole("shutting down...");
        fApp = nullptr;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
