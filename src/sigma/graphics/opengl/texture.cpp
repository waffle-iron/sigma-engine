#include <sigma/graphics/opengl/texture.hpp>

#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    std::pair<GLenum, GLenum> convert_gl(graphics::texture_format fmt)
    {
        switch (fmt) {
        case graphics::texture_format::RGB8:
            return { GL_RGB, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGBA8:
            return { GL_RGBA, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGB32F:
            return { GL_RGB, GL_FLOAT };
        }
    }

    internal_format convert_internal(graphics::texture_format fmt)
    {
        switch (fmt) {
        case graphics::texture_format::RGB8:
            return internal_format::RGB8;
        case graphics::texture_format::RGBA8:
            return internal_format::RGBA8;
        case graphics::texture_format::RGB32F:
            return internal_format::RGB32F;
        }
    }

    texture::texture(internal_format format, glm::ivec2 size,
        graphics::texture_filter minification_filter,
        graphics::texture_filter magnification_filter,
        graphics::texture_filter mipmap_filter)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (magnification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));

        int mip_levels = 1;
        if (mipmap_filter != graphics::texture_filter::NONE) {
            if (minification_filter == graphics::texture_filter::LINEAR) {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST));
            } else {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
            }

            mip_levels = calculate_mipmap_levels(size.x, size.y);
        } else {
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (minification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));
        }

        GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, mip_levels, GLenum(format), size.x, size.y));
    }

    texture::texture(internal_format format, glm::ivec2 size,
        graphics::texture_filter minification_filter,
        graphics::texture_filter magnification_filter,
        graphics::texture_filter mipmap_filter,
        graphics::texture_format data_format,
        const std::vector<char>& data)
        : texture(format, size, minification_filter, magnification_filter, mipmap_filter)
    {
        auto gl_format = convert_gl(data_format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, gl_format.first, gl_format.second, data.data()));
        if (mipmap_filter != graphics::texture_filter::NONE)
            GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }

    texture::texture(graphics::texture_data data)
        : texture(convert_internal(data.format), data.size, data.minification_filter, data.magnification_filter, data.mipmap_filter, data.format, data.data)
    {
    }

    texture::~texture()
    {
        glDeleteTextures(1, &object_);
    }

    GLuint texture::get_object() const
    {
        return object_;
    }

    void texture::bind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
    }

    std::unique_ptr<graphics::texture> texture_manager::create(graphics::texture_data data)
    {
        return std::make_unique<texture>(std::move(data));
    }
}
}
