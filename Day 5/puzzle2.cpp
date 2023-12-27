#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

using namespace std;
using ll = long long;

struct Map {
    ll destination;
    ll source;
    ll range;
};

class Range {

    public: 
        ll startPosition;
        ll endPosition;
        ll length;

        bool operator==(Range other) {
            return this->endPosition == other.endPosition && this->startPosition == other.endPosition; 
        }
};

void printRange(Range range) {
    std::cout << "{Start: " << range.startPosition << ", End: " << range.endPosition << ", Length: " << range.length << "}" << endl;
}

void printMap(Map map) {
    std::cout << "{Source: " << map.source << ", Destination: " << map.destination << ", Range: " << map.range << "}" << endl;
}

void printVector(vector<Map> vector) {
    std::cout << "[" << endl;
    for (Map value : vector) {
        printMap(value);
    }
    std::cout << "]" << endl;
}

void printVector(vector<Range> vector) {
    std::cout << "[" << endl;
    for (Range value : vector) {
        printRange(value);
    }
    std::cout << "]" << endl;
}

void printVector(vector<string> vector) {
    std::cout << "[";
    for (string value : vector) {
        if (value == vector.back()){
            std::cout << value;
        } else {
            std::cout << value << ", ";
        }
    }
    std::cout << "]" << endl;
}

void printVector(vector<ll> vector) {
    std::cout << "[";
    for (int i = 0; i < vector.size(); i++) {
        if (i == vector.size() - 1){
            std::cout << vector[i];
        } else {
            std::cout << vector[i] << ", ";
        }
    }
    std::cout << "]" << endl;
}

vector<Range> convertSeedsToRanges(vector<ll> seeds) {

    vector<Range> ranges;

    for (int i = 0; i < seeds.size(); i += 2) {
        Range range;
        range.startPosition = seeds[i];
        range.endPosition = range.startPosition + seeds[i + 1] - 1;
        range.length = range.endPosition - range.startPosition + 1;
        ranges.push_back(range);
    }

    return ranges;

}

vector<Range> convertSectionToRanges(vector<Map> section) {

    vector<Range> ranges;

    for (Map map : section) {
        Range range;
        range.startPosition = map.source;
        range.endPosition = map.source + map.range - 1;
        range.length = map.range;
        ranges.push_back(range);
    }

    sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
        return a.startPosition < b.startPosition;
    });

    return ranges;

}

bool vectorContains(vector<Range> vector, Range value) {

    for (Range element : vector) {
        if (element == value) {
            return true;
        }
    }

    return false;

}

vector<Range> mapNewSeeds(vector<Map> sectionMap, vector<Range> seedsInRow) {

    vector<Range> mappedSeeds;

    for (Range seedRange : seedsInRow) {

        // cout << "Seed Range: " << endl;
        // printRange(seedRange);

        Range newSeedRange;

        Map correspondingMap;

        for (Map map : sectionMap) {

            if (seedRange.startPosition >= map.source && seedRange.startPosition < map.range + map.source + 1){
                correspondingMap = map;
            }

        }

        // printMap(correspondingMap);

        ll offset = seedRange.startPosition - correspondingMap.source;
        newSeedRange.startPosition = correspondingMap.destination + offset;
        newSeedRange.endPosition = newSeedRange.startPosition + seedRange.length - 1;
        newSeedRange.length = seedRange.length;

        // printRange(newSeedRange);

        mappedSeeds.push_back(newSeedRange);

    }

    return mappedSeeds;

}

vector<Range> findSeeds(Range currentSection, vector<Range> currentSeeds) {

    vector<Range> seedsInRow;

        for (Range seedRange : currentSeeds) {

            if (seedRange.endPosition < currentSection.startPosition || seedRange.startPosition > currentSection.endPosition) {
                continue;
            }

            // If true, then set the start to be the beginning of the seed, otherwise set it to be the beginning of the section
            ll newSeedStart = seedRange.startPosition >= currentSection.startPosition ? seedRange.startPosition : currentSection.startPosition;
            // Same but for the end
            ll newSeedEnd = seedRange.endPosition <= currentSection.endPosition ? seedRange.endPosition : currentSection.endPosition;

            seedsInRow.push_back({newSeedStart, newSeedEnd, newSeedEnd - newSeedStart + 1});

        }

    return seedsInRow;

}

vector<Range> adjustSeeds(vector<Range> currentSeeds, vector<Range> mappedSeedsInRow) {

    vector<Range> adjustedSeeds;

    for (Range seed : currentSeeds) {

        // cout << "Seed: ";
        // printRange(seed);

        vector<Range> trimmedSeeds;
        bool trimmed = false;

        for (Range mappedSeed : mappedSeedsInRow) {

            // cout << "Mapped Seed: ";
            // printRange(mappedSeed);

            bool trimBottom = mappedSeed.startPosition >= seed.startPosition && mappedSeed.endPosition <= seed.endPosition;
            bool trimTop = mappedSeed.endPosition >= seed.startPosition && mappedSeed.endPosition <= seed.endPosition;
            trimmed = trimBottom || trimTop;

            if (mappedSeed.startPosition == seed.startPosition && mappedSeed.endPosition == seed.endPosition) {
                // cout << "Mapped seed is the same as the seed, so we don't need to trim it" << endl;
                trimmed = true;
                break;
            }

            if (trimTop) {

                Range trimmedSeed = {
                    seed.startPosition,
                    mappedSeed.startPosition - 1,
                    mappedSeed.startPosition - seed.startPosition
                };

                if (vectorContains(mappedSeedsInRow, trimmedSeed)) {
                    // cout << "Trimmed seed (top) is already in the mapped seeds, so we don't need to add it" << endl;
                    
                }
                else {
                    trimmedSeeds.push_back(trimmedSeed);
                    printVector(trimmedSeeds);
                }

            }
            if (trimBottom) {

                Range trimmedSeed = {
                    mappedSeed.startPosition,
                    seed.endPosition,
                    seed.endPosition - mappedSeed.startPosition
                };

                if (mappedSeed.startPosition == trimmedSeed.startPosition && mappedSeed.endPosition == trimmedSeed.endPosition) {
                    // cout << "Trimmed seed (bottom) is already in the mapped seeds, so we don't need to add it" << endl;
                    
                }
                else {
                    trimmedSeeds.push_back(trimmedSeed);
                    printVector(trimmedSeeds);
                }

            }

        }

        if (trimmed){
            for (Range trimmedSeed : trimmedSeeds) {
                adjustedSeeds.push_back(trimmedSeed);
            }
        } else {
            adjustedSeeds.push_back(seed);
        
        }

    }

    cout << endl << endl;

    return adjustedSeeds;

}

int main() {

    ifstream file("testing.txt");

    if (!file.is_open()) {
        std::cout << "File not found" << endl;
        return 0;
    }

    string line;
    int lineIdx = -1;
    vector<ll> seeds;
    vector<vector<Map>> almanac;
    vector<vector<Range>> almanacRanges;
    vector<Map> currentSection;

    while (getline(file, line)) {
        std::cout << line << endl;

        if (lineIdx == -1) {
            stringstream ss(line.substr(7, line.size() - 7));
            ll num;
            while (ss >> num){
                seeds.push_back(num);
            }
            lineIdx++;
            continue;
        }

        const regex letter_regex("[a-zA-Z]");
        const regex number_regex("[0-9]");
        const string character(1, line[0]);

        if (regex_match(character, letter_regex)) {
            lineIdx++;

            sort(currentSection.begin(), currentSection.end(), [](Map a, Map b) {
                return a.source < b.source;
            });
            
            if (lineIdx != 1) {
                almanac.push_back(currentSection);    
            }
            currentSection.clear();
        }
        else if (regex_match(character, number_regex)) {
            stringstream ss(line);
            Map rowMap;
            ss >> rowMap.destination >> rowMap.source >> rowMap.range;
            currentSection.push_back(rowMap);
        }

    }

    almanac.push_back(currentSection);
    std::cout << "Finished Reading File" << endl;

    vector<Range> seedRanges = convertSeedsToRanges(seeds);
        
    for (vector<Map> section : almanac) {
        vector<Range> sectionAsRanges = convertSectionToRanges(section);
        almanacRanges.push_back(sectionAsRanges);
    }

    vector<Range> currentSeeds = seedRanges;

    for (int i = 0; i < almanacRanges.size(); i++) {

        vector<Range> currentSection = almanacRanges[i];
        vector<Map> currentSectionMap = almanac[i];

        vector<Range> mappedSeeds;
        vector<Range> seedsInRow;

        for (int i = 0; i < currentSection.size(); i++) {

            vector<Range> seedsFoundForSection = findSeeds(currentSection[i], currentSeeds);
            
            for (Range seed : seedsFoundForSection) {
                seedsInRow.push_back(seed);
            }

            vector<Range> mappedSeedsInRow = mapNewSeeds(currentSectionMap, seedsInRow);
            for (Range mappedSeed : mappedSeedsInRow) {
                mappedSeeds.push_back(mappedSeed);
            }

            vector<Range> adjustedSeeds = adjustSeeds(currentSeeds, seedsInRow);

            currentSeeds.clear();
            for (Range seed : adjustedSeeds) {
                currentSeeds.push_back(seed);
            }
            for (Range seed : mappedSeeds) {
                currentSeeds.push_back(seed);
            }

            printVector(currentSeeds);
        }

    }

    printVector(currentSeeds);
    // ==================== Testing ====================

    // vector<Range> testRange = almanacRanges[1];
    // vector<Map> testSectionMap = almanac[1];

    // vector<Range> currentSeeds = seedRanges;
    // // vector<Range> currentSeeds = {
    // //     {79, 92, 14},
    // //     {55, 67, 13},
    // //     {40, 55, 19}
    // // };
    // currentSeeds = {
    //     {81, 94, 14},
    //     {57, 69, 13},
    // };

    // vector<vector<Range>> testRun = {testRange};
    // vector<vector<Map>> testRunMap = {testSectionMap};
    // for (int i = 0; i < testRun.size(); i++) {

    //     vector<Range> currentSection = testRun[i];
    //     vector<Map> currentSectionMap = testRunMap[i];

    //     vector<Range> mappedSeeds;
    //     vector<Range> seedsInRow;

    //     for (int i = 0; i < currentSection.size(); i++) {

    //         vector<Range> seedsFoundForSection = findSeeds(currentSection[i], currentSeeds);
            
    //         for (Range seed : seedsFoundForSection) {
    //             seedsInRow.push_back(seed);
    //         }
            

    //     }

    //     vector<Range> mappedSeedsInRow = mapNewSeeds(currentSectionMap, seedsInRow);
    //     for (Range mappedSeed : mappedSeedsInRow) {
    //         mappedSeeds.push_back(mappedSeed);
    //     }
    //     printVector(currentSeeds);
    //     printVector(seedsInRow);
    //     printVector(mappedSeeds);


    //     vector<Range> adjustedSeeds = adjustSeeds(currentSeeds, seedsInRow);
    //     printVector(adjustedSeeds);

    //     currentSeeds.clear();
    //     for (Range seed : adjustedSeeds) {
    //         currentSeeds.push_back(seed);
    //     }
    //     for (Range seed : mappedSeeds) {
    //         currentSeeds.push_back(seed);
    //     }

    //     printVector(currentSeeds);

        /*
        *
        * Work on the adjustSeeds function when you get back. Or test the find seeds for sections other than the first
        *
        * 
        * */

    // }    

    // vector<Range> mappedSeedsInRow = {
    //     {79, 92, 14},
    //     {55, 67, 13},
    //     {50, 54, 5},
    // };


    // vector<Range> adjustedSeeds = adjustSeeds(currentSeeds, mappedSeedsInRow);
    // printVector(adjustedSeeds);

    // printVector(currentSeeds);
    
    file.close();
    return 0;
}