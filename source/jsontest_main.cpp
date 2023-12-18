#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

int main() {
    
    std::string inputFile = "../assets/input.json";

    // Open file
    std::ifstream file(inputFile);

    // Check if open
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    // Parse JSON data
    nlohmann::json data;
    try {
        file >> data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    // Close file
    //file.close();

    // Iterate throughout 
    for (const auto& object : data) {
        std::cout << "Name: " << object["name"] << std::endl;
        std::cout << "x: " << object["x"] << std::endl;
        std::cout << "y: " << object["y"] << std::endl;
	//agent properties
        std::cout << std::endl;
    }

    return 0;
}