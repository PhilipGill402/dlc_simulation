#pragma once
#include <cstdint>

class IdGenerator {
    uint32_t _next;
public:
    IdGenerator();
    uint32_t make_id();
};
