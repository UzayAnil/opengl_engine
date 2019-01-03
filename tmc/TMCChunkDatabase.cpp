#include "TMCChunkDatabase.h"


void TMCChunkDatabase::load_chunk(int chunk_x, int chunk_y, std::map <chunk_coord, std::map<block_coord,block_def>>& world_mods)
{
    chunk_coord chunk_to_load(chunk_x, chunk_y);
    std::ifstream chunk_file;
    std::string filename = std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".dat";
    chunk_file.open (db_path + "/" + filename, std::ios::in | std::ios::binary);
    if (chunk_file.good())
    {
        uint32_t block_count;
        chunk_file.read((char*)&block_count, sizeof(uint32_t));
        for (int i = 0; i < block_count; ++i)
        {
            uint16_t x;
            uint16_t y;
            uint16_t z;
            uint8_t block_type_byte;
            e_block_type block_type;
            chunk_file.read((char*)&x, sizeof(uint16_t));
            chunk_file.read((char*)&y, sizeof(uint16_t));
            chunk_file.read((char*)&z, sizeof(uint16_t));
            chunk_file.read((char*)&block_type_byte, sizeof(uint8_t));
            block_coord block_to_load(x,y,z);
            block_def block_def_to_load(x,y,z,(e_block_type)block_type_byte);
            if (world_mods.find(chunk_to_load) == world_mods.end())
            {
                world_mods[chunk_to_load] = std::map<block_coord,block_def>();
            }
            world_mods[chunk_to_load][block_to_load] = block_def_to_load;
        }
        //chunk_file << "Writing this to a file.\n";
        chunk_file.close();
    }
}

void TMCChunkDatabase::save_chunk(int chunk_x, int chunk_y, std::map <chunk_coord, std::map<block_coord,block_def>>& world_mods)
{
    chunk_coord chunk_to_write(chunk_x, chunk_y);
    std::ofstream chunk_file;
    std::string filename = std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + ".dat";
    chunk_file.open (db_path + "/" + filename, std::ios::out | std::ios::binary);
    if (chunk_file.good())
    {
        if (world_mods.find(chunk_to_write) != world_mods.end())
        {
            uint32_t block_count = world_mods[chunk_to_write].size();
            chunk_file.write((char*)&block_count,sizeof(uint32_t));
            for (auto block : world_mods[chunk_to_write])
            {
                uint16_t x = block.second.loc.x;
                uint16_t y = block.second.loc.y;
                uint16_t z = block.second.loc.z;
                uint8_t block_type_byte = (uint8_t)block.second.type;
                chunk_file.write((char*)&x, sizeof(uint16_t));
                chunk_file.write((char*)&y, sizeof(uint16_t));
                chunk_file.write((char*)&z, sizeof(uint16_t));
                chunk_file.write((char*)&block_type_byte, sizeof(uint8_t));
            }
        }
        
    }
}