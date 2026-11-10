#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

//  definition for global bool value 'pinGet'
//  'pinGet' is false by default
//  when 'pinGet' is false, checkPin() loops, when 'pinGet' is true, checkPin() exits loop
bool pinGet = false;

//  declaration for called function clearScreen()
//  function is type void, returns no value to caller function
//  
void clearScreen();

//  declaration for called function printBorderLine()
//  function is type void, returns no value to caller function
//  
void printBorderLine();

//  declaration for called function userVault()
//  function is type void, returns no value to caller function
//  
void userVault( const size_t& userLine );

std::pair<bool, std::string> getFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineNum );

bool updateFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent );

bool confirmTargetLine( const std::string& fileName, const size_t& lineTarget, const std::string& searchTarget );

void errorRefUser( const size_t& userLine );

int encryptPin( const int& pin );

int decryptPin( const int& encryptedPin );
/*
void addPin( const int& pin ){

    std::string pinString{ std::to_string(pin) };

    std::ofstream filePinAssign{ "user-pins.txt", std::ios::app };
    if ( filePinAssign.is_open() ){
        filePinAssign << pinString + "\n";
        filePinAssign.close();
    }
    else {
        std::cout << "Error! File 'pins.txt' unable to be opened." << std::endl;
    }

}

bool confirmPin( const int& pin, const size_t& lineTarget ){

    bool confirmPin = false;

    std::vector<int> pinDigits{ splitDigits(pin) };
    std::vector<int> encTemp{ encryptPin(pinDigits) };
    int encryptPin{ combineDigits(encTemp) };
    std::string encryptPinString{ std::to_string(encryptPin) };

    std::ifstream readFilePins{ "user-pins.txt" };
    size_t lineNum{0};
    std::string lineContent{};

    if ( readFilePins.is_open() ){
        while (getline( readFilePins, lineContent )){
            if (lineContent.find(encryptPinString) != std::string::npos ){
                if ( lineNum == lineTarget ){
                    confirmPin = true;
                }
                else {
                    confirmPin = false;
                }
            }
            lineNum++;
        }
        readFilePins.close();
    }

    return confirmPin;

}
*/

void welcomeUser( const std::string& userName ){

    printBorderLine();
    std::cout << "Welcome to your personal notes vault, " << userName << ". :)" << std::endl;

}

int getNewPin(){

    int pin{};

    printBorderLine();
    std::cout << "Enter new PIN: ";
    std::cin >> pin;

    return pin;

}

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

void checkPin( const size_t& line, int promptInput ){

    auto [ gotFileLine, lineContent ] = getFileLine( "sys/", "users.txt", line );

    if ( gotFileLine == true ){
        std::string userName{ lineContent };
        pinGet = false;

        clearScreen();
        while (!pinGet){
            if ( promptInput == 1 ){
                auto [ verifyPin, pin ] = getUserPin( line );
                if ( confirmTargetLine( "user-pins.txt", line, std::to_string(pin) ) == true ){
                    clearScreen();
                    printBorderLine();
                    std::cout << "Success! PIN was verified on line " << line + 1 << " of ' sys/user-pins.txt '." << std::endl;
                    welcomeUser( userName );
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    userVault( line );
                    pinGet = true;
                }
                else {
                    int choice{};
                    do {
                        printBorderLine();
                        std::cout << "PIN was unable to be verified!" << std::endl;
                        std::cout << "Would you like to [1] Try again or [2] Quit: ";
                        if ( choice != 1 && choice != 2){
                            clearScreen();
                            printBorderLine();
                            std::cout << "Invalid input! Input must either be '1' or '2'." << std::endl;
                        }
                    } while ( choice != 1 && choice != 2 );
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
                int pin{ getNewPin() };
                int encryptedPin{ encryptPin(pin) };
                int testPin{};
                int pinTries{0};
                do {
                    std::cout << "Confirm PIN: ";
                    std::cin >> testPin;
                    if ( testPin != decryptPin( encryptedPin ) ){
                        clearScreen();
                        printBorderLine();
                        std::cout << "Error! PIN doesn't match." << std::endl;
                        std::cout << "Try again." << std::endl;
                        pinTries++;
                    }
                    if ( pinTries == 3 ){
                        clearScreen();
                        printBorderLine();
                        std::cout << "Incorrect PIN was input 3 times!" << std::endl;
                        std::cout << "Try again later. D':" << std::endl;
                        break;
                    }
                } while ( testPin != decryptPin( encryptedPin ) );

                if ( testPin == decryptPin( encryptedPin ) ){
                    clearScreen();
                    if ( updateFileLine( "sys/", "user-pins.txt", line, std::to_string( encryptedPin ) ) == true ){
                        if ( confirmTargetLine( "user-pins.txt", line, std::to_string( pin ) ) == true ){
                            printBorderLine();
                            std::cout << "Success! PIN added to line " << line + 1 << " of ' sys/user-pins.txt '." << std::endl;
                            std::cout << "PIN set for user [ " << userName << " ]." << std::endl;
                            welcomeUser( userName );
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            userVault( line );
                            pinGet = true;
                        }
                        else{
                            printBorderLine();
                            std::cout << "Error! PIN not found on line " << line << " of ' sys/user-pins.txt '." << std::endl;
                        }
                    }
                    else {
                        printBorderLine();
                        std::cout << "Error adding PIN to ' sys/user-pins.txt ' ! " << std::endl;
                    }
                }
                else if ( pinTries == 3 ){
                    int choice{};
                    do {
                        printBorderLine();
                        std::cout << "PIN was unable to be verified!" << std::endl;
                        std::cout << "Would you like to [1] Try again or [2] Quit: ";
                        if ( choice != 1 && choice != 2){
                            clearScreen();
                            printBorderLine();
                            std::cout << "Invalid input! Input must either be '1' or '2'." << std::endl;
                        }
                    } while ( choice != 1 && choice != 2 );
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
        clearScreen();
        errorRefUser( line );
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return;

}
