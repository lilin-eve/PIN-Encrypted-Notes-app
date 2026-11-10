#include <iostream>
#include <vector>
#include <string>


std::vector<int> decryptPinDigits( const std::vector<int>& pinDigits ){

    std::vector<int> decryptedPinDigits{};

    for ( size_t i = 0; i < pinDigits.size(); i++ ){
        decryptedPinDigits.push_back( (pinDigits[i] + 3) % 10 );
    }

    return decryptedPinDigits;

}

std::vector<int> encryptPinDigits( const std::vector<int>& pinDigits ){

    std::vector<int> encryptedPinDigits{};

    for ( size_t i = 0; i < pinDigits.size(); i++ ){
        encryptedPinDigits.push_back( (pinDigits[i] + 7) % 10 );
    }

    return encryptedPinDigits;

}

int combineDigits( const std::vector<int>& digits ){

    std::string numTemp{ std::to_string(digits[0]) + std::to_string(digits[1]) + std::to_string(digits[2]) + std::to_string(digits[3]) };

    int num{ std::stoi(numTemp) };

    return num;

}

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

//  definition for called & caller function encryptPin()
//  function is type int, returns int value to caller function
//  function receives 1 const reference for int value, locally named 'pin'
//  function performs all the steps for encrypting 4-digit int value, call this function when encrypting int values
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
//  
int decryptPin( const int& encryptedPin ){

    std::vector<int> encryptedPinDigits{ splitDigits(encryptedPin) };
    std::vector<int> decryptedPinDigits{ decryptPinDigits( encryptedPinDigits ) };
    int decryptedPin{ combineDigits( decryptedPinDigits ) };

    return decryptedPin;

}
