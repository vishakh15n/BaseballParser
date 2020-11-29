#include <iostream>
#include <fstream>
#include "HashMap.h"
#include "Player.h"
#include <algorithm>
using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end+1);
}

string trim(const string& s)
{
    return rtrim(ltrim(s));
}

class Main {
private:
    HashMap<string, string> plateMap;
    HashMap<string, Player> playerMap;

public:
    void parseKeyFile();
    void parsePlayerFile(const string& filename, ostream& out);

    void printResults(ostream& os);
    void printLeaders(ostream& os);

    void updatePlayer(Player *player, const string &plateResult) const;
};

void Main::parseKeyFile() {
    ifstream inFile("keyfile.txt");
    string line;
    string value;
    while(getline(inFile, line)){
        line = trim(line);
        if (line[0] == '#'){
            line = line.substr(3);
            line = line.substr(0, line.find("#") - 1);
            value = line;
        }
        else if(!line.empty()){

            plateMap.put(trim(line), trim(value));
        }
    }

}

void Main::parsePlayerFile(const string &filename, ostream& out) {
    ifstream inFile(filename);
    string line;
    string value;
    while(getline(inFile, line)) {
        bool home = false;


        if (line[0] == 'A'){
            home = false;
        }
        else if (line[0] == 'H'){
            home = true;
        }
        else{
            continue;
        }

        line = line.substr(2);


        auto worBreakIndex = line.find(" ");
        string name = trim(line.substr(0, worBreakIndex));
        line = line.substr(worBreakIndex + 1);
        string plateResult;

        string plateKey = trim(line);
        string *expression5 = plateMap.get(plateKey);

        if (expression5){
            plateResult = *expression5;
        }


        Player *p = playerMap.get(name);
        if (p){
            updatePlayer(p, plateResult);
        }
        else {
            Player player;
            player.home = home;
            updatePlayer(&player, plateResult);
            player.name = name;
            playerMap.put(name, player);

        }

    }

}


void Main::updatePlayer(Player *player, const string &plateResult) const {
    if(plateResult == "OUTS"){
        player->out++;
        player->atBats++;
        player->pA++;
    }
    else if(plateResult == "STRIKEOUT"){
        player->strikeOut++;
        player->atBats++;
        player->pA++;
    }
    else if(plateResult == "HITS"){
        player->hits++;
        player->atBats++;
        player->pA++;
    }
    else if (plateResult == "WALK"){
        player->walks++;
        player->pA++;
    }
    else if(plateResult == "SACRIFICE"){
        player->sacrifice++;
        player->pA++;
    }
    else if(plateResult == "HIT BY PITCH"){
        player->HBP++;
        player->pA++;
    }
    else if(plateResult == "ERRORS"){
        player->pA++;
    }
    player->setBattingAverage();
    player->setOBP();
}

void Main::printResults(ostream &out) {
    vector<string> playerKeys = playerMap.getKeys();
    vector<string> homeKeys;
    vector<string> awayKeys;

    for (auto it = playerKeys.begin(); it != playerKeys.end(); ++it) {
        Player *p = playerMap.get(*it);
        if (p->home) {
            homeKeys.push_back(*it);
        } else {
            awayKeys.push_back(*it);
        }
    }

    sort(homeKeys.begin(), homeKeys.end());
    sort(awayKeys.begin(), awayKeys.end());


    out << "AWAY" << endl;

    for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
        Player* p = playerMap.get(*it);
        out << *p << endl;
    }

    out << endl;
    out << "HOME" << endl;
    for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
        Player* p = playerMap.get(*it);
        out << *p << endl;
    }

    out << endl << "LEAGUE LEADERS";


}

void Main::printLeaders(ostream &out) {
    // Need to check size of Hash Map so that we don't get stuck in infinite loop if
    // we have less than 3 nodes

    vector<string> v = playerMap.getKeys();
    // Initiallize amount of leaders to 3
    int maxCount = 3;
    if (v.size() < maxCount){
        maxCount = v.size();
    }

    vector<string> homeKeys;
    vector<string> awayKeys;

    for (auto it = v.begin(); it != v.end(); ++it) {
        Player *p = playerMap.get(*it);
        if (p->home) {
            homeKeys.push_back(*it);
        } else {
            awayKeys.push_back(*it);
        }
    }

    sort(homeKeys.begin(), homeKeys.end());
    sort(awayKeys.begin(), awayKeys.end());


    // Set highest number equal to 0 so that we can find highest number
    int highest = 0;
    // Set count equal to 0 so that we can check how many leaders have been processed
    int count = 0;
    // Need boolean variable to check for ties
    bool tie = false;
    // Initializing double for the leaders that are double instead of integer
    double highest2;
    // Print an extra line before starting
    out << endl;
    // For each data type, find the highest value, then check to see when highest value
    // occurs
    vector<string> vector = playerMap.getKeys();
    Player *player;
    // Print out BATTING AVEERAGE
    out << "BATTING AVERAGE" << endl;
    // Have to make sure we only print out 3 leaders
    while (count < maxCount){
        // We need to set tie to false at the beginning because we can't start off with a tie.
        tie = false;
        highest2 = 0;
        // Write the for loop using the individual keys...
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            // ... then initialize each Player with the key.
            player = playerMap.get(*it);
            // Find highest value.
            if(player->battingAverage > highest2){
                highest2 = player->battingAverage;
            }
        }
        // We need two for loops because we need to print all the away players first then all the home players instead of just alphabetical order.
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            // Check to see if each player matches the highest.
            Player* p = playerMap.get(*it);
            if(p->battingAverage == highest2){
                // If we don't have a tie we need to print the number, and if we don't we need to have a comma first.
                if (tie == false){
                    out << p->battingAverage << "\t" << p->name;
                    // Set data variable to -1 so it won't get picked up as the highest again.
                    p->battingAverage = -1;

                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    // Set data variable to -1 so it won't get picked up as the highest again.
                    p->battingAverage = -1;
                }
                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        // Same thing as awayKeys except for homeKeys.
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->battingAverage == highest2){
                if (tie == false){
                    out << p->battingAverage << "\t" << p->name;
                    p->battingAverage = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->battingAverage = -1;

                }
                // Increases the count by one to check if it exceeds 3.
                count++;

            }

        }
        out << endl;
    }
    // Do the same thing for the other data members
    count = 0;
    out << endl;
    out << "ON-BASE PERCENTAGE" << endl;
    while (count < maxCount){
        tie = false;
        highest2 = 0;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            player = playerMap.get(*it);
            if(player->OBP > highest2){
                highest2 = player->OBP;
            }
        }
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->OBP == highest2){
                if (tie == false){
                    out << p->OBP << "\t" << p->name;
                    p->OBP = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->OBP = -1;
                }
                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->OBP == highest2){
                if (tie == false){
                    out << p->OBP << "\t" << p->name;
                    p->OBP = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->OBP = -1;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;
            }

        }
        out << endl;
    }
    count = 0;
    out << endl;
    out << "HITS" << endl;
    while (count < maxCount){
        tie = false;
        // Set highest to 0 because highest is an integer and highest2 was a double
        highest = 0;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            player = playerMap.get(*it);
            if(player->hits > highest){
                highest = player->hits;
            }
        }
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->hits == highest){
                if (tie == false){
                    out << p->hits << "\t" << p->name;
                    p->hits = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->hits = -1;
                }
                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->hits == highest){
                if (tie == false){
                    out << p->hits << "\t" << p->name;
                    p->hits = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->hits = -1;
                }
                // Increases the count by one to check if it exceeds 3.
                count++;

            }

        }
        out << endl;
    }
    count = 0;
    out << endl;
    out << "WALKS" << endl;
    while (count < maxCount){
        tie = false;
        highest = 0;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            player = playerMap.get(*it);
            if(player->walks > highest){
                highest = player->walks;
            }
        }
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->walks == highest){
                if (tie == false){
                    out << p->walks << "\t" << p->name;
                    p->walks = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->walks = -1;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->walks == highest){
                if (tie == false){
                    out << p->walks << "\t" << p->name;
                    p->walks = -1;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->walks= -1;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;

            }

        }
        out << endl;
    }
    count = 0;
    out << endl;
    out << "STRIKEOUTS" << endl;
    while (count < maxCount){
        tie = false;
        highest = 10000000;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            player = playerMap.get(*it);
            if(player->strikeOut < highest){
                highest = player->strikeOut;
            }
        }
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->strikeOut == highest){
                if (tie == false){
                    out << p->strikeOut << "\t" << p->name;
                    p->strikeOut = 100000;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->strikeOut = 100000;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player* p = playerMap.get(*it);
            if(p->strikeOut == highest){
                if (tie == false){
                    out << p->strikeOut << "\t" << p->name;
                    p->strikeOut = 100000;
                    tie = true;
                }
                else{
                    out << ", " << p->name;
                    p->strikeOut = 100000;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;

            }

        }
        out << endl;
    }
    count = 0;
    out << endl;
    out << "HIT BY PITCH" << endl;
    while (count < maxCount) {
        tie = false;
        highest = 0;
        for (auto it = vector.begin(); it != vector.end(); ++it) {
            player = playerMap.get(*it);
            if (player->HBP > highest) {
                highest = player->HBP;
            }
        }
        for (auto it = awayKeys.begin(); it != awayKeys.end(); ++it) {
            Player *p = playerMap.get(*it);
            if (p->HBP == highest) {
                if (tie == false) {
                    out << p->HBP << "\t" << p->name;
                    p->HBP = -1;
                    tie = true;
                } else {
                    out << ", " << p->name;
                    p->HBP = -1;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;
            }
        }
        for (auto it = homeKeys.begin(); it != homeKeys.end(); ++it) {
            Player *p = playerMap.get(*it);
            if (p->HBP == highest) {
                if (tie == false) {
                    out << p->HBP << "\t" << p->name;
                    p->HBP = -1;
                    tie = true;
                } else {
                    out << ", " << p->name;
                    p->HBP = -1;
                }

                // Increases the count by one to check if it exceeds 3.
                count++;

            }

        }
        if(count >= maxCount){
            out << endl;
        }
        out << endl;
        //out << plateMap;


    }

}



int main() {

    Main m;
    m.parseKeyFile();


    string filename;
    cin >> filename;
    // Prompt filename

    ofstream outputFile("leaders.txt");

    m.parsePlayerFile(filename, outputFile);



    m.printResults(outputFile);

    m.printLeaders(outputFile);

}




