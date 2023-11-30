//
// Created by Vincenzo on 10/18/2023.
//

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
     * @brief This function loads texture for the maze world images - Default maze
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<char, sf::Texture> TextureHolder::MazeTexture()
    {

        std::map<char, sf::Texture> textures;

        // Load textures

        LoadTexture("wallTexture", "../assets/walls/wall.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/default-the-first.png");
        textures['#'] = GetTexture("wallTexture");
        textures['*'] = GetTexture("trollTexture");
        textures['@'] = GetTexture("agentTexture");
        return textures;
    }
    /*
     * @brief This function loads texture for the second world images group 4
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<char, sf::Texture> TextureHolder::SecondWorldTexture()
    {

        std::map<char, sf::Texture> textures;

        // Load textures
        LoadTexture("wallTexture", "../assets/walls/wall.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/witch-girl.png");
        LoadTexture("armourTexture", "../assets/weapons/leather_armor.png");
        LoadTexture("swordTexture", "../assets/weapons/longsword.png");
        LoadTexture("axeTexture", "../assets/weapons/w_axe_war.png");
        LoadTexture("daggerTexture", "../assets/weapons/dagger.png");
        LoadTexture("chestTexture", "../assets/weapons/chest_closed.png");
        LoadTexture("flagTexture", "../assets/weapons/flag.png");
        LoadTexture("pathTexture", "../assets/Ground_tiles/Grass2.png");

        textures['#'] = GetTexture("wallTexture");
        textures['*'] = GetTexture("trollTexture");
        textures['@'] = GetTexture("agentTexture");
        textures['+'] = GetTexture("armourTexture");
        textures['S'] = GetTexture("swordTexture");
        textures['A'] = GetTexture("axeTexture");
        textures['D'] = GetTexture("daggerTexture");
        textures['C'] = GetTexture("chestTexture");
        textures['g'] = GetTexture("flagTexture");
        textures[' '] = GetTexture("pathTexture");

        return textures;
    }
    /*
     * @brief This function loads texture for the manual world images group 8
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<char, sf::Texture> TextureHolder::ManualWorldTexture()
    {

        std::map<char, sf::Texture> textures;

        // Load textures
        LoadTexture("wallTexture", "../assets/walls/brick_wall.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/witch-girl.png");
        LoadTexture("treeTexture", "../assets/trees/tree1.png");
        LoadTexture("waterTexture", "../assets/Ground_tiles/water.jpg");
        LoadTexture("axeTexture", "../assets/weapons/w_axe_war_steel.png");
        LoadTexture("boatTexture", "../assets/weapons/Boat_color1_2.png");
        LoadTexture("pathTexture", "../assets/Ground_tiles/Sand1.png");
        LoadTexture("portal1Texture", "../assets/walls/portal1.png");
        LoadTexture("portal2Texture", "../assets/walls/portal2.png");
        textures['P'] = GetTexture("axeTexture");
        textures['U'] = GetTexture("boatTexture");
        textures['#'] = GetTexture("wallTexture");
        textures['*'] = GetTexture("trollTexture");
        textures['@'] = GetTexture("agentTexture");
        textures['^'] = GetTexture("treeTexture");
        textures['~'] = GetTexture("waterTexture");
        textures[' '] = GetTexture("pathTexture");
        textures['{'] = GetTexture("portal1Texture");
        textures['}'] = GetTexture("portal2Texture");

        return textures;
    }
    /*
     * @brief This function loads texture for the generative world images group 6
     * @return std::map< name, texture> returns the map, key is the name of the texture and values is the actual texture
     */
    std::map<char, sf::Texture> TextureHolder::GenerativeWorldTexture()
    {

        std::map<char, sf::Texture> textures;

        // Load textures
        LoadTexture("wallTexture", "../assets/walls/stone_wall02.png");
        LoadTexture("trollTexture", "../assets/agents/troll.png");
        LoadTexture("agentTexture", "../assets/agents/Character_03_Front.png");
        LoadTexture("tarTexture", "../assets/Ground_tiles/tar.jpg");
        LoadTexture("doorTexture", "../assets/walls/castledoors.png");
        LoadTexture("spikeTexture", "../assets/weapons/Individual_Spike.png");
        LoadTexture("keyTexture", "../assets/weapons/key.png");
        LoadTexture("pathTexture", "../assets/Ground_tiles/Dirt1.png");
        LoadTexture("bootTexture", "../assets/weapons/2.png");
        LoadTexture("shieldTexture", "../assets/weapons/shield.png");
        LoadTexture("teleportTexture", "../assets/walls/teleport.png");
        LoadTexture("armoryTexture", "../assets/weapons/steel_armor.png");
        LoadTexture("waterTexture", "../assets/Ground_tiles/water.jpg");
        LoadTexture("sandTexture", "../assets/Ground_tiles/Sand1.png");
        LoadTexture("grassTexture", "../assets/Ground_tiles/Grass2.png");
        LoadTexture("dirtTexture", "../assets/Ground_tiles/Dirt1.png");
        textures['~'] = GetTexture("dirtTexture");
        textures['M'] = GetTexture("grassTexture");
        textures['-'] = GetTexture("sandTexture");
        textures['W'] = GetTexture("waterTexture");
        textures['B'] = GetTexture("bootTexture");
        textures['X'] = GetTexture("spikeTexture");
        textures['O'] = GetTexture("tarTexture");
        textures['#'] = GetTexture("wallTexture");
        textures['*'] = GetTexture("trollTexture");
        textures['@'] = GetTexture("agentTexture");
        textures['D'] = GetTexture("doorTexture");
        textures['K'] = GetTexture("keyTexture");
        textures[' '] = GetTexture("pathTexture");
        textures['S'] = GetTexture("shieldTexture");
        textures['T'] = GetTexture("teleportTexture");
        textures['A'] = GetTexture("armoryTexture");

        return textures;
    }

}