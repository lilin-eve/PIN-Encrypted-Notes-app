#include "encryptPin.h"
#include <iostream>
#include <vector>
#include <string>

//  definition for called function combineDigits()
//  function is type int, returns int value to caller function
//  function receives 1 const ref to 1 int vector, locally named 'digits'
//  
int combineDigits( const std::vector<int>& digits ){

    std::string numTemp{ std::to_string(digits[0]) + std::to_string(digits[1]) + std::to_string(digits[2]) + std::to_string(digits[3]) };

    int num{ std::stoi(numTemp) };

    return num;

}

//  definition for called function splitDigits()
//  function is type int vector, returns vector of int values to caller function
//  function receives 1 const ref to 1 int value, locally named 'num'
//  
std::vector<int> splitDigits( const int& num ){

    std::vector<int> numDigits{};

    /*
    while ( num > 0 ){
        numDigits.push_back( num % 10 );
        num /= 10;
    }
    */
   for ( int x = num; x > 0; x /= 10 ){
        numDigits.push_back( x % 10 );
   }
    std::reverse( numDigits.begin(), numDigits.end() );

    return numDigits;

}

//  definition for called function decryptPinDigits()
//  function is type int vector, returns vector of int values to caller function
//  function receives 1 const ref to 1 int vector, locally named 'encryptedPinDigits'
//  
std::vector<int> decryptPinDigits( const std::vector<int>& encryptedPinDigits ){

    std::vector<int> decryptedPinDigits{};

    for ( size_t i = 0; i < encryptedPinDigits.size(); i++ ){
        decryptedPinDigits.push_back( (encryptedPinDigits[i] + 3) % 10 );
    }

    return decryptedPinDigits;

}

//  definition for called function encryptPinDigits()
//  function is type int vector, returns vector of int values to caller function
//  function receives 1 const ref to 1 int vector, locally named 'pinDigits'
//  
std::vector<int> encryptPinDigits( const std::vector<int>& pinDigits ){

    std::vector<int> encryptedPinDigits{};

    for ( size_t i = 0; i < pinDigits.size(); i++ ){
        encryptedPinDigits.push_back( (pinDigits[i] + 7) % 10 );
    }

    return encryptedPinDigits;

}

//  definition for called & caller function encryptPin()
//  function is type int, returns int value to caller function
//  function receives 1 const reference for int value, locally named 'pin'
//  function performs all the steps for encrypting 4-digit int value, call this function when encrypting int values
//  function calls splitDigits(), encryptPinDigits(), & combineDigits()
//  
int encryptPin( const int& pin ){

    std::vector<int> pinDigits{ splitDigits(pin) };
    std::vector<int> encryptedPinDigits{ encryptPinDigits( pinDigits ) };
    int encryptedPin{ combineDigits( encryptedPinDigits ) };

    return encryptedPin;

}

//  definition for called & caller function decryptPin()
//  function is type int, returns int value to caller function
//  function receives 1 const reference for int value, locally named 'pin'
//  function performs all the steps for encrypting 4-digit int value, call this function when encrypting int values
//  function calls splitDigits(), decryptPinDigits(), & combineDigits()
//  
int decryptPin( const int& encryptedPin ){

    std::vector<int> encryptedPinDigits{ splitDigits(encryptedPin) };
    std::vector<int> decryptedPinDigits{ decryptPinDigits( encryptedPinDigits ) };
    int decryptedPin{ combineDigits( decryptedPinDigits ) };

    return decryptedPin;

}
