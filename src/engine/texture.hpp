#pragma once

#include <string>

enum FilteringType {
	LINEAR = 0,
    NEAREST
};

class Texture {
    public:
        std::string src;
        int width, height, channels;

        Texture(std::string src, FilteringType type);

        void bind();

    private:
        unsigned int m_texture;
};