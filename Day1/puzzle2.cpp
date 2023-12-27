
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<std::string> NUMBERS = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven", 
    "eight",
    "nine"
};

struct numbers {
    int startIdx;
    int value;
};


void print_all_numbers_found(std::vector<numbers> numbersFound){
    for (auto number : numbersFound){
        std::cout << number.value << " ";
    }
}

void find_english_numbers(std::string line, std::vector<numbers> &numbersFound){
    
    for (std::string number : NUMBERS){

        bool numberFound;
        int repeats = 0;
        std::string tempLine = line;

        do {
            numberFound = false;

            const int indexInTempLine = tempLine.find(number);
            if (indexInTempLine != std::string::npos){
                
                const int indexInLine = indexInTempLine + (number.length() * repeats);
                const int value = std::distance(NUMBERS.begin(), std::find(NUMBERS.begin(), NUMBERS.end(), number));
                numbers newNumber = {indexInLine, value};
                numbersFound.push_back(newNumber);

                numberFound = true;
                repeats++;
                tempLine.erase(indexInTempLine, number.length());
            }

        } while (numberFound);

    }
    

}

void find_symbolic_numbers(std::string line, std::vector<numbers> &numbersFound){
    
    int startIdx = 0;

    for (int i = 0; i < line.length(); i++){

        if (line[i] >= '0' && line[i] <= '9'){
            const int idxInLine = i;
            numbersFound.push_back({idxInLine, line[i] - '0'});
        }

    }

}

void print_number_indecies(std::vector<numbers> numbersFound){
    for (auto number : numbersFound){
        std::cout << number.value << " Was found at idx: " << number.startIdx << std::endl;
    }
}


int main(){

    std::ifstream inputFile;
    inputFile.open("puzzleInput.txt", std::ios::in);

    if (inputFile.is_open()){

        std::string line;
        int runningTotal = 0;
        

        while (std::getline(inputFile, line)){

            bool foundNumber;
            std::vector<numbers> numbersFound;

            find_symbolic_numbers(line, numbersFound);
            find_english_numbers(line, numbersFound);

            std::sort(numbersFound.begin(), numbersFound.end(), [](numbers a, numbers b){
                return a.startIdx < b.startIdx;
            });

            print_number_indecies(numbersFound);
            std::cout << "All numbers found comprise: ";
            print_all_numbers_found(numbersFound);

            if (!numbersFound.empty()){
                int tensPlace = numbersFound.front().value;
                int onesPlace = numbersFound.back().value;
                int lineValue = (tensPlace * 10) + onesPlace;
                runningTotal += lineValue;

                std::cout << "\tAdding a number to the total: " << lineValue << "\tFrom line: " << line << std::endl << std::endl;
            }
        }

        std::cout << runningTotal << std::endl;
    }
}