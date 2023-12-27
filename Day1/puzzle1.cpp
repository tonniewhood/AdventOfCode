
#include <iostream>
#include <fstream>
#include <string>

int main(){

    std::ifstream inputFile;
    inputFile.open("puzzleInput.txt", std::ios::in);

    int runningTotal = 0;

    if (inputFile.is_open()){

        std::string line;

        // while (std::getline(inputFile, line)){
            
        //     for (int i = 0; i < line.length(); i++){

        //         std::

        //     }

        // }

        while (std::getline(inputFile, line)){

            std::string lineNumbers;

            for (int i = 0; i < line.length(); i++){
                
                if ((int) line[i] > 48 && (int) line[i] < 58){
                    lineNumbers += line[i];
                } 

            }

            if (!lineNumbers.empty()){
                int tensPlace = lineNumbers.front() - '0';
                int onesPlace = lineNumbers.back() - '0';
                int lineValue = (tensPlace * 10) + onesPlace;
                runningTotal += lineValue;
            }

        }

        std::cout << runningTotal << std::endl;
    }

    return 0;

}