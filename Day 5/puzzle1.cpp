
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <regex>
#include <sstream>
#include <algorithm>

using namespace std;

void printVector(vector<string> vector) {
    cout << "[";
    for (string value : vector) {
        if (value == vector.back()){
            cout << value;
        } else {
            cout << value << ", ";
        }
    }
    cout << "]" << endl;
}

void printVector(vector<long long> vector) {
    cout << "[";
    for (int i = 0; i < vector.size(); i++) {
        if (i == vector.size() - 1){
            cout << vector[i];
        } else {
            cout << vector[i] << ", ";
        }
    }
    cout << "]" << endl;
}

vector<string> getValues(string allValues) {

    vector<string> vectorValues;

    stringstream values(allValues);

    while (values.good()) {
        string substr;
        getline(values, substr, ' ');
        vectorValues.push_back(substr);
    }

    return vectorValues;

}

long long locateDestination(long long index, vector<vector<long long>> currentSection) {

    // basic breakdown of this monstrosity:
        // the key in the vector is the source value
        // the second map which is the value is contains the range and destination
        // I image it looking something like this
        // [{'source': [{'destination': x}, {'range': y}],...]
        // That way I can sort by source and stil get a hold of the range and destination
    sort(currentSection.begin(), currentSection.end(), [](const vector<long long>& a, const vector<long long>& b) {
        return a[0] < b[0];
    });

    long long indexOfNearestSourceBelow = 0;
    if (index < currentSection[0][0]) {
        return index;
    }
    else {
        indexOfNearestSourceBelow = currentSection[0][0];
    }

    for (int i = 0; i < currentSection.size(); i++) {
        if (index >= currentSection[i][0]) {
            indexOfNearestSourceBelow = i;
        }
    }


    long long nearestSourceBelow = currentSection[indexOfNearestSourceBelow][0];
    long long rangeOfNearestSourceBelow = currentSection[indexOfNearestSourceBelow][2];

    if (index < nearestSourceBelow + rangeOfNearestSourceBelow) {

        long long difference = index - nearestSourceBelow;
        long long destination = currentSection[indexOfNearestSourceBelow][1] + difference;

        return destination;
    }
    else {
        return index;
    }

}

long long findLocation(string seed, vector<vector<vector<long long>>> allMaps){

    vector<vector<long long>> currentSection;
    long long nextIndex = stoll(seed);


    for (int i = 0; i < allMaps.size(); i++){
        currentSection = allMaps[i];
        nextIndex = locateDestination(nextIndex, currentSection);
    }
    
    return nextIndex;
}


long long findMinimumLocation(vector<long long> locations) {

    long long minimum = locations[0];

    for (int i = 1; i < locations.size(); i++) {
        if (locations[i] < minimum) {
            minimum = locations[i];
        }
    }

    return minimum;

}

vector<string> findAllSeeds(vector<string> seeds) {

    vector<string> allSeeds;

    for (int i = 0; i < seeds.size(); i += 2) {
        long long seedStart = stoll(seeds[i]);
        long long seedRange = stoll(seeds[i + 1]);

        for (long long j = seedStart; j < seedStart + seedRange; j++) {
            if (find(allSeeds.begin(), allSeeds.end(), to_string(j)) == allSeeds.end()) {
                allSeeds.push_back(to_string(j));
            }
        }
    }

    return allSeeds;

}

int main() {

    ifstream file("testing.txt");

    if(!file.is_open()){
        cout << "Error opening file" << endl;
        return 0;
    }

    string line;    

    // Set to negative 1 so that the first line can offset it to proper position (seed-to-soil-map should be index 1)
    long long mapNumber = -1;
    vector<string> seeds;
    vector<vector<long long>> currentSection;
    vector<vector<vector<long long>>> allSections;

    while (getline(file, line)) {

        cout << line << endl;

        if (mapNumber == -1) {
            string allSeeds = line.substr(7, line.length() - 7);
            seeds = getValues(allSeeds);
        }
        
        const regex number_regex("[0-9]");
        const regex letter_regex("[a-zA-Z]");
        const string character(1, line[0]);

        if (regex_match(character, number_regex)) {
            
            vector<string> mapDetails = getValues(line);

            vector<long long> rowInSection = {
                stoll(mapDetails[1]),
                stoll(mapDetails[0]),
                stoll(mapDetails[2])
            };

            currentSection.push_back(rowInSection);

        } 
        else if (regex_match(character, letter_regex)) {
            mapNumber++;
            if (!currentSection.empty()) {
                allSections.push_back(currentSection);
                currentSection.clear();
            }
        } 

    }

    // Push the last map into the vector that was waiting for a heading that never came
    allSections.push_back(currentSection);


    cout << "File Scanned" << endl << endl;

    vector<long long> locations;

    for (string seed : seeds) {
        long long location = findLocation(seed, allSections);
        locations.push_back(location);
    }

    printVector(locations);
    cout << "There are " << locations.size() << " locations" << endl;
    cout << "Minimum location is: " << findMinimumLocation(locations) << endl;


    // ========== testing ==========

        

    file.close();
    return 0;

}