#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glad/glad.h>
#include <format>

#include "texture.hpp"

Texture::Texture(std::string src, FilteringType type) : src{src} {
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type == NEAREST ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type == NEAREST ? GL_NEAREST : GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (!stbi_is_hdr(src.c_str())) {
        unsigned char *data = stbi_load(src.c_str(), &width, &height, &channels, 0);
        if (data) {
            GLenum format;

            switch (channels) {
                case 1: 
                    format = GL_RED;
                    break;
                case 2:
                    format = GL_RG;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error(std::format("Failed to load texture file {}\n", src));
        }

        stbi_image_free(data);
    } else {
        float *data = stbi_loadf(src.c_str(), &width, &height, &channels, 0);

        if (data) {
            GLenum format;

            switch (channels) {
                case 1: 
                    format = GL_RED;
                    break;
                case 2:
                    format = GL_RG;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
            }

            GLenum internalFormat = (channels == 4) ? GL_RGBA16F : GL_RGB16F;

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error(std::format("Failed to load texture file {}\n", src));
        }

        stbi_image_free(data);
    }
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}