#include <memory>
#include <pd_api.h>

#include "KlangwellenAppExample.h"

KlangwellenPlaydateApplication* get_instance() {
    return new KlangwellenAppExample();
}
