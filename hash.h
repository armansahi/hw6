#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        unsigned long long w[5] = {0}; // array to store converted value of string

        std::string lowerK = k;
        for (char& ch : lowerK) {
          ch = tolower(ch);
        } // convert string to lowercase to avoid case sensitivity

        int len = lowerK.length();
        int group = 0;

        // process string groups by 6 char in reverse
        for (int i = len; i > 0; i-=6, group++) {
          int start = std::max(0, i-6); // make sure index not below 0
          int size = i - start; // curr group size
          unsigned long long val = 0; // initalize group value

          for (int j = 0; j < size; j++) { // convert to base 36 value 
            val = val * 36 + letterDigitToNumber(lowerK[start + j]); 
          }
          w[4 - group] = val; // store val of group in w array
        }

        unsigned long long hash = 0; // initialize final hash val

        // combine w and r values for final hash
        for (int i = 0; i < 5; i++) {
          hash += rValues[i] * w[i]; // multiply eachw by r and add to hash
        }

        return static_cast<HASH_INDEX_T>(hash); // return final hash
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        letter = tolower(letter);
        if (letter >= 'a' && letter <= 'z') {
          return letter - 'a'; // maps 0-25
        }
        if (letter >= '0' && letter <= '9') {
          return letter - '0' + 26; // maps 26-35
        }

        return 0; // any invalid chars
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
