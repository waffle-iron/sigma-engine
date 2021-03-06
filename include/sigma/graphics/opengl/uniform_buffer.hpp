#ifndef SIGMA_OPENGL_UNIFORM_BUFFER_HPP
#define SIGMA_OPENGL_UNIFORM_BUFFER_HPP

#include <glad/glad.h>
#include <sigma/graphics/opengl/util.hpp>

#include <cstring>

namespace sigma {
namespace opengl {

    template <class UnifomBufferType>
    class uniform_buffer {
    public:
        uniform_buffer()
        {
            GL_CHECK(glGenBuffers(1, &object_));
            GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, object_));
            GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, sizeof(UnifomBufferType), nullptr, GL_DYNAMIC_DRAW));
        }

        uniform_buffer(uniform_buffer&&) = default;

        uniform_buffer& operator=(uniform_buffer&&) = default;

        ~uniform_buffer()
        {
            glDeleteBuffers(1, &object_);
        }

        void set_data(const UnifomBufferType& data)
        {
            GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, object_));
            GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UnifomBufferType), &data));
        }

        void set_binding_point(unsigned int index)
        {
            GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, index, object_));
        }

    private:
        uniform_buffer(const uniform_buffer&) = delete;
        uniform_buffer& operator=(const uniform_buffer&) = delete;

        GLuint object_ = 0;
    };
}
}

#endif // SIGMA_OPENGL_UNIFORM_BUFFER_HPP
