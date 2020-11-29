//
// Created by Vishakh Nair on 11/24/19.
//

#ifndef PROJECT04_PLAYER_H
#define PROJECT04_PLAYER_H

#include <string>
using namespace std;


class Player {
public:
    // Data Variables
    string name;
    bool home;
    int atBats;
    int hits;
    int walks;
    int out;
    int strikeOut;
    int HBP;
    int sacrifice;
    double battingAverage;
    double OBP;
    int pA;

    // Constructors
    // Mainly default Constructor will be used.
    Player (string n, int h, int o, int sO, int w, int hbp, int s, bool ho, int p){
        name = n;
        home = ho;
        atBats = h + o + sO;
        hits = h;
        walks = w;
        out = o;
        strikeOut = sO;
        HBP = hbp;
        sacrifice = s;
        pA = p;
        battingAverage = 1.0 * hits/atBats;
        OBP = 1.0 * (h + w + hbp ) / pA;

    }

    Player (){
        name = "";
        atBats = 0;
        hits = 0;
        walks = 0;
        out = 0;
        strikeOut = 0;
        HBP = 0;
        sacrifice = 0;
        battingAverage = 0;
        OBP = 0;
        pA = 0;
    }

    // Calulating Batting Average inline
    void setBattingAverage(){
        // Need to check for division by 0
        if (atBats != 0){
            battingAverage = 1.0 * hits/atBats;
        }
        else{
            battingAverage = 0;
        }
    }

    // Calculating On-Base Percentage inline
    void setOBP(){
        // Need to check for division by 0
        if (pA != 0){
            OBP = 1.0 * (hits + walks + HBP ) / pA;
        }
        else{
            OBP = 0;
        }
    }

    // Overloading == operator inline. Simple logic
    bool operator==(Player const &player){
        if (name == player.name){
            return true;
        }
        return false;
    }

    // Functions overriden in Player.cpp
    friend ostream & operator <<(ostream &out, const Player &p);
    friend bool operator < (Player a, Player b);
};


#endif //PROJECT04_PLAYER_H
