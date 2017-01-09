
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/resource/identifier.hpp>

#include <glm/vec3.hpp>

#include <boost/python.hpp>

template <class T>
float get_item(T& v, int i)
{
    return v[i];
}

template <class T>
void set_item(T& v, int i, float in)
{
    v[i] = in;
}

BOOST_PYTHON_MODULE(sigmacore)
{
    using namespace boost::python;

    using namespace glm;

    class_<vec2>("vec2")
        .def_readwrite("x", &vec2::x)
        .def_readwrite("y", &vec2::y)
        .def("__getitem__", &get_item<vec2>)
        .def("__setitem__", &set_item<vec2>);
    class_<vec3>("vec3")
        .def_readwrite("x", &vec3::x)
        .def_readwrite("y", &vec3::y)
        .def_readwrite("z", &vec3::z)
        .def("__getitem__", &get_item<vec3>)
        .def("__setitem__", &set_item<vec3>);
    class_<vec4>("vec4")
        .def_readwrite("w", &vec4::w)
        .def_readwrite("x", &vec4::x)
        .def_readwrite("y", &vec4::y)
        .def_readwrite("z", &vec4::z)
        .def("__getitem__", &get_item<vec4>)
        .def("__setitem__", &set_item<vec4>);

    sigma::resource::export_development_identifier();
    sigma::graphics::export_static_mesh_data();
}
