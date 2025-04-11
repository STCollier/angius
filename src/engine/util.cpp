#include "util.hpp"

namespace util {
    std::string slurp(std::ifstream& in) {
        std::ostringstream sstr;
        sstr << in.rdbuf();
        return sstr.str();
    }

    size_t strSpaces(std::string str) {
        int x = 0;
        for (char& c : str) {
            if (c == 32) x++;
        }

        return x;
    }
};