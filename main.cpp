#include <memory>
#include <string>
#include <pd_api.h>
#include <pdcpp/pdnewlib.h>

#include "KlangWellen.h"
#include "Wavetable.h"

// from https://github.com/nstbayless/playdate-cpp

typedef struct {
    PlaydateAPI *pd;
    SoundSource *source;
} AudioState;

constexpr int TEXT_WIDTH = 86;
constexpr int TEXT_HEIGHT = 16;

class KlangwellenApp {
public:
    explicit KlangwellenApp(PlaydateAPI *api)
            : pd(api), fontpath("/System/Fonts/Asheville-Sans-14-Bold.pft"), x((400 - TEXT_WIDTH) / 2),
              y((240 - TEXT_HEIGHT) / 2), dx(1), dy(2) {
        const char *err;
        font = pd->graphics->loadFont(fontpath.c_str(), &err);

        if (font == nullptr) {
            pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath.c_str(), err);
        }

        fWavetable.set_waveform(klangwellen::KlangWellen::WAVEFORM_SAWTOOTH, 16);
        fWavetable.set_frequency(55);
        fWavetable.set_amplitude(0.5);
    }

    void update() {
        pd->graphics->clear(kColorWhite);
        pd->graphics->setFont(font);
        pd->graphics->drawText("Klangwellen", strlen("Klangwellen"), kASCIIEncoding, x, y);

        x += dx;
        y += dy;

        if (x < 0 || x > LCD_COLUMNS - TEXT_WIDTH) {
            dx = -dx;
        }

        if (y < 0 || y > LCD_ROWS - TEXT_HEIGHT) {
            dy = -dy;
        }

        pd->system->drawFPS(0, 0);

        fWavetable.set_frequency(55 + y);
    }

    int audioblock(AudioState *context, int16_t *left, int16_t *right, int len) {

//        char result[100] = {0};
//        sprintf(result, "%i", len);
//        pd->graphics->drawText(result, strlen(result), kASCIIEncoding, 10, 10);

        for (int i = 0; i < len; i++) {
            left[i] = (int16_t)(fWavetable.process() * 32768);
            right[i] = left[i];
        }
        audio_frame_counter++;
        return 1;
    }

private:
    PlaydateAPI *pd;
    std::string fontpath;
    LCDFont *font;
    int x, y, dx, dy;
    int audio_frame_counter = 0;

    klangwellen::Wavetable fWavetable;
};

std::unique_ptr<KlangwellenApp> fApp;

static int update(void *userdata) {
    fApp->update();
    return 1;
};

static int update_audio(void *context, int16_t *left, int16_t *right, int len) {
    // Do not use the audio callback for system tasks:
    //   spend as little time here as possible
    auto *state = (AudioState *) context;
    return fApp->audioblock(state, left, right, len);
}

/**
 * the `eventHandler` function is the entry point for all Playdate applications
 * and Lua extensions. It requires C-style linkage (no name mangling) so we
 * must wrap the entire thing in this `extern "C" {` block
 */
#ifdef __cplusplus
extern "C" {
#endif

void setup_audio(PlaydateAPI *pd) {
    // Create a new audio source with a state context
    auto *state = (AudioState *) pd->system->realloc(nullptr, sizeof(AudioState));
    state->pd = pd;
    state->source = pd->sound->addSource(&update_audio, state, 1);
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg) {
    /* This is required, otherwise linker errors abound */
    eventHandler_pdnewlib(pd, event, arg);

    if (event == kEventInit) {
        pd->display->setRefreshRate(20);
        fApp = std::make_unique<KlangwellenApp>(pd);

        /* setup callback, turn off lua */
        pd->system->setUpdateCallback(update, pd);

        setup_audio(pd);
    }

    if (event == kEventTerminate) {
        pd->system->logToConsole("shutting down...");
        fApp = nullptr;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
