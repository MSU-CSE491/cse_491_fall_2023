//
// Created by Vincenzo on 10/18/2023.
//
#include <map>
#include "TextureHolder.hpp"

namespace i_2D
{
    /**
     * @brief Stores a texture under the given id/name;
     *
     * @param id Identifier used to reference this texture in the future.
     * @param file_name File to load the texture from.
     */
    void TextureHolder::LoadTexture(std::string id, std::string file_name) {
        // Load texture to pointer
        std::unique_ptr<sf::Texture> texture(new sf::Texture());
        if(!texture->loadFromFile(file_name))
        {
            throw std::runtime_error("Failed to load texture from: " + file_name);
        }

        // Insert the pointer to map with id as key
        textures_.insert(std::make_pair(id, std::move(texture)));
        return;
    }

    /**
     * @brief Returns a held texture matching the given id.
     *
     * @param id Identifier/Name of the texture stored.
     * @return The sf::Texture matching the id.
     */
    const sf::Texture &TextureHolder::GetTexture(std::string id) {
        // Find iterator of matching id (avoids copy of unique_ptr)
        auto got = textures_.find(id);
        // Return dereferenced unique_ptr found at iterator pair
        return *got->second;
    }
    /*
     * @brief This function loads texture for the maze world images
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<std::string, sf::Texture> TextureHolder::MazeTexture()
    {

        std::map<std::string, sf::Texture> textures;

        // Load textures

        LoadTexture("wallTexture", "../assets/walls/wall.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/default-the-first.png");
        textures["wall"] = GetTexture("wallTexture");
        textures["troll"] = GetTexture("trollTexture");
        textures["agent"] = GetTexture("agentTexture");
        return textures;
    }
    /*
     * @brief This function loads texture for the second world images
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<std::string, sf::Texture> TextureHolder::SecondWorldTexture()
    {

        std::map<std::string, sf::Texture> textures;

        // Load textures
        LoadTexture("wallTexture", "../assets/walls/brick_wall-red.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/witch-girl.png");
        LoadTexture("armourTexture", "../assets/weapons/leather_armor.png");
        LoadTexture("swordTexture", "../assets/weapons/longsword.png");
        LoadTexture("axeTexture", "../assets/weapons/w_axe_war.png");
        LoadTexture("daggerTexture", "../assets/weapons/dagger.png");
        LoadTexture("chestTexture", "../assets/weapons/chest_closed.png");
        LoadTexture("flagTexture", "../assets/weapons/4_Conjoined_Spikes.png");
        textures["wall"]= GetTexture("wallTexture");
        textures["troll"]= GetTexture("trollTexture");
        textures["agent"]= GetTexture("agentTexture");
        textures["armour"]= GetTexture("armourTexture");
        textures["sword"]= GetTexture("swordTexture");
        textures["axe"]= GetTexture("axeTexture");
        textures["dagger"]= GetTexture("daggerTexture");
        textures["chest"]= GetTexture("chestTexture");
        textures["flag"]= GetTexture("flagTexture");
        return textures;
    }
    /*
     * @brief This function loads texture for the manual world images
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<std::string, sf::Texture> TextureHolder::ManualWorldTexture()
    {

        std::map<std::string, sf::Texture> textures;

        // Load textures
        LoadTexture("wallTexture", "../assets/walls/brick_wall.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/witch-girl.png");
        LoadTexture("treeTexture", "../assets/trees/tree1.png");
        LoadTexture("waterTexture", "../assets/Ground_tiles/water.jpg");
        LoadTexture("axeTexture", "../assets/weapons/w_axe_war_steel.png");
        LoadTexture("boatTexture", "../assets/weapons/leather_armor.png");
        textures["axe1"]= GetTexture("axeTexture");
        textures["boat"]= GetTexture("boatTexture");
        textures["wall"]= GetTexture("wallTexture");
        textures["troll"]= GetTexture("trollTexture");
        textures["agent"]= GetTexture("agentTexture");
        textures["tree"]= GetTexture("treeTexture");
        textures["water"]= GetTexture("waterTexture");


        return textures;
    }

}