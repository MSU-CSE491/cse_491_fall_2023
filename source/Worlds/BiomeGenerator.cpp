/**
 * @file BiomeGenerator.cpp
 * @author Paul Schulte
 */

#include <fstream>
<<<<<<< HEAD
#include <cmath>
#include <set>
#include <tuple>
=======
#include <random>
>>>>>>> 627df6f (Added Key and Door tile with functionality)
#include "BiomeGenerator.h"

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
 * @param tile1  The first tile generated
 * @param tile2 The second tile generated
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

//bool BiomeGenerator::isPathExists() {
//    std::set<Point> closedSet;
//    std::vector<Point> openSet{ {0, 0} };
//
//    auto heuristic = [](const Point& a, const Point& b) {
//        return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
//    };
//
//    while (!openSet.empty()) {
//        Point current = openSet.back();
//        openSet.pop_back();
//
//        if (current.x == width - 1 && current.y == height - 1)
//            return true;
//
//        closedSet.insert(current);
//
//        std::vector<Point> neighbors{
//                {current.x + 1, current.y},
//                {current.x - 1, current.y},
//                {current.x, current.y + 1},
//                {current.x, current.y - 1}
//        };
//
//        for (const auto& neighbor : neighbors) {
//            if (neighbor.x < 0 || neighbor.x >= width || neighbor.y < 0 || neighbor.y >= height)
//                continue;
//
//            if (grid[neighbor.y][neighbor.x] == '#')  // Check if the neighbor is walkable
//                continue;
//
//            if (closedSet.find(neighbor) != closedSet.end())
//                continue;
//
//            double tentative_gScore = heuristic({0, 0}, neighbor);
//
//            auto it = std::find(openSet.begin(), openSet.end(), neighbor);
//            if (it == openSet.end() || tentative_gScore < heuristic({0, 0}, *it)) {
//                if (it == openSet.end()) {
//                    openSet.push_back(neighbor);
//                }
//            }
//        }
//    }
//
//    return false;
//}


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

         std::uniform_int_distribution<int> distribution(height/2, height-1);
         auto random_x = distribution(gen);
         auto random_y = distribution(gen);

         if( grid[random_x][random_y] == ' ' )
         {
             grid[random_x][random_y] = keyTile;
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
    grid[0][0] = doorTile;
}
/**
 * Generates special tiles on the grid
 * @param genericTile  The tile that the special tile can spawn on
 * @param specialTile The special tile to generate
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

