#pragma once

#include <cmath>

class CrankListener {
public:
    virtual ~CrankListener() = default;

    virtual void crank_event(int event_id, int direction) = 0;
};

class Crank {
public:
    // void handleCrank() {
    //     float crank          = pd->system->getCrankChange();
    //     char crank_value[32] = {0};
    //     fAccumulatedCrank += crank;
    //     snprintf(crank_value, sizeof(crank_value), "crank change: %f", (double) crank);
    //     pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 20);
    //     snprintf(crank_value, sizeof(crank_value), "crank position: %i", (int) fAccumulatedCrank);
    //     pd->graphics->drawText(crank_value, strlen(crank_value), kASCIIEncoding, 1, 30);
    // }

    explicit Crank(int number_of_events_per_rotation) : fNumEventsPerRotation(number_of_events_per_rotation) {
        fEventsTriggered = new bool[fNumEventsPerRotation];
    }

    ~Crank() {
        delete fEventsTriggered;
    }

    void init(float crank_angle) {
        lastAngle     = crank_angle; //pd->system->getCrankAngle();
        totalRotation = 0.0f;
        // for (bool &i: eventsTriggered) {
        //     i = false;
        // }
        for (int i = 0; i < fNumEventsPerRotation; ++i) {
            fEventsTriggered[i] = false;
        }
    }

    void update(float crank_angle) {
        float currentAngle = crank_angle; //pd->system->getCrankAngle();
        float deltaAngle   = currentAngle - lastAngle;

        // Adjust deltaAngle for wrapping (if crank crosses 0/360 boundary)
        if (deltaAngle > 180.0f) deltaAngle -= FULL_ROTATION;
        if (deltaAngle < -180.0f) deltaAngle += FULL_ROTATION;

        totalRotation += deltaAngle;

        const float triggerAngle     = FULL_ROTATION / (float) fNumEventsPerRotation;
        const float absTotalRotation = fabs(totalRotation);

        for (int i = 0; i < fNumEventsPerRotation; ++i) {
            const float eventAngle = triggerAngle * ((float) i + 1.0f);
            if (!fEventsTriggered[i] && absTotalRotation >= eventAngle) {
                if (crankListener != nullptr) {
                    crankListener->crank_event(i, deltaAngle > 0.0f ? 1 : -1);
                }
                fEventsTriggered[i] = true;
            }

            if (absTotalRotation >= FULL_ROTATION) {
                fEventsTriggered[i] = false;
            }
        }

        // Reset total rotation after completing a full rotation
        if (absTotalRotation >= FULL_ROTATION) {
            totalRotation = fmodf(totalRotation, FULL_ROTATION);
        }

        lastAngle = currentAngle;
    }

    void set_listener(CrankListener *listener) {
        crankListener = listener;
    }

private:
    const int fNumEventsPerRotation;
    static constexpr float FULL_ROTATION = 360.0f;
    float totalRotation                  = 0.0f; // Total cumulative rotation
    float lastAngle                      = 0.0f;
    bool *fEventsTriggered;
    CrankListener *crankListener = nullptr;
};
