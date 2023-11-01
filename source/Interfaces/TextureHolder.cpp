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
}