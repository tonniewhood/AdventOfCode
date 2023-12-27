
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// The answer to puzzle one is down below, use this to check your answer.
const int ANSWER = 526404;

int findStartIndexForDigit(int characterLocation, string currentLine){

    const regex basic_regex("[0-9]");

    int startIndex = 0;
    for (int i = characterLocation; i >= 0; i--){

        const string character(1, currentLine[i]);
        // cout << "Checking character: " << character << " at inex: " << i << endl;

        if (!regex_match(character, basic_regex)){
            // cout << "Found the start of the number at index: " << i + 1 << endl;
            startIndex = i + 1;
            break;
        }

    }

    return startIndex;

}

bool isSymbolAbove(int characterLocation, string previousLine, string currentLine){

    // if the previous line is null, skip it
    if (previousLine == ""){
        return false;
    }

    // If not, determine where to start
    int startIndex;
    if (characterLocation == 0){
        startIndex = 0;
    }
    else {
        startIndex = characterLocation - 1;
    }

    int index = startIndex;
    for (int index = startIndex; index < startIndex + 3 && index != string::npos; index++){

        const regex basic_regex("[^a-z0-9\\.\\n\\t\\r\\0]");
        const string character(1, previousLine[index]);

        //cout << "Checking character" << character << endl;

        if (regex_match(character, basic_regex)){
            cout << "Found a symbol above: " << to_string(previousLine[index]) << endl;
            return true;
        }

    }

    return false;

}

bool isSymbolBelow(int characterLocation, string nextLine, string currentLine){

    // if the next line is null, skip it
    if (nextLine == ""){
        return false;
    }

    // If not, determine where to start
    int startIndex;
    if (characterLocation == 0){
        startIndex = 0;
    }
    else {
        startIndex = characterLocation - 1;
    }

    int index = startIndex;
    for (int index = startIndex; index < startIndex + 3 && index != string::npos; index++){

        const regex basic_regex("[^a-z0-9\\.\\n\\r\\t\\s\\0]");
        const string character(1, nextLine[index]);

       // cout << "Checking character" << character << "\t Also checking the current line at the index to see that currentLine[index]: " << currentLine[characterLocation] << endl;

        if (regex_match(character, basic_regex)){
            cout << "Found a symbol below: " << character << endl;
            return true;
        }

    }

    return false;

}

bool isSymbolLeft(int characterLocation, string currentLine){

    if (characterLocation == 0){
        return false;
    }

    const regex basic_regex("[^a-z0-9\\.\\n\\r\\t\\s\\0]");
    const string character(1, currentLine[characterLocation - 1]);

   // cout << "Checking character" << character << "\t Also checking the current line at the index to see that currentLine[index]: " << currentLine[characterLocation] << endl;

    cout << "Found a symbol on the left: " << character << endl;
    return regex_match(character, basic_regex);

}

bool isSymbolRight(int characterLocation, string currentLine){

    if (characterLocation == currentLine.length() - 1){
        return false;
    }

    const regex basic_regex("[^a-z0-9\\.\\n\\r\\t\\s\\0]");
    const string character(1, currentLine[characterLocation + 1]);

    //cout << "Checking character" << character << "\t Also checking the current line at the index to see that currentLine[index]: " << currentLine[characterLocation] << endl;

    cout << "Found a symbol on the right: " << character << endl;
    return regex_match(character, basic_regex);

}

// This function checks to see if the current character is surrounded by a symbol
bool isCharacterSurrounded(char character, int characterLocation, string currentLine, string previousLine, string nextLine){

    bool symbolAbove = isSymbolAbove(characterLocation, previousLine, currentLine);
    bool symbolBelow = isSymbolBelow(characterLocation, nextLine, currentLine);
    bool symbolLeft = isSymbolLeft(characterLocation, currentLine);
    bool symbolRight = isSymbolRight(characterLocation, currentLine);

    return symbolAbove || symbolBelow || symbolLeft || symbolRight;

}

// This function finds rest of the number surrounding a digit that has been found
int findPartNumber(string currentLine, int characterLocation){

    const regex basic_regex("[0-9]");

    int startIndex = findStartIndexForDigit(characterLocation, currentLine);


    string number = "";
    for (int i = startIndex; i < currentLine.length(); i++){

        const string character(1, currentLine[i]);

        if (!regex_match(character, basic_regex)){
            break;
        }

        number += currentLine[i];

    }

    // cout << "The number is: " << number << endl;
    // cout << currentLine << endl;

    return stoi(number);

}



int main (){

    // Open the file
    ifstream partsFile("partNumbers.txt");

    // Check if the file is open
    if (!partsFile.is_open()){
        cout << "Could not open file." << endl;
        return 0;
    }

    // Read in the first two lines and set up the cascade of lines with the previous, current, and next lines.
    string previousLine = "";

    string currentLine;
    getline(partsFile, currentLine);

    string nextLine; 
    getline(partsFile, nextLine);

    int partNumberSum = 0;
    int counter = 3;
    int lineCounter = 0;

    // Loop through the file until the end
    while (currentLine != "" && counter > 0){
        
        cout << lineCounter++ << "\t" << currentLine << endl;
        // cout << lineCounter++ << "\t" <<  currentLine << "\tPart Numbers found: ";

        for (int i = 0; i < currentLine.length(); i++){
            
            bool surrounded = false;
            // Check to see if the current character is surrounded by a symbol
            if (currentLine[i] >= '0' && currentLine[i] <= '9'){
                surrounded = isCharacterSurrounded(currentLine[i], i, currentLine, previousLine, nextLine);
                
                if (surrounded){
                    int partNumber = findPartNumber(currentLine, i);
                    partNumberSum += partNumber;

                    cout << partNumber << " ";

                    const int numLength = to_string(partNumber).length();
                    i = findStartIndexForDigit(i, currentLine) + numLength - 1;
                }   

            }
        }

        cout << endl;

        // Set up the next iteration
        previousLine = currentLine;
        currentLine = nextLine;
        getline(partsFile, nextLine);
        if (partsFile.eof()){
            counter--;
        }

    }

    cout << "The sum of all the part numbers is: " << partNumberSum << endl;
    cout << "The error in your answer is " << ANSWER - partNumberSum << endl;

}