#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>



Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    // Initialize all provinces as unvisited
   
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    std::ifstream file(filename);

    if (!file.is_open()) {
      std::cerr << "Error opening file " << filename << std::endl;
      return;
    }

    for (int i = 0; i < MAX_SIZE; ++i) {
        std::string line;
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            for (int j = 0; j < MAX_SIZE; ++j) {
                std::string cell;
                if (std::getline(ss, cell, ',')) {
                    distanceMatrix[i][j] = std::stoi(cell);
                    
                }
            }
        }
    }
    file.close();

}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    if (provinceA >= 0 && provinceA < MAX_SIZE && provinceB >= 0 && provinceB < MAX_SIZE) {
        if (distanceMatrix[provinceA][provinceB] <= maxDistance) {
            return true;
        }
    }
    return false;

}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province]=true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    if(visited[province]==true){
        return true;
    }
    return false;
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (size_t i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {
            ++count;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}