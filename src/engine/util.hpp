#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace util {
    std::string slurp(std::ifstream& in);
    size_t strSpaces(std::string str);
    void renderQuad();
};