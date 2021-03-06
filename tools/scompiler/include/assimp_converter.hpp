#ifndef SIGMA_ENGINE_CONVERT_ASSIMP_CONVERTER_HPP
#define SIGMA_ENGINE_CONVERT_ASSIMP_CONVERTER_HPP

#include <json/json.h>

#include <boost/filesystem/path.hpp>

#include <set>

namespace Assimp {
class Importer;
}
class aiMesh;
class aiNode;
class aiMaterial;

namespace sigma {
namespace graphics {
    struct static_mesh_data;
}

class converter;

class assimp_converter {
public:
    assimp_converter(boost::filesystem::path package_root, boost::filesystem::path source_file);

    ~assimp_converter();

    const std::set<std::string>& material_names() const;

    const std::set<std::string>& static_mesh_names() const;

    const std::set<std::string>& scene_object_names() const;

    void convert_material(std::string name, Json::Value& material) const;

    void convert_static_mesh(std::string name, graphics::static_mesh_data& mesh) const;

    void convert_object(std::string name, Json::Value& entity) const;

private:
    boost::filesystem::path root_directroy_;
    boost::filesystem::path source_file_;
    std::unique_ptr<Assimp::Importer> importer_;
    std::unique_ptr<converter> converter_;

    std::set<std::string> material_names_;
    std::set<std::string> static_mesh_names_;
    std::set<std::string> object_names_;

    std::string get_name(const aiMesh* mesh) const;

    std::string get_name(const aiNode* node) const;

    std::string get_name(const aiMaterial* mat, int texture_type) const;

    bool is_source(const aiMaterial* mat) const;

    std::string get_name(const aiMaterial* mat) const;
};
}

#endif // SIGMA_ENGINE_CONVERT_ASSIMP_CONVERTER_HPP
