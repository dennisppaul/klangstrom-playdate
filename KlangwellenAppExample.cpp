#include <memory>
#include <pd_api.h>

#include "KlangwellenAppExample.h"

KlangwellenPlaydateApplication* get_instance(PlaydateAPI *pd) {
    return new KlangwellenAppExample(pd);
}
