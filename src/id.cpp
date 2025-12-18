#include "id.h"

IdGenerator::IdGenerator() {
    _next = 1;
}

uint32_t IdGenerator::make_id() {
    return _next++;
}
