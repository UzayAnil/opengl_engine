#include "tgl/mesh.h"
#include "tmc/chunk.h"

namespace tmc
{

Chunk::Chunk(tgl::MeshVertices * mesh_vertices,
             tgl::Material * block_material,
             int type_count,
             std::vector <std::vector <GLfloat>> instances)
{
    is_chunk = true;
    static_physics = true;
	int unused_count = 50;
    for (int i = 0; i < type_count; ++i)
    {
        tgl::Mesh * temp_mesh = new tgl::Mesh(mesh_vertices);
        temp_mesh->set_material(block_material, i);
        //if (instances[i].size())
        {
            instances[i].resize(instances[i].size() + unused_count *3, -0.5);
            temp_mesh->enable_instancing(&(instances[i][0]),
                                         (instances[i].size() / 3) - unused_count,
										 unused_count);
        }
        add_component(temp_mesh);
    }
    block_instances = instances;
}

Chunk::~Chunk()
{
}

bool Chunk::remove_instance(int block_type, glm::vec3 loc)
{
    std::vector <GLfloat>& fresh_instances =
        (std::dynamic_pointer_cast<tgl::Mesh>(components[block_type - 1]))->
            get_instances();
    int index = -1;
    for (int i = 0; i < fresh_instances.size(); i += 3)
    {
        if (fresh_instances[i] == static_cast<int>(loc.x) &&
            fresh_instances[i + 1] == static_cast<int>(loc.y) &&
            fresh_instances[i + 2] == static_cast<int>(loc.z))
        {
            index = i/3;
            break;
        }
    }
    if (index != -1)
    {
        (std::dynamic_pointer_cast<tgl::Mesh>(components[block_type - 1]))->
            remove_instance(index);
        return true;
    }
    return false;
}

void Chunk::add_instance(int block_type, glm::vec3 loc)
{
    std::vector <GLfloat>& fresh_instances =
        (std::dynamic_pointer_cast<tgl::Mesh>(components[block_type - 1]))->
            get_instances();
    for (int i = 0; i < fresh_instances.size(); i += 3)
    {
        if (fresh_instances[i] == static_cast<int>(loc.x) &&
            fresh_instances[i + 1] == static_cast<int>(loc.y) &&
            fresh_instances[i + 2] == static_cast<int>(loc.z))
        {
            return;
        }
    }
    int index = 
        (std::dynamic_pointer_cast<tgl::Mesh>(components[block_type - 1]))->
           add_instance(loc);
    return;

    for (int i = 0; i < block_instances[block_type - 1].size(); i += 3)
    {
        if (block_instances[block_type - 1][i] == static_cast<int>(loc.x) &&
            block_instances[block_type - 1][i + 1] == static_cast<int>(loc.y) &&
            block_instances[block_type - 1][i + 2] == static_cast<int>(loc.z))
        {
            return;
        }
    }
    index = 
        (std::dynamic_pointer_cast<tgl::Mesh>(components[block_type - 1]))->
            add_instance(loc);
    if (index < block_instances[block_type - 1].size())
    {
        block_instances[block_type - 1][3*index] = static_cast<int>(loc.x);
        block_instances[block_type - 1][3 * index + 1] = static_cast<int>(loc.y);
        block_instances[block_type - 1][3 * index + 2] = static_cast<int>(loc.z);
    }
    else
    {
        block_instances[block_type - 1].resize((index+1) * 3);
        block_instances[block_type - 1][3 * index] = static_cast<int>(loc.x);
        block_instances[block_type - 1][3 * index + 1] = static_cast<int>(loc.y);
        block_instances[block_type - 1][3 * index + 2] = static_cast<int>(loc.z);
    }
}

}  // namespace tmc
