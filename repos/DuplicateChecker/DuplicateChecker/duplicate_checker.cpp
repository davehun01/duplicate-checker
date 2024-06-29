#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include "network.h"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <utility>

std::vector<Network> networks;

std::vector<std::string> tokenize(std::string s, char del) {
    std::stringstream ss(s);
    std::string word;
    std::vector<std::string> tokens;
    while (!ss.eof()) {
        std::getline(ss, word, del);
        tokens.push_back(word);
    }
    return tokens;
}

void readFile() {
    std::fstream file;
   
    file.open("network.csv",std::ios::in); 
    if (file.is_open()) {   
        std::string line;
        std::vector<std::string> tokens;
        while(std::getline(file, line)) { 
            tokens = tokenize(line, ',');
            Network network;
            network.id = std::stoi(tokens[0]);
            network.from_node = std::stoi(tokens[1]);
            network.to_node = std::stoi(tokens[2]);
            network.frc_a = tokens[3];
            network.oneway = tokens[4];
            network.speed_limit = std::stoi(tokens[5]);
            network.kph = std::stoi(tokens[6]);
            network.street_name = tokens[7];
            network.street_type = tokens[8];
            network.schemeleft = tokens[9];
            network.fromleft = std::stoi(tokens[10]);
            network.toleft = std::stoi(tokens[11]);
            network.schemeright = tokens[12];
            network.fromright = std::stoi(tokens[13]);
            network.toright = std::stoi(tokens[14]);
            network.order8_name = tokens[15];

            //add to vector if town, street name and type are not empty (2 long strings will be equal to "")
            if (network.order8_name.size() > 2 && network.street_name.size() > 2 && network.street_type.size() > 2) {
                networks.push_back(network);
            }
        }
    file.close();
   }
}

bool compareNetworks(Network a, Network b) {
    if (a.order8_name != b.order8_name)
        return a.order8_name < b.order8_name;
 
    if (a.street_name != b.street_name)
        return a.street_name < b.street_name;
 
    if (a.street_type != b.street_type)
        return a.street_type < b.street_type;
    
    return (a.id < b.id);
}

bool compareIntervals(std::pair<int,int> a, std::pair<int, int> b) { 
    return a.first < b.first; 
}

std::vector<std::pair<int, int>> findDuplicateIntervals(std::vector<std::pair<int, int>> intervals, int n) {
    std::vector<std::pair<int, int>> duplicateIntervals;

    std::sort(intervals.begin(), intervals.end(), compareIntervals);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i >= j) continue;
            if (intervals[i].second >= intervals[j].first && intervals[i].second >= intervals[j].second) {
                duplicateIntervals.push_back(std::pair<int, int>(std::make_pair(intervals[j].first, intervals[j].second)));
            }
            else if (intervals[i].second >= intervals[j].first && intervals[i].second <= intervals[j].second) {
                duplicateIntervals.push_back(std::pair<int, int>(std::make_pair(intervals[j].first, intervals[i].second)));
            }
        }
    }

    return duplicateIntervals;
}

std::vector<std::pair<int, int>> mergeIntervals(std::vector<std::pair<int, int>> intervals, int n) {
    std::vector<std::pair<int, int>> mergedVector;

    std::sort(intervals.begin(), intervals.end(), compareIntervals);

    int index = 0; 

    for (int i = 1; i < n; i++) {

        if (intervals[index].second >= intervals[i].first) {
            intervals[index].second = std::max(intervals[index].second, intervals[i].second);
            intervals.erase(intervals.begin() + i);
            i--;
            n--;
        }
        else {
            index++;
            intervals[index] = intervals[i];
        }
    }
    
    return intervals;
}

void writeToFile(std::string filename) {
    std::ofstream outputFile(filename);

    for (std::vector<Network>::iterator it = networks.begin(); it != networks.end() - 1; ++it) {
        std::vector<std::pair<int, int>> OddHouseNumbers;
        std::vector<std::pair<int, int>> EvenHouseNumbers;

        while (((*it).order8_name == (*std::next(it, 1)).order8_name &&
            (*it).street_name == (*std::next(it, 1)).street_name &&
            (*it).street_type == (*std::next(it, 1)).street_type) &&
            it != networks.end() - 2) {
            if ((*it).schemeleft == "\"O\"") OddHouseNumbers.push_back(std::make_pair((*it).fromleft, (*it).toleft));
            if ((*it).schemeleft == "\"E\"") EvenHouseNumbers.push_back(std::make_pair((*it).fromleft, (*it).toleft));
            if ((*it).schemeright == "\"O\"") OddHouseNumbers.push_back(std::make_pair((*it).fromright, (*it).toright));
            if ((*it).schemeright == "\"E\"") EvenHouseNumbers.push_back(std::make_pair((*it).fromright, (*it).toright));
            ++it;
        }

        if ((*it).schemeleft == "\"O\"") OddHouseNumbers.push_back(std::make_pair((*it).fromleft, (*it).toleft));
        if ((*it).schemeleft == "\"E\"") EvenHouseNumbers.push_back(std::make_pair((*it).fromleft, (*it).toleft));
        if ((*it).schemeright == "\"O\"") OddHouseNumbers.push_back(std::make_pair((*it).fromright, (*it).toright));
        if ((*it).schemeright == "\"E\"") EvenHouseNumbers.push_back(std::make_pair((*it).fromright, (*it).toright));

        std::vector<std::pair<int, int>> duplicateOddIntervals = findDuplicateIntervals(OddHouseNumbers, OddHouseNumbers.size());
        std::vector<std::pair<int, int>> duplicateEvenIntervals = findDuplicateIntervals(EvenHouseNumbers, EvenHouseNumbers.size());

        std::vector<std::pair<int, int>> mergedOddIntervals = mergeIntervals(duplicateOddIntervals, duplicateOddIntervals.size());
        std::vector<std::pair<int, int>> mergedEvenIntervals = mergeIntervals(duplicateEvenIntervals, duplicateEvenIntervals.size());

        
        if (mergedOddIntervals.size() > 0) outputFile << (*it).order8_name << " " << (*it).street_name << " " << (*it).street_type << ": " << "O ";

        for (std::vector<std::pair<int, int>>::iterator it = mergedOddIntervals.begin(); it != mergedOddIntervals.end(); ++it)
            outputFile << "[" << (*it).first << "-" << (*it).second << "] ";
        if (mergedOddIntervals.size() > 0) outputFile << std::endl;

        if (mergedEvenIntervals.size() > 0) outputFile << (*it).order8_name << " " << (*it).street_name << " " << (*it).street_type << ": " << "E ";

        for (std::vector<std::pair<int, int>>::iterator it = mergedEvenIntervals.begin(); it != mergedEvenIntervals.end(); ++it)
            outputFile << "[" << (*it).first << "-" << (*it).second << "] ";
        if (mergedEvenIntervals.size() > 0) outputFile << std::endl;
        
    }

    outputFile.close();
}

int main() {

    readFile();

    std::sort(networks.begin(), networks.end(), compareNetworks);

    writeToFile("output.txt");
    
    return 0;
}