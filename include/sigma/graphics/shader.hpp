#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/resource/manager.hpp>

#include <string>

namespace sigma {
namespace graphics {
    enum class shader_type : size_t {
        vertex,
        fragment,
        geometry,
        header
    };

    static constexpr std::array<shader_type, 3> all_shader_types()
    {
        return { shader_type::vertex, shader_type::fragment, shader_type::geometry };
    }

    struct shader_data {
        shader_type type;
        std::string source;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type;
            ar& source;
        }
    };

    class SIGMA_API shader {
    public:
        using resource_data = shader_data;

        shader() = default;

        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        virtual ~shader() = default;

    private:
        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;
    };

    using shader_manager = resource::manager<shader>;
}
}

namespace std {
template <>
struct hash<sigma::graphics::shader_type> {
    size_t operator()(const sigma::graphics::shader_type& type) const
    {
        return size_t(type);
    }
};
}

BOOST_CLASS_VERSION(sigma::graphics::shader_data, 1);

#endif // SIGMA_GRAPHICS_SHADER_HPP
