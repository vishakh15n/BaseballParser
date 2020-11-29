//
// Created by Vishakh Nair on 11/24/19.
//

#include "Player.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Overloaded  << operator
ostream & operator <<(ostream &out, const Player &p){
    // Write data ouput seperated by tabs
    out << p.name << "\t";
    out << p.atBats << "\t";
    out << p.hits << "\t";
    out << p.walks << "\t";
    out << p.strikeOut << "\t";
    out << p.HBP << "\t";
    out << p.sacrifice << "\t";
    // Need to round to 3 decimal places for the doubles
    out << setprecision(3) << fixed << p.battingAverage << "\t";
    out << setprecision(3) << fixed << p.OBP;

    return out;
}

// Overloaded < operator
bool operator < (const Player &a, const Player &b)
{
    // When comparing strings, need to make sure upper case doesnt matter
    // for example D should not be less than a
    string string1 = a.name;
    string string2 = b.name;
    transform(string1.begin(), string1.end(), string1.begin(), ::tolower);
    transform(string2.begin(), string2.end(), string2.begin(), ::tolower);
    return string1 < string2;

}

