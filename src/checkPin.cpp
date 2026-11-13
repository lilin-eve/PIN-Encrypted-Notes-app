#include "checkPin.h"
#include "encryptPin.h"
#include "systemFiles.h"
#include <iostream>
//  #include <vector>
#include <string>
//  #include <fstream>
#include <thread>
#include <chrono>


//  definition for global bool value 'pinGet'
//  'pinGet' is false by default
//  when 'pinGet' is false, checkPin() loops, when 'pinGet' is true, checkPin() exits loop
bool pinGet = false;


//  declaration for called function clearScreen()
void clearScreen();

//  declaration for called function printBorderLine()
void printBorderLine();

//  declaration for called function userVault()
//  function located in file userVault.cpp
//  
void userVault( const size_t& userLine );


//  definition for called function getNewPin()
//  function is type int, returns int value to caller function
//  
std::pair<bool, int> getNewPin(){

    bool verifyPin{false};
    int pin{};

    int testPin{};
    int pinTries{0};
    do {
        printBorderLine();
        std::cout << "Enter new PIN: ";
        std::cin >> pin;
        std::cout << "Confirm PIN: ";
        std::cin >> testPin;
        if ( testPin != decryptPin(encryptPin(pin))){
            clearScreen();
            printBorderLine();
            std::cout << "Errr, PIN does not match. :/ " << std::endl;
            std::cout << "Try again." << std::endl;
            pinTries++;
        }
        if ( pinTries == 3 ){
            clearScreen();
            printBorderLine();
            std::cout << "Incorrect PIN was input 3 times. " << std::endl;
            break;
        }
    } while ( testPin != decryptPin(encryptPin(pin)) );

    if ( testPin == decryptPin(encryptPin(pin)) ){
        verifyPin = true;
    }

    return {verifyPin, pin};

}

//  definition for called & caller function getUserPin()
//  function is type pair, returns bool & int value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'userLine'
//  function calls clearScreen(), printBorderLine(), & confirmTargetLine()
//  
std::pair<bool, int> getUserPin( const size_t& userLine ){

    //  'verifyPin' is getPin()'s bool return value
    bool verifyPin{false};

    //  'pin' is getPin()'s int return value
    int pin{};
    int pinTries{0};

    clearScreen();
    do {
        printBorderLine();
        std::cout << "Enter PIN: ";
        std::cin >> pin;
        if ( !confirmTargetLine( "user-pins.txt", userLine, std::to_string(pin) )){
            clearScreen();
            printBorderLine();
            std::cout << "Incorrect PIN!" << std::endl;
            std::cout << "Try again." << std::endl;
            pinTries++;
        }
        if ( pinTries == 3 ){
            clearScreen();
            printBorderLine();
            std::cout << "Incorrect PIN was input 3 times!" << std::endl;
            break;
        }
    } while ( !confirmTargetLine( "user-pins.txt",userLine, std::to_string(pin) ) );

    if ( confirmTargetLine( "user-pins.txt", userLine, std::to_string(pin) ) == true ){
        pinTries = 0;
        verifyPin = true;
    }
    else if ( pinTries == 3 ){
        pinTries = 0;
        verifyPin = false;
    }
    
    return {verifyPin, pin};

}

void pinFound( const size_t& line ){
    printBorderLine();
    std::cout << "Success! PIN was verified on line " << line + 1 << " of ' sys/user-pins.txt '." << std::endl;
}

int pinNotFound(){

    int choice{};
    do {
        printBorderLine();
        std::cout << "PIN was unable to be verified!" << std::endl;
        std::cout << "Would you like to [1] Try again or [2] Quit: ";
        std::cin >> choice;
        if ( choice != 1 && choice != 2){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input! Input must either be '1' or '2'." << std::endl;
        }
    } while ( choice != 1 && choice != 2 );

    return choice;

}

void errorAddPin(){
    printBorderLine();
    std::cout << "Error adding PIN to ' sys/user-pins.txt ' ! " << std::endl;
}

void successAddPin( const std::string& userName, const size_t& line ){
    printBorderLine();
    std::cout << "Success! PIN added to line " << line + 1 << " of ' sys/user-pins.txt '." << std::endl;
    std::cout << "PIN set for user [ " << userName << " ]." << std::endl;
}

//  definition for called & caller function welcomeUser()
//  function is type void, function returns no value to caller function
//  function receives 1 const ref to 1 string value, locally named 'userName'
//  
void welcomeUser( const std::string& userName ){
    printBorderLine();
    std::cout << "Welcome to your personal notes vault, " << userName << ". :)" << std::endl;
    printBorderLine();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

//  definition for called & caller function checkPin()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'line', & 1 int value, locally named 'promptInput'
//  function calls getFileLine(), clearScreen(), getUserPin(), confirmTargetLine(), printBorderLine(), welcomeUser(), userVault(), getNewPin(), encryptPin(), decryptPin(), updateFileLine(), & errorRefUser()
//  
void checkPin( const size_t& line, int promptInput ){

    auto [ gotFileLine, lineContent ] = getFileLine( "users.txt", line );

    clearScreen();
    if ( gotFileLine == true ){
        std::string userName{ lineContent };
        int choice{};
        pinGet = false;

        while (!pinGet){
            if ( promptInput == 1 ){
                auto [ verifyPin, pin ] = getUserPin( line );
                if ( confirmTargetLine( "user-pins.txt", line, std::to_string(pin) ) == true ){
                    clearScreen();
                    pinFound( line );
                    welcomeUser(userName);
                    userVault( line );
                    pinGet = true;
                }
                else {
                    choice = pinNotFound();
                    clearScreen();
                    if ( choice == 1 ){
                        pinGet = false;
                    }
                    else if ( choice == 2 ){
                        pinGet = true;
                    }
                }
            }
            else if ( promptInput == 2 ){
                auto [ gotNewPin, pin ] = getNewPin();
                //  int pin{ getNewPin() };
                int encryptedPin{ encryptPin(pin) };
                if ( gotNewPin == true ){
                    clearScreen();
                    if ( updateFileLine( "user-pins.txt", line, std::to_string( encryptedPin ) ) == true ){
                        if ( confirmTargetLine( "user-pins.txt", line, std::to_string( pin ) ) == true ){
                            successAddPin( userName, line );
                            welcomeUser( userName );
                            userVault( line );
                            pinGet = true;
                        }
                        else{
                            printBorderLine();
                            std::cout << "Program Error! When passed user-input value 'pin', 'updateFileLine' returned true, 'confirmTargetLine' returned false. " << std::endl;
                            std::cout << "PIN not found on line " << line << " of ' sys/user-pins.txt '." << std::endl;
                            printBorderLine();
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            pinGet = true;
                        }
                    }
                    else {
                        errorAddPin();
                        pinGet = true;
                    }
                }
                else {
                    choice = pinNotFound();
                    clearScreen();
                    if ( choice == 1 ){
                        pinGet = false;
                    }
                    else if ( choice == 2 ){
                        pinGet = true;
                    }
                }
            
            }

        }
    }
    else {
        errorRefUser( line );
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return;

}
