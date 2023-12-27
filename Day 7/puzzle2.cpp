
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

string findHandtype(string hand) {

    string handType = "highCard";
    string adjustedHand;

    int jackCount = 0;
    for (char c : hand) {
            if (c != 'J') {
                adjustedHand += c;
            }
            else {
                jackCount++;
            }
        }

    map<char, int> cardCount;

    // cout << "Adjusted Hand: " << adjustedHand << " For hand: " << hand << endl;

    char highestRepetition = adjustedHand[0];
    int highestRepetitionCount = 0;
    for (int i = 0; i < adjustedHand.length(); i++) {
        
        cardCount[adjustedHand[i]]++;

        if (cardCount[adjustedHand[i]] > highestRepetitionCount) {
            highestRepetition = adjustedHand[i];
            highestRepetitionCount = cardCount[adjustedHand[i]];
        }
    }

    cardCount[highestRepetition] += jackCount;

    int pairCount = 0;
    int threeCount = 0;
    int fourCount = 0;
    int fiveCount = 0;

    for (auto card : cardCount) {

        if (card.first == 'J') {
            continue;
        }

        if (card.second == 2) {
            pairCount += 1;
        } else if (card.second == 3) {
            threeCount += 1;
        } else if (card.second == 4) {
            fourCount += 1;
        } else if (card.second == 5) {
            fiveCount += 1;
        }
    }

    if (fiveCount == 1) {
        handType = "fiveOfAKind";
    } else
    if (fourCount == 1) {
        handType = "fourOfAKind";
    } else if (threeCount == 1 && pairCount == 1) {
        handType = "fullHouse";
    } else if (threeCount == 1) {
        handType = "threeOfAKind";
    } else if (pairCount == 2) {
        handType = "twoPair";
    } else if (pairCount == 1) {
        handType = "onePair";
    } else if (pairCount == 0) {
        handType = "highCard";
    }

    return handType;

}

bool compareCards(map<string, string> a, map<string, string> b) {

    map<char, int> cardValues;
    cardValues['J'] = 1;
    cardValues['2'] = 2;
    cardValues['3'] = 3;
    cardValues['4'] = 4;
    cardValues['5'] = 5;
    cardValues['6'] = 6;
    cardValues['7'] = 7;
    cardValues['8'] = 8;
    cardValues['9'] = 9;
    cardValues['T'] = 10;
    cardValues['Q'] = 12;
    cardValues['K'] = 13;
    cardValues['A'] = 14;

    for (int i = 0; i < a["hand"].length(); i++) {
        
        if (a["hand"][i] == b["hand"][i]) {
            continue;
        }

        else {

            char aChar = a["hand"][i];
            char bChar = b["hand"][i];

            return cardValues[aChar] > cardValues[bChar];
        }
    }
    
    return cardValues[a["hand"].back()] > cardValues[b["hand"].back()];

}

void printVector(vector<map<string, string>> v) {

    cout << "Printing new Type of hand" << endl;
    for (auto hand : v) {
        cout << hand["hand"] << " " << hand["wager"] << endl;
    }
    cout << endl;

}

int main() {

    ifstream handFile("hands.txt");

    if (!handFile) {
        cout << "File not found" << endl;
        return 1;
    }

    string line;

    string hand;
    int wager;

    map<string, vector<map<string, string>>> hands;

    hands["fiveOfAKind"] = vector<map<string, string>>();
    hands["fourOfAKind"] = vector<map<string, string>>();
    hands["fullHouse"] = vector<map<string, string>>();
    hands["threeOfAKind"] = vector<map<string, string>>();
    hands["twoPair"] = vector<map<string, string>>();
    hands["onePair"] = vector<map<string, string>>();
    hands["highCard"] = vector<map<string, string>>();


    while (handFile >> hand >> wager) {

        string handType = findHandtype(hand);

        cout << "Hand: " << hand << " Hand Type: " << handType << endl;

        map<string, string> newHand;
        newHand["hand"] = hand;
        newHand["wager"] = to_string(wager);

        hands[handType].push_back(newHand);

    }

    int totalWinnings = 0;

    vector<vector<map<string, string>>> handsTogether;

    handsTogether.push_back(hands["fiveOfAKind"]);
    handsTogether.push_back(hands["fourOfAKind"]);
    handsTogether.push_back(hands["fullHouse"]);
    handsTogether.push_back(hands["threeOfAKind"]);
    handsTogether.push_back(hands["twoPair"]);
    handsTogether.push_back(hands["onePair"]);
    handsTogether.push_back(hands["highCard"]);

    vector<map<string, string>> allHands;

    for (auto handType : handsTogether) {

        sort(handType.begin(), handType.end(), compareCards);

        // cout << "Cards have been sorted" << endl;

        // printVector(handType);

        for (map<string, string> hand : handType) {

            allHands.push_back(hand);

        }

    }

    for (int i = 0; i < allHands.size(); i++) {

        int wager = stoi(allHands[i]["wager"]);
        int rank = allHands.size() - i;
        
        // cout << "Rank: " << rank << " Wager: " << wager << endl;
        totalWinnings += (rank * wager);

    }

    // printVector(allHands);

    cout << "The total winnings are: " << totalWinnings << endl;

    return 0;

}