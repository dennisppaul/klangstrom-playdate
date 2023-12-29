#pragma once

#include <pd_api.h>

#include "KlangWellen.h"
#include "Wavetable.h"
#include "KlangwellenApp.h"
#include "AudioState.h"
#include "SAM.h"

using namespace klangwellen;

constexpr int TEXT_WIDTH  = 86;
constexpr int TEXT_HEIGHT = 16;

class KlangwellenApp {
public:
    explicit KlangwellenApp(PlaydateAPI *api)
            : pd(api), fontpath("/System/Fonts/Roobert-10-Bold.pft"), x((400 - TEXT_WIDTH) / 2),
              y((240 - TEXT_HEIGHT) / 2), dx(1), dy(2) {
        const char *err;
        font = pd->graphics->loadFont(fontpath.c_str(), &err);

        const char *path1 = "images/neven-patterns.png";
        flake = pd->graphics->loadBitmap(path1, &err);

        if (font == nullptr) {
            pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath.c_str(), err);
        }

        fWavetable.set_waveform(klangwellen::KlangWellen::WAVEFORM_SAWTOOTH, 16);
        fWavetable.set_frequency(55);
        fWavetable.set_amplitude(0.5);

        fSAM.speak("hello");

        PDMenuItem *menuItem       = pd->system->addMenuItem("Item 1", menuItemCallback, this);
        PDMenuItem *checkMenuItem  = pd->system->addCheckmarkMenuItem("Item 2", 1, menuCheckmarkCallback, this);
        const char *options[]      = {"one", "two", "three"};
        PDMenuItem *optionMenuItem = pd->system->addOptionsMenuItem("Item 3", options, 3, menuOptionsCallback, this);
    }

    void update() {
        pd->graphics->clear(kColorWhite);
        pd->graphics->drawBitmap(flake, 1, 50, kBitmapUnflipped);
        pd->graphics->setFont(font);
        pd->graphics->drawText("Klangwellen", strlen("Klangwellen"), kASCIIEncoding, x, y);

        handleCrank();

        x += dx;
        y += dy;

        if (x < 0 || x > LCD_COLUMNS - TEXT_WIDTH) {
            dx = -dx;
        }

        if (y < 0 || y > LCD_ROWS - TEXT_HEIGHT) {
            dy = -dy;
        }

        pd->graphics->drawText(key_pressed_value,
                               strlen(key_pressed_value),
                               kASCIIEncoding,
                               1,
                               10);

        pd->system->drawFPS(0, 0);

        fWavetable.set_frequency(55.0f + (float) y);

        PDButtons current;
        PDButtons pushed;
        pd->system->getButtonState(&current, &pushed, nullptr);
        if (pushed & kButtonUp) {
            pd->graphics->drawText("UP",
                                   strlen("UP"),
                                   kASCIIEncoding,
                                   1,
                                   10);
            pd->system->logToConsole("UP");
            fSAM.speak(std::to_string(beat_counter++));
        }
    }

    void keyPressed(uint32_t key) {
        snprintf(key_pressed_value, 24, "key_pressed: %i", key);
        printf("key_pressed: %i\n", key); // prints to terminal ( not console ) if simulator is run from terminal
    }

    void keyReleased(uint32_t key) {
    }

    int audioblock(AudioState *context, int16_t *left, int16_t *right, int len) {

//        char result[100] = {0};
//        sprintf(result, "%i", len);
//        pd->graphics->drawText(result, strlen(result), kASCIIEncoding, 10, 10);

        float mSAMBuffer[len];
        fSAM.process(mSAMBuffer);
        for (int i = 0; i < len; i++) {
            float mSample = 0.0f;
            mSample += fWavetable.process();
            mSample += mSAMBuffer[i];
            mSample *= 0.5f;
            left[i]  = (int16_t) (mSample * 32768);
            right[i] = left[i];
        }
        audio_frame_counter++;
        return 1;
    }

private:
    PlaydateAPI            *pd;
    std::string            fontpath;
    LCDFont                *font;
    int                    x, y, dx, dy;
    int                    audio_frame_counter   = 0;
    klangwellen::Wavetable fWavetable;
    char                   key_pressed_value[24] = {0};

    int8_t           fBuffer[48000]{0};
    klangwellen::SAM fSAM{fBuffer, 48000};
    uint32_t         beat_counter      = 0;
    float            fAccumulatedCrank = 0;
    LCDBitmap        *flake;

    void handleCrank() {
        float crank           = pd->system->getCrankChange();
        char  crank_value[32] = {0};
        fAccumulatedCrank += crank;
        snprintf(crank_value, sizeof(crank_value), "crank change: %f", (double) crank);
        pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 20);
        snprintf(crank_value, sizeof(crank_value), "crank position: %i", (int) fAccumulatedCrank);
        pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 30);
    }

    static void menuItemCallback(void *userdata) {
        auto *instance = static_cast<KlangwellenApp *>(userdata);
        instance->pd->system->logToConsole("menu item callback ...");
    }

    static void menuCheckmarkCallback(void *userdata) {
        auto *instance = static_cast<KlangwellenApp *>(userdata);
        instance->pd->system->logToConsole("menu checkmark callback ...");
    }

    static void menuOptionsCallback(void *userdata) {
        auto *instance = static_cast<KlangwellenApp *>(userdata);
        instance->pd->system->logToConsole("menu options callback ...");
    }
};
