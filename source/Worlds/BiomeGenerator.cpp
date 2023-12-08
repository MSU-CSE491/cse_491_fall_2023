/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @file BiomeGenerator.cpp
 * @author Paul Schulte, Milan Mihailovic (some code assisted with ChatGPT)
 */

#include "BiomeGenerator.hpp"
#include "Agents/AgentLibary.hpp"

#include <cmath>
#include <tuple>
#include <queue>

using namespace group6;
using namespace cse491;

using std::vector;

/**
 * Creates a generator with a grid of the given size and using the given seed
 * @param biome  The biome of the grid
 * @param width  The width of the grid
 * @param height The height of the grid
 * @param seed   The seed used for random number generation
 */
BiomeGenerator::BiomeGenerator(BiomeType biome, unsigned int width, unsigned int height, unsigned int seed) : biome(biome), width(width), height(height), seed(seed) {
    if (biome == BiomeType::Maze) {
        setTiles(floor_id, wall_id);
    } else if (biome == BiomeType::Grasslands) {
        setTiles(grass_id, dirt_id);
    }
    else if (biome == BiomeType::Ocean) {
        setTiles(water_id, sand_id);
    }

    perlinNoise = PerlinNoise(seed);
    grid.Resize(width, height);
}

void BiomeGenerator::setWorld(WorldBase *world) {
    worldPtr = world;
}

/**
 * Generates the grid with two types of tiles
 */
void BiomeGenerator::generate() {
    size_t tile1 = tiles[0];
    size_t tile2 = tiles[1];

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // Give 5x5 clear space in top left corner
            // TODO: Replace with putting player in valid room instead of 0,0
            if (x > 4 || y > 4) {
                const double val = perlinNoise.noise2D(x * frequency / width, y * frequency / height);
                grid.At(x, y) = val < 0 ? tile1 : tile2;
            }
        }
    }


    if (biome == BiomeType::Maze) {
        placeSpecialTiles(tile1, spike_id, 0.05); // Placing spike tiles
        placeSpecialTiles(tile1, tar_id, 0.08); // Placing tar tiles
        placeTileRandom(key_id, floor_id); // placing key tile

        vector<GridPosition> path = clearPath();
        applyPathToGrid(path);
        placeDoorTile(door_id); // placing door tile
        grid.At(keyLocation) = key_id;

    }

    if (biome == BiomeType::Grasslands) {
        placeTileRandom(hole_id, grass_id); // placing hole tile
    }

    if (biome == BiomeType::Ocean) {
        oceanHandler();
        placeTileRandom(hole_id, sand_id);
    }

}

/**
 * Generates random coordinates to place the given tile
 *
 * @param tile      The tile being placed
 * @param spawnTile The type of tile allowed to be replaced
 */
void BiomeGenerator::placeTileRandom(const size_t &tile, const size_t &spawnTile) {
    bool counter = false;
    while (!counter) {
        int random_x = (int)worldPtr->GetRandom(width / 2.0, width - 1);
        int random_y = (int)worldPtr->GetRandom(height / 2.0, height - 1);

        if (grid.At(random_x, random_y) == spawnTile) {
            grid.At(random_x, random_y) = tile;

            if (tile == key_id) {
                keyLocation = GridPosition(random_x, random_y);
            }

            counter = true;
        }
    }
}

/**
 * Generates door tile on grid at [1][1]
 * @param doorTile  Door Tile
 */
void BiomeGenerator::placeDoorTile(const size_t &doorTile) {
    grid.At(2, 2) = doorTile;
}

/**
 * Generates special tiles on the grid
 * @param genericTile  The tile that the special tile can spawn on
 * @param specialTile The special tile to generate
 * @param percentage Chance of special tile generating on the generic tile
 */
void BiomeGenerator::placeSpecialTiles(const size_t &genericTile, const size_t &specialTile, double percentage) {
    std::vector<std::pair<int, int>> floorPositions;
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            if (grid.At(x, y) == genericTile) {
                floorPositions.emplace_back(x, y);
            }
        }
    }

    int numSpikes = (int)round((int)floorPositions.size() * percentage);

    // Convert some generic floor tiles to special tiles
    for (int i = 0; i < numSpikes; ++i) {
        int pos = (int)round(worldPtr->GetRandom((int)floorPositions.size() - 1));
        grid.At(floorPositions [pos].first, floorPositions[pos].second) = specialTile;

        floorPositions.erase(floorPositions.begin() + pos);
    }
}

/**
 * Clears a randomized path from the top left of the
 * grid, to any point on the rightmost side of the map
 * @return A vector of GridPositions necessary for this path
 */
vector<GridPosition> BiomeGenerator::clearPath() const {
    vector<GridPosition> path;

    GridPosition current(0, 0); // Starting point
    path.push_back(current);

    // Continue until we reach the KeyLocation
    while (current != keyLocation) {
        std::vector<GridPosition> possibleMoves;

        // Always add right movement if not aligned horizontally
        if (current.GetX() < keyLocation.GetX()) {
            possibleMoves.push_back(current.ToRight());
        }

        // Add down movement if above the target and within grid bounds
        if (current.GetY() < keyLocation.GetY() && current.GetY() < height - 1) {
            possibleMoves.push_back(current.Below());
        }

        // Add up movement if below the target and within grid bounds
        if (current.GetY() > keyLocation.GetY() && current.GetY() > 0) {
            possibleMoves.push_back(current.Above());
        }

        // Randomly choose one of the possible moves
        if (!possibleMoves.empty()) {
            GridPosition next = possibleMoves[int(worldPtr->GetRandom(0, 2)) % possibleMoves.size()];

            // Check if we have made a valid move, if so, update the path and current position
            if (next != current) {
                path.push_back(next);
                current = next;
            }
        }
    }

    return path;
}



/**
 * Clears the walls out of the grid, guaranteeing a path from the
 * left of the grid, to any point on the rightmost side of the map
 * @param path A vector of GridPositions necessary for this path
 */
void BiomeGenerator::applyPathToGrid(const vector<GridPosition> &path) {
    for (const GridPosition &p: path) {
        grid.At(p) = floor_id;
    }
}

/**
 * Saves the grid to the given filepath
 * @param filename The filename the grid will be saved to
 */
void BiomeGenerator::saveToFile(const std::string &filename) const {
    type_options_t types = type_options_t();

    types.push_back(CellType{"floor", "Floor that you can easily walk over.", ' '});
    types.push_back(CellType{"wall", "Impenetrable wall that you must find a way around.", '#'});
    types.push_back(CellType{"spike", "Dangerous spike that resets the game.", 'X'});
    types.push_back(CellType{"tar", "Slow tile that makes you take two steps to get through it", 'O'});
    types.push_back(CellType{"key", "item that can be picked up to unlock door and escape maze", 'K'});
    types.push_back(CellType{"door", "Door that can be walked through only with possession of key to leave maze", 'D'});
    types.push_back(CellType{"grass", "Grass you can walk on.", 'M'});
    types.push_back(CellType{"dirt", "Dirt you can walk on.", '~'});
    types.push_back(CellType{"tree", "A tree that blocks the way.", 't'});
    types.push_back(CellType{"hole", "A hole that you can fall into the maze from.", '8'});

    types.push_back(CellType{"water","Water that you may be able to swim on.", 'W'});
    types.push_back(CellType{"sand", "Sand you can walk on.", '-'});


    grid.Write(filename, types);

}

/**
 * Sets the tile vector for the biome
 * @param firstTile Tile #1 for the biome
 * @param secondTile Tile #2 for the biome
 */
void BiomeGenerator::setTiles(const size_t &firstTile, const size_t &secondTile) {
    tiles.clear();
    tiles.push_back(firstTile);
    tiles.push_back(secondTile);
}

/**
 * Places trees on the grid
 */
void BiomeGenerator::placeTrees() {
    // Iterates through each tile in the grid with a margin of 1 tile to prevent out of bounds access
    for (unsigned int y = 1; y < height - 1; ++y) {
        for (unsigned int x = 1; x < width - 1; ++x) {
            // Only place trees on grass tiles and ensure we have space for a 3x3 tree
            if (grid.At(x, y) == grass_id &&
                    grid.At(x-1, y) == grass_id && grid.At(x+1, y) == grass_id &&
                    grid.At(x, y-1) == grass_id && grid.At(x, y+1) == grass_id &&
                    grid.At(x-1, y-1) == grass_id && grid.At(x+1, y-1) == grass_id &&
                    grid.At(x-1, y+1) == grass_id && grid.At(x+1, y+1) == grass_id) {
                // Use a random chance to place a tree
                if (worldPtr->GetRandom( 100) < 10) { // 10% chance to place a tree
                    // Place a 3x3 block of tree tile characters for the tree
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            grid.At(x + i, y + j) = tree_id;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Handles logic for Ocean biome
 */
void BiomeGenerator::oceanHandler(){
    for (unsigned int y = 1; y < height - 1; ++y) {
        for (unsigned int x = 1; x < width - 1; ++x) {
            if (grid.At(x, y) == water_id) {
                if (worldPtr->GetRandom(100) < 15) {
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            if (x + i > 0 && x + i < width - 1 && y + j > 0 && y + j < height - 1) {
                                grid.At(x + i, y + j) = sand_id;
                            }
                        }
                    }
                }
            }
        }
    }
}



