#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

class gear {

    public:
        int value;
        int lineIdx;
        int startIdx;
        int endIdx;

        bool operator==(const gear& g) const {
            
            bool sameValue = this->value == g.value;
            bool sameStartIdx = this->startIdx == g.startIdx;
            bool sameLineIdx = this->lineIdx == g.lineIdx;

            return sameValue && sameStartIdx && sameLineIdx;
        }
};

struct searchResult {
    bool found;
    vector<gear> gearsFound;
};

gear findGearValue(string line, int columnIdx, int rowIdx) {

    gear g;
    g.lineIdx = rowIdx;

    // Iterate back through the line until we find the first non-number character. This will be the beginning of the part number.
    for (int i = columnIdx; i >= 0; i--) {

        const regex basic_regex("[0-9]");
        const string character(1, line[i]);


        if (i == 0 && regex_match(string(1, line[i]), basic_regex)){
            g.startIdx = 0;
            break;
        }

        if (!regex_match(character, basic_regex)) {
            g.startIdx = i + 1;
            break;
        }
    }

    // Iterate forward through the line until we find the first non-number character. This will be the end of the part number.
    string partNumber = "";

    for (int i = g.startIdx; i < line.length(); i++) {
        const regex basic_regex("[0-9]");
        const string character(1, line[i]);

        if (regex_match(character, basic_regex)) {
            partNumber += character;
        }
        else {
            g.endIdx = i - 1;
            break;
        }
    }

    g.value = stoi(partNumber);
    return g;

}

searchResult searchAboveAndDiagonally(string previousLine, int columnIdx, int rowIdx) {

    // cout << "\t\t" << previousLine << endl;

    searchResult gearFinder;
    gearFinder.found = false;

    vector<gear> gearsFound;

    if (previousLine == "") {
        return gearFinder;
    }

    int startIdx;
    if (columnIdx == 0) {
        startIdx = 0;
    } else {
        startIdx = columnIdx - 1;
    }

    for (int i = startIdx; i < startIdx + 3 && i < previousLine.length(); i++) {
        
        const regex basic_regex("[0-9]");
        const string character(1, previousLine[i]);

        // cout << "Character: " << character << endl;

        // If the character is a number, find the rest of the part number, and if there are no other gears with the same part number, add it to the gearsFound vector.
        if (regex_match(character, basic_regex)) {

            gear g = findGearValue(previousLine, i, rowIdx - 1);

            // Check to make sure that the gear is not already in the gearsFound vector.
            if (find(gearsFound.begin(), gearsFound.end(), g) == gearsFound.end()) {
                gearsFound.push_back(g);
                gearFinder.found = true;

            }
        }
    }

    gearFinder.gearsFound = gearsFound;
    return gearFinder;

}

searchResult searchBelowAndDiagonally(string nextLine, int columnIdx, int rowIdx) {

    searchResult gearFinder;
    gearFinder.found = false;

    vector<gear> gearsFound;

    if (nextLine == "") {
        return gearFinder;
    }

    int startIdx;
    if (columnIdx == 0) {
        startIdx = 0;
    } else {
        startIdx = columnIdx - 1;
    }

    for (int i = startIdx; i < startIdx + 3 && i < nextLine.length(); i++) {
        
        const regex basic_regex("[0-9]");
        const string character(1, nextLine[i]);

        // If the character is a number, find the rest of the part number, and if there are no other gears with the same part number, add it to the gearsFound vector.
        if (regex_match(character, basic_regex)) {

            gear g = findGearValue(nextLine, i, rowIdx + 1);
            
            // Check to make sure that the gear is not already in the gearsFound vector.
            if (find(gearsFound.begin(), gearsFound.end(), g) == gearsFound.end()) {
                gearsFound.push_back(g);
                gearFinder.found = true;

            }
        }
    }

    gearFinder.gearsFound = gearsFound;
    return gearFinder;

}

searchResult searchLeft(string currentLine, int columnIdx, int rowIdx) {

    searchResult gearFinder;
    gearFinder.found = false;

    if (columnIdx == 0) {
        return gearFinder;
    }

    const regex basic_regex("[0-9]");
    const string character(1, currentLine[columnIdx - 1]);

    if (regex_match(character, basic_regex)) {
        gear g = findGearValue(currentLine, columnIdx - 1, rowIdx);
        gearFinder.found = true;
        gearFinder.gearsFound.push_back(g);
    }

    return gearFinder;

}

searchResult searchRight(string currentLine, int columnIdx, int rowIdx) {

    searchResult gearFinder;
    gearFinder.found = false;

    if (columnIdx == currentLine.length() - 1) {
        return gearFinder;
    }

    const regex basic_regex("[0-9]");
    const string character(1, currentLine[columnIdx + 1]);

    if (regex_match(character, basic_regex)) {
        gear g = findGearValue(currentLine, columnIdx + 1, rowIdx);
        gearFinder.found = true;
        gearFinder.gearsFound.push_back(g);
    }

    return gearFinder;

}

searchResult lookForGears(string currentLine, string previousLine, string nextLine, int columnIdx, int rowIdx) {

    // cout << "\t\t\t" << previousLine << endl;
    // cout << "\t\t\t" << currentLine << endl;
    // cout << "\t\t\t" << nextLine << endl;

    searchResult gearFinder;
    gearFinder.found = false;

    vector<gear> gearsFound;

    // Search above and on the diagonals of the previous line. If any gears are found, add them to the gearsFound vector.

    searchResult aboveResult = searchAboveAndDiagonally(previousLine, columnIdx, rowIdx);

    if (aboveResult.found) {
        for (gear g : aboveResult.gearsFound) {
            gearsFound.push_back(g);
        }
    }

    // Search below and on the diagonals of the next line. If any gears are found, add them to the gearsFound vector.

    searchResult belowResult = searchBelowAndDiagonally(nextLine, columnIdx, rowIdx);

    if (belowResult.found) {
        for (gear g : belowResult.gearsFound) {
            gearsFound.push_back(g);
        }
    }

    // Search left and right of the current line. If any gears are found, add them to the gearsFound vector.
    searchResult rightResult = searchRight(currentLine, columnIdx, rowIdx);

    if (rightResult.found) {
        for (gear g : rightResult.gearsFound) {
            gearsFound.push_back(g);
        }
    }
    
    searchResult leftResult = searchLeft(currentLine, columnIdx, rowIdx);

    if (leftResult.found) {
        for (gear g : leftResult.gearsFound) {
            gearsFound.push_back(g);
        }
    }

    for(gear g : gearsFound) {
        // cout << "Found gear with value: " << g.value << endl;
    }

    // If there are only two parts, then it is a gear, and we return the gearsFound vector.
    if (gearsFound.size() == 2) {
        gearFinder.found = true;
        gearFinder.gearsFound = gearsFound;
    }

    return gearFinder;
}

int main() {

    fstream file("partNumbers.txt");

    string previousLine = "";
    string currentLine;
    file >> currentLine;

    string nextLine;
    file >> nextLine;

    vector<gear> gearsFound;
    int gearRatioSum = 0;

    int rowIdx = 0;

    while (previousLine != currentLine) {

        cout << currentLine;

        for (int i = 0; i < currentLine.length(); i++) {
            
            if (currentLine[i] == '*'){
                searchResult result = lookForGears(currentLine, previousLine, nextLine, i, rowIdx);
                
                if (result.found) {
                    
                    int gearRatio = result.gearsFound[0].value * result.gearsFound[1].value;
                    cout << "\t Found gear with ratio:" << gearRatio;
                    gearRatioSum += gearRatio;
                }
            }
        }

        cout << endl;

        previousLine = currentLine;
        currentLine = nextLine;
        file >> nextLine;
        rowIdx++;

    }

    cout << "The gear ratio sum is: " << gearRatioSum << endl;

    // string previousLine = ".........";
    // string currentLine =  "...5*8...";
    // string nextLine =     ".........";
    // searchResult result = lookForGears(currentLine, previousLine, nextLine, 4, 1);
    // // gear g = findGearValue(currentLine, 3, 2);

    // for(gear g : result.gearsFound) {
    //     cout << g.value << endl;
    // }

    // if (result.found) {
    //     cout << "Found gear" << endl;
    // } else {
    //     cout << "Did not find gear" << endl;
    // }  

    // // cout << g.value << endl;

    return 0;
}