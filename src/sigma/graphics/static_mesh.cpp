#include <sigma/graphics/static_mesh.hpp>

#include <sigma/util/array_indexing_suite.hpp>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <boost/archive/binary_oarchive.hpp>

namespace sigma {
namespace graphics {
    resource::handle<graphics::material> static_mesh::material()
    {
        return material_;
    }

    void static_mesh::set_material(resource::handle<graphics::material> mat)
    {
        if (material_ != mat)
            material_ = mat;
    }

    void export_static_mesh_data()
    {
        using namespace sigma::graphics;
        using namespace boost::python;
        using namespace sigma::python;

        scope outer = class_<static_mesh_data>("static_mesh_data")
                          .def_readwrite("vertices", &static_mesh_data::vertices)
                          .def_readwrite("triangles", &static_mesh_data::triangles)
                          .def_readwrite("material", &static_mesh_data::material)
                          .def("write_to_disk", make_function(
                                                    [](const static_mesh_data& mesh_data, resource::development_identifier rid, std::string output_directory) {
                                                        auto final_path = boost::filesystem::path(output_directory) / std::to_string(rid.value());
                                                        std::ofstream stream(final_path.string(), std::ios::binary | std::ios::out);
                                                        boost::archive::binary_oarchive oa(stream);
                                                        oa << mesh_data;
                                                    },
                                                    default_call_policies(),
                                                    boost::mpl::vector<void, const static_mesh_data&, resource::development_identifier, std::string>()));

        class_<static_mesh_data::vertex>("vertex")
            .def_readwrite("position", &static_mesh_data::vertex::position)
            .def_readwrite("normal", &static_mesh_data::vertex::normal)
            .def_readwrite("tangent", &static_mesh_data::vertex::tangent)
            .def_readwrite("texcoord", &static_mesh_data::vertex::texcoord);

        class_<static_mesh_data::triangle>("triangle")
            .def(array_indexing_suite<static_mesh_data::triangle>());

        class_<std::vector<static_mesh_data::vertex> >("vertex_vector")
            .def(vector_indexing_suite<std::vector<static_mesh_data::vertex> >());

        class_<std::vector<static_mesh_data::triangle> >("triangle_vector")
            .def(vector_indexing_suite<std::vector<static_mesh_data::triangle> >());
    }
}
}
