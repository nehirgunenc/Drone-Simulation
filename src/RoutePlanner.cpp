#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};


// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces

    map.loadDistanceData(distance_data);
    map.resetVisited();
    
    loadPriorityProvinces(priority_data);

    loadWeatherRestrictedProvinces(restricted_data);

    
    
    displayResults();
    
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    // TODO: Your code here
    
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    std::string line;
    numPriorityProvinces=0;

    while (std::getline(file, line) && numPriorityProvinces<MAX_PRIORITY_PROVINCES) {
      std::istringstream linestream(line);
      std::string city;
      std::getline(linestream, city, '(');

        std::string indexStr;
        std::getline(linestream, indexStr, ')');

        int index = std::stoi(indexStr); 
        priorityProvinces[numPriorityProvinces++]=index;
        
    }

    file.close();

    
    

}

    

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    // TODO: Your code here
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }

    std::string line;
    numWeatherRestrictedProvinces = 0;  

    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {  
        std::istringstream linestream(line);
        std::string city;
        std::getline(linestream, city, '(');

        std::string indexStr;
        std::getline(linestream, indexStr, ')');

        int index = std::stoi(indexStr); 
        weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = index;  
    }

    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for(int p:priorityProvinces){
        if(p==province){
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for(int wp:weatherRestrictedProvinces){
        if(wp==province){
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    stack.push(startingCity);
    map.markAsVisited(startingCity);
    route.push_back(startingCity);

    while (!isExplorationComplete()) {
        int currentProvince = stack.peek();  

        // Add neighboring provinces to the queue
        enqueueNeighbors(currentProvince);


        bool moved = false;

        if (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();

            // Perform operations if it is an unvisited and valid province
            if (!map.isVisited(nextProvince)) {
                stack.push(nextProvince);
                map.markAsVisited(nextProvince);
                route.push_back(nextProvince);
                moved = true;

                exploreFromProvince(nextProvince); 
                
                break;
            }
        } 

        if (!moved) {
           backtrack();
        }
    }
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here 
    
    enqueueNeighbors(province);

    while (!queue.isEmpty()) {
        int nextProvince = queue.dequeue();

        if (!map.isVisited(nextProvince) && !isWeatherRestricted(nextProvince)) {
            stack.push(nextProvince);
            map.markAsVisited(nextProvince);
            route.push_back(nextProvince);
            exploreFromProvince(nextProvince); 
        }
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < 81; ++i) {
        if (map.distanceMatrix[province][i] <= maxDistance && !map.isVisited(i)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);  
            } else {
                queue.enqueue(i); 
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province 
    // TODO: Your code here
      if (!stack.isEmpty()) {
        stack.pop(); 
        route.pop_back();
    }
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
     
    if (stack.isEmpty() && queue.isEmpty()) {
        return true;
    }

    
    if (!stack.isEmpty()) {
        int currentProvince = stack.peek();  
        
        bool hasUnvisitedNeighbor = false;
        for (int i = 0; i < 81; ++i) {
            if (map.distanceMatrix[currentProvince][i] <= maxDistance && 
                !map.isVisited(i) && !isWeatherRestricted(i)) {
                hasUnvisitedNeighbor = true;
                break;
            }
        }

        // If there are no neighbors left to visit in the current province and the return trip is over
        if (!hasUnvisitedNeighbor && stack.getSize() == 1) {
            return true;  
        }
    }

    
    int visitedPriorityCount = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (map.isVisited(priorityProvinces[i])) {
            visitedPriorityCount++;
        }
    }
    if (visitedPriorityCount == numPriorityProvinces) {
        return true;
    }

    
    return false;
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary



    for(int j=0; j<numWeatherRestrictedProvinces; ++j){
        if (!map.isVisited(weatherRestrictedProvinces[j])) {
            int cityNum= weatherRestrictedProvinces[j];
            std::cout<<"Province "<<cities[cityNum]<<" is weather-restricted. Skipping."<<std::endl; 
        }   
    }
    
    
    std::cout<<"----------------------------\n"
    <<"Journey Completed!\n"
    <<"----------------------------\n";

    
    std::cout << "\nTotal Number of Provinces Visited: " << route.size();

    std::cout << "\nTotal Distance Covered: " << totalDistanceCovered << " km\n";

    
    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]]; 
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    

    
    int priorityVisitedCount = 0;
    std::cout << "\nPriority Province Status:\n";
    for (int i = 0; i < numPriorityProvinces; ++i) {
        bool visited = map.isVisited(priorityProvinces[i]);
        std::cout <<"-" <<cities[priorityProvinces[i]]
                  << (visited ? " (Visited)" : " (Not Visited)") << "\n";
        if (visited) {
            ++priorityVisitedCount;
        }
    }

    std::cout<<"\nTotal Priority Provinces Visited: "<< priorityVisitedCount<< " out of "<< numPriorityProvinces;
   
    if (priorityVisitedCount == numPriorityProvinces) {
        std::cout << "\nAll priority provinces were successfully visited!\n";
    } else {
        std::cout << "\nWarning:  Not all priority provinces were visited.\n";
    }

    
}


