#pragma once
#include <print>

#define BAD_IMPLEMENTED(message) {                                         \
    std::println("Function {} is bad implemented:", __PRETTY_FUNCTION__);  \
    std::println("{}", message);                                           \
}

#define NOT_IMPLEMENTED() {                                               \
    std::println("Function {} is not implemented", __PRETTY_FUNCTION__);  \
}

