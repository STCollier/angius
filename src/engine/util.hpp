#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <math.h>

namespace util {
    constexpr float TAU = M_PI * 2;

    std::string slurp(std::ifstream& in);
    size_t strSpaces(std::string str);
    void renderQuad();
};