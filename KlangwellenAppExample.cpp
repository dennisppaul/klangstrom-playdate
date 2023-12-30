#include <memory>
#include <string>
#include <pd_api.h>
#include <pdcpp/pdnewlib.h>

#include "KlangwellenAppExample.h"

KlangwellenApp* get_instance(PlaydateAPI *pd) {
    return new KlangwellenAppExample(pd);
}