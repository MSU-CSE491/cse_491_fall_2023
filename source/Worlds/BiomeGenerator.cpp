/**
 * @file BiomeGenerator.cpp
 * @author Paul Schulte, Milan Mihailovic, ChatGPT
 */

#include <fstream>
#include <cmath>
#include <set>
#include <tuple>
#include <random>
#include "BiomeGenerator.hpp"

using std::vector;

/**
 * Creates a generator with a grid of the given size and using the given seed
 * @param biome  The biome of the grid
 * @param width  The width of the grid
 * @param height The height of the grid
 * @param seed   The seed used for random number generation
 */
BiomeGenerator::BiomeGenerator(BiomeType biome, unsigned int width, unsigned int height, unsigned int seed) : biome(biome), width(width), height(height) {

    if (biome == BiomeType::Maze)
    {
        setTiles(' ', '#');
    }
    else if (biome == BiomeType::Grasslands)
    {
        setTiles('M', '~');
    }

    perlinNoise = PerlinNoise(seed);
    grid = vector<vector<char>>(height, vector<char>(width));
}

/**
 * Generates the grid with two types of tiles
 */
void BiomeGenerator::generate() {

    char tile1 = tiles[0];
    char tile2 = tiles[1];

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            const double val = perlinNoise.noise2D(x * frequency / width, y * frequency / height);
            grid.at(y).at(x) = val < 0 ? tile1 : tile2;
        }
    }


    if (biome == BiomeType::Maze)
    {
        placeSpecialTiles(tile1, 'X', 0.02); // Placing spike tiles
        placeSpecialTiles(tile1, 'O', 0.05); // Placing tar tiles
        placeDoorTile('D'); // placing door tile
        placeKeyTile('K'); // placing key tile
    }
}

/**
 * Generates random coordinate to place Key tile
 * @param keyTile  Door Tile
 */
 void BiomeGenerator::placeKeyTile(const char &keyTile)
{
     bool counter = false;
     while( counter == false )
     {
         std::random_device rd;
         std::mt19937 gen(rd());

         std::uniform_int_distribution<int> x_distribution(width/2, width-1);
         std::uniform_int_distribution<int> y_distribution(height/2, height-1);
         int random_x = x_distribution(gen);
         int random_y = y_distribution(gen);

         if( grid[random_y][random_x] == ' ' )
         {
             grid[random_y][random_x] = keyTile;
             counter = true;
         }
     }
}

/**
 * Generates door tile on grid at [0][0]
 * @param doorTile  Door Tile
 */
void BiomeGenerator::placeDoorTile(const char &doorTile)
{
    grid[1][1] = doorTile;
}

/**
 * Generates special tiles on the grid
 * @param genericTile  The tile that the special tile can spawn on
 * @param specialTile The special tile to generate
 * @param percentage Chance of special tile generating on the generic tile
 */
void BiomeGenerator::placeSpecialTiles(const char &genericTile, const char &specialTile, double percentage) {
    std::vector<std::pair<int, int>> floorPositions;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j] == genericTile) {
                floorPositions.push_back({j, i});
            }
        }
    }

    int numSpikes = floorPositions.size() * percentage;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(floorPositions.begin(), floorPositions.end(), g);

    // Convert some generic floor tiles to special tiles
    for (int i = 0; i < numSpikes; ++i) {
        grid[floorPositions[i].second][floorPositions[i].first] = specialTile;
    }
}

/**
 * Clears a randomized path from the top left of the
 * grid, to any point on the rightmost side of the map
 * @return A vector of points necessary for this path
 */
std::vector<Point> BiomeGenerator::clearPath() const {
    std::vector<Point> path;

    Point current(0, 0);
    path.push_back(current);

    while (current.x < width - 1) {
        int randDirection = rand() % 3; // 0: Right, 1: Up, 2: Down

        // Choose next point based on random direction
        Point next = current;
        if (randDirection == 0)
        {
            next.x++;
        }

        else if (randDirection == 1)
        {
            if (next.y > 0) // Ensure within grid bounds
                next.y--;
        }
        else {

            if (next.y < height - 1) // Ensure within grid bounds
                next.y++;
        }

        // If the next point is the same as the current, then we chose an invalid direction
        // (like trying to go up at the top edge), so just skip this iteration.
        if (next != current) {
            path.push_back(next);
            current = next;
        }
    }

    return path;
}

/**
 * Clears the walls out of the grid, guaranteeing a path from the
 * left of the grid, to any point on the rightmost side of the map
 * @param path A vector of points necessary for this path
 */
void BiomeGenerator::applyPathToGrid(const std::vector<Point>& path) {
    for (const Point& p : path) {
        grid[p.y][p.x] = ' ';
    }
}

/**
 * Saves the grid to the given filepath
 * @param filename The filename the grid will be saved to
 */
void BiomeGenerator::saveToFile(const std::string &filename) const {
    std::ofstream out(filename);
    for (const auto &row : grid) {
        for (const auto &cell : row) {
            out << cell;
        }
        out << "\n";
    }
    out.close();
}

/**
 * Sets the tile vector for the biome
 * @param firstTile Tile #1 for the biome
 * @param secondTile Tile #2 for the biome
 */
void BiomeGenerator::setTiles(const char& firstTile, const char& secondTile)
{
    tiles.clear();
    tiles.push_back(firstTile);
    tiles.push_back(secondTile);
}

