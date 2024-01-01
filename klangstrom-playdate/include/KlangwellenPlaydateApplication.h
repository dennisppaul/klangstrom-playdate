#pragma once

#include <pd_api.h>
#include <cstdio>

#include "AudioState.h"

class KlangwellenPlaydateApplication {
    inline static uint32_t x32Seed = 23;

public:
    virtual ~KlangwellenPlaydateApplication() = default;

    void set_environment(PlaydateAPI *api) {
        pd = api;
    }

    void set_framerate(const float framerate) const {
        pd->display->setRefreshRate(framerate);
    }

    virtual void setup() = 0;

    virtual void update() = 0;

    virtual void finish() = 0;

    virtual void keyPressed(uint32_t key) = 0;

    virtual void keyReleased(uint32_t key) = 0;

    virtual int audioblock(AudioState *context, int16_t *left, int16_t *right, int len) = 0;

    static constexpr int width = 400;

    static constexpr int height = 240;

    PlaydateAPI *pd = nullptr;

    static float random() {
        // TODO replace with mapping, without division
        return static_cast<float>(xorshift32()) / UINT32_MAX;
    }

private:
    /* xorshift32 ( ref: https://en.wikipedia.org/wiki/Xorshift ) */
    // static uint32_t x32Seed = 23;
    static uint32_t xorshift32() {
        x32Seed ^= x32Seed << 13;
        x32Seed ^= x32Seed >> 17;
        x32Seed ^= x32Seed << 5;
        return x32Seed;
    }
};
