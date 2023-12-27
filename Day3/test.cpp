
#include <iostream>
#include <regex>


int main(){

    const std::regex basic_regex("[^a-z0-9\\.]");


    while (true) {

        std::string character;

        std::cout << "Enter a character: ";
        std::cin >> character;
        std::cout << std::endl;

        if (std::regex_match(character, basic_regex) && character != "."){
            std::cout << "Matched" << std::endl;
        }
        else {
            std::cout << "Not matched" << std::endl;
        }
    }
    
}