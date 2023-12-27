
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<vector<long long>> getHistories(ifstream& history) {

    string line;

    vector<vector<long long>> histories;
    while (getline(history, line)) {
        
        stringstream ssLine(line);
        vector<long long> history;
        long long num; 

        while (ssLine >> num) {
            history.push_back(num);
        }

        histories.push_back(history);

    }

    return histories;

}

void printVector(vector<long long> v) {
    cout << "[";
    for (auto num : v) {
        cout << num << " ";
    }
    cout << "]" << endl;
}

bool allZeros(vector<long long> v) {

    for (auto num : v) {
        if (num != 0) {
            return false;
        }
    }

    return true;
}

long long findNextInSequence(vector<long long> history) {

    vector<long long> differences;

    for (int i = 0; i < history.size() - 1; i++) {
        differences.push_back(history[i + 1] - history[i]);
    }

    if (allZeros(differences)) {
        return history[history.size() - 1];
    }
    else {
        return findNextInSequence(differences) + history[history.size() - 1];
    }
}

int main() {

    ifstream history("history.txt");

    if (!history) {
        cout << "Error opening file" << endl;
        return -1;
    }

    vector<vector<long long>> histories = getHistories(history);

    history.close();

    vector<long long> predictedNext;

    for (auto history : histories) {
        
        
        long long nextInSequence = findNextInSequence(history);
        predictedNext.push_back(nextInSequence);

        // cout << "Next in sequence: " << nextInSequence << endl;

    }

    cout << "Sum of all predicted nexts: ";
    long long sum = 0;
    for (auto num : predictedNext) {
        sum += num;
    }
    cout << sum << endl;

    

    return 0;

}