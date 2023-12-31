#pragma once

#include <pd_api.h>
#include <cstdio>

#include "AudioState.h"

class KlangwellenPlaydateApplication {
public:
    explicit KlangwellenPlaydateApplication(PlaydateAPI *api) : pd(api) {
    }

    virtual ~KlangwellenPlaydateApplication() = default;

    virtual void setup() = 0;

    virtual void update() = 0;

    virtual void finish() = 0;

    virtual void keyPressed(uint32_t key) = 0;

    virtual void keyReleased(uint32_t key) = 0;

    virtual int audioblock(AudioState *context, int16_t *left, int16_t *right, int len) = 0;

    const static int width = 400;

    const static int height = 240;

    PlaydateAPI *pd;
};
