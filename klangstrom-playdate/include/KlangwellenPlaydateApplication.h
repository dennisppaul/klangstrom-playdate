#pragma once

#include <pd_api.h>
#include <cstdio>

#include "AudioState.h"

class KlangwellenPlaydateApplication {
public:
    explicit KlangwellenPlaydateApplication(PlaydateAPI *api) : pd(api) {}

    virtual void setup() = 0;

    virtual void update() = 0;

    virtual void keyPressed(uint32_t key) = 0;

    virtual void keyReleased(uint32_t key) = 0;

    virtual int audioblock(AudioState *context, int16_t *left, int16_t *right, int len) = 0;

    virtual void handleCrankEvent(int eventIndex) = 0;

    PlaydateAPI *pd;
protected:

    void handleCrank() {
        float crank           = pd->system->getCrankChange();
        char  crank_value[32] = {0};
        fAccumulatedCrank += crank;
        snprintf(crank_value, sizeof(crank_value), "crank change: %f", (double) crank);
        pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 20);
        snprintf(crank_value, sizeof(crank_value), "crank position: %i", (int) fAccumulatedCrank);
        pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 30);
    }

    void initCrankState() {
        lastAngle     = pd->system->getCrankAngle();
        totalRotation = 0.0f;
        for (bool &i: eventsTriggered) {
            i = false;
        }
//        for (int i    = 0; i < MAX_EVENTS; ++i) {
//            eventsTriggered[i] = false;
//        }
    }

    void checkCrankEvents() {
        float currentAngle = pd->system->getCrankAngle();
        float deltaAngle   = currentAngle - lastAngle;

        // Adjust deltaAngle for wrapping (if crank crosses 0/360 boundary)
        if (deltaAngle > 180.0f) deltaAngle -= FULL_ROTATION;
        if (deltaAngle < -180.0f) deltaAngle += FULL_ROTATION;

        totalRotation += deltaAngle;

        float triggerAngle     = FULL_ROTATION / (float) MAX_EVENTS;
        float absTotalRotation = fabs(totalRotation);

        for (int i = 0; i < MAX_EVENTS; ++i) {
            float eventAngle = triggerAngle * ((float) i + 1.0f);
            if (!eventsTriggered[i] && absTotalRotation >= eventAngle) {
                handleCrankEvent(i);
                eventsTriggered[i] = true;
            }

            if (absTotalRotation >= FULL_ROTATION) {
                eventsTriggered[i] = false;
            }
        }

        // Reset total rotation after completing a full rotation
        if (absTotalRotation >= FULL_ROTATION) {
            totalRotation = fmodf(totalRotation, FULL_ROTATION);
        }

        lastAngle = currentAngle;
    }

private:

    // TODO move to own class
    static constexpr int   MAX_EVENTS    = 8;
    static constexpr float FULL_ROTATION = 360.0f;

    float fAccumulatedCrank           = 0;
    float totalRotation               = 0.0f; // Total cumulative rotation
    float lastAngle                   = 0.0f;
    bool  eventsTriggered[MAX_EVENTS] = {false};
};
