#include "checkPin.h"
#include "encryptPin.h"
#include "systemFiles.h"
#include "userFiles.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <thread>
#include <chrono>


bool suspendSettings = false;


//  declaration for called function clearScreen()
void clearScreen();

//  declaration for called function printBorderLine()
void printBorderLine();

//  declaration for called function getUserName()
//  function located in file searchUser.cpp
//  
std::string getUserName();


//  definition for called & caller function changeUserName()
//  function is type bool, returns true or false value to caller function
//  function receives 2 const refs to 1 string value, locally named 'oldUserName', & 1 size_t value, 'userLine'
//  function calls getUserName(), updateFileLine(), & confirmTargetLine()
//  
bool changeUserName( const std::string& oldUserName, const size_t& userLine ){

    bool verifyChangeName{false};

    int checksChangeName{0};

    std::string newUserName{ getUserName() };
    clearScreen();

    const std::filesystem::path oldFilePath{ "../users/" + oldUserName };
    const std::filesystem::path newFilePath{ "../users/" + newUserName };

    if ( updateFileLine( "users.txt", userLine, newUserName ) == true ){
        if ( confirmTargetLine( "users.txt", userLine, newUserName ) == true ){
            checksChangeName++;
        }
        else {
            checksChangeName = 0;
            verifyChangeName = false;
            return verifyChangeName;
        }
    }
    else {
        checksChangeName = 0;
        verifyChangeName = false;
        return verifyChangeName;
    }

    try {
        std::filesystem::rename( oldFilePath, newFilePath );
        checksChangeName++;
    } catch ( const std::filesystem::filesystem_error& err ){
        (void)err;
        if ( updateFileLine( "users.txt", userLine, oldUserName ) == true ){
            checksChangeName = 0;
            verifyChangeName = false;
            return verifyChangeName;
        }
        else {
            checksChangeName = 0;
            verifyChangeName = false;
            return verifyChangeName;
        }
    }

    if ( checksChangeName == 2 ){
        verifyChangeName = true;
    }
    else {
        verifyChangeName = false;
    }

    return verifyChangeName;

}

//  definition for called & caller function changeUserPin()
//  function is type bool, returns true or false value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'userLine'
//  function calls getNewPin(), encryptPin(), clearScreen(), printBorderLine(), decryptPin(), updateFileLine(), & confirmTargetLine()
//  
bool changeUserPin( const size_t& userLine ){

    bool verifyChangePin{false};

    //  const int newPin{ getNewPin() };
    auto [ gotPin, newPin ] = getNewPin();
    const int encryptedPin{ encryptPin(newPin)};
    

    if ( gotPin == true ){
        if ( updateFileLine( "user-pins.txt", userLine, std::to_string(encryptedPin) ) == true ){
            if ( confirmTargetLine( "user-pins.txt", userLine, std::to_string(newPin) ) == true ){
                verifyChangePin = true;
            }
            else {
                verifyChangePin = false;
            }
        }
        else {
            verifyChangePin = false;
        }
    }
    
    return verifyChangePin;

}

//  definition for called & caller function deleteUser()
//  function is type bool, return true or false value to caller function
//  function receives 2 const refs to 1 string value, locally named 'userName', & 1 size_t value, locally named 'userLine'
//  function calls deleteFileLine() & deleteDirectory()
//  
bool deleteUser( const std::string& userName, const size_t& userLine ){

    bool verifyDeleteUser{false};

    int checksDeleteUser{0};

    if ( deleteFileLine( "users.txt", userLine ) == true ){
        checksDeleteUser++;
    }
    else {
        return verifyDeleteUser;
    }

    if ( deleteFileLine( "user-pins.txt", userLine ) == true ){
        checksDeleteUser++;
    }
    else {
        checksDeleteUser = 0;
        return verifyDeleteUser;
    }

    auto [ confirmDeleteDir, filesRemoved ] = deleteDirectory( "../users/" + userName );

    if ( confirmDeleteDir == true ){
        std::cout << "Local directory ' users/" << userName << "/ ' & " << filesRemoved << " files successfully deleted." << std::endl;
        checksDeleteUser++;
    }
    else {
        checksDeleteUser = 0;
        return verifyDeleteUser;
    }

    if ( checksDeleteUser == 3 ){
        verifyDeleteUser = true;
    }

    checksDeleteUser = 0;

    return verifyDeleteUser;

}

//  definition for called & caller function printSystemFile()
//  function is type void, returns no value to caller function
//  function receives 2 const refs to 2 string values, locally named 'dirName' & 'fileName'
//  function calls getSystemFile()
//  
void printSystemFile( const std::string& dirName, const std::string& fileName ){

    auto [ gotSystemFile, fileLines ] = getSystemFile( fileName );

    if ( gotSystemFile == true ){
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << fileLines[i] << std::endl;
        }
    }
    else {
        std::cout << "Error! File ' " << dirName << "/" << fileName << " ' unable to be opened." << std::endl;
    }

}

//  definition for called & caller function purgeUserStorage()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 string value, locally named 'userName'
//  function calls printBorderLine()
//  
void purgeUserStorage( const std::string& userName ){

    const std::filesystem::path dirPath( userName + "/*" );

    try {
        std::uintmax_t filesRemoved = std::filesystem::remove_all( dirPath );
        std::cout << filesRemoved << " files successfully deleted from ' " << userName << "/ '." << std::endl;
    } catch( const std::filesystem::filesystem_error& err ) {
        printBorderLine();
        std::cout << err.what() << std::endl;
    }

}

//  definition for called & caller function userSettings()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'userLine'
//  function calls getFileLine(), clearScreen(), printBorderLine(), printSystemFile(), changeUserName(), errorRefUser(), 
bool userSettings( const size_t& userLine ){

    bool userExists{false};

    suspendSettings = false;

    clearScreen();
    while ( !suspendSettings ){

        auto [ gotFileLine, lineContent ] = getFileLine( "users.txt", userLine );

        std::string userName{ lineContent };
        suspendSettings = false;

        if ( gotFileLine == true ){
            userExists = true;
            int choice{};
            do {
                printBorderLine();
                std::cout << userName << "'s User Settings: " << std::endl;
                printBorderLine();
                std::cout << "[1] Print users." << std::endl;
                std::cout << "[2] Change username." << std::endl;
                std::cout << "[3] Change user PIN." << std::endl;
                std::cout << "[4] Delete user." << std::endl;
                std::cout << "[5] Purge user storage." << std::endl;
                std::cout << "[6] Back." << std::endl;
                printBorderLine();
                std::cout << ">";
                std::cin >> choice;
                if ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 ){
                    clearScreen();
                    printBorderLine();
                    std::cout << "Invalid input!" << std::endl;
                    std::cout << "Input must be '1', '2', '3', '4', or '5'." << std::endl;
                }
            } while ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 );

            clearScreen();
            if ( choice == 1 ){
                printBorderLine();
                printSystemFile( "sys/", "users.txt" ); 
            } 
            else if ( choice > 1 && choice < 6 ){
                if ( choice == 2 ){
                    if ( changeUserName( userName, userLine ) == true ){
                        auto [ gotFileLine, lineContent ] = getFileLine( "users.txt", userLine );
                        if ( gotFileLine == true ){
                            userName = lineContent;
                            printBorderLine();
                            std::cout << "Local directory successfully changed to ' users/" << userName << " ' . " << std::endl;
                            std::cout << "User-name successfully changed to [ " << userName << " ] in ' sys/users.txt '. " << std::endl;
                        }
                        else {
                            errorRefUser( userLine );
                        }
                    }
                    else {
                        std::cout << "Error! User-name unable to be changed." << std::endl;
                    }
                }
                else if ( choice == 3 ){
                    if ( changeUserPin( userLine ) == true ){
                        printBorderLine();
                        std::cout << "PIN successfully changed for user [ " << userName << " ] in ' sys/user-pins.txt ' . " << std::endl;
                    }
                    else {
                        printBorderLine();
                        std::cout << "Error! PIN on line " << userLine << " unable to be changed in ' sys/user-pins.txt ' . " << std::endl;
                    }
                }
                else if ( choice == 4 || choice == 5 ){
                    int choice_1{};
                    clearScreen();
                    do {
                        printBorderLine();
                        std::cout << "Are you sure? "<< std::endl;
                        std::cout << "[1] Yes." << std::endl;
                        std::cout << "[2] No." << std::endl;
                        printBorderLine();
                        std::cout << ">";
                        std::cin >> choice_1;
                        if ( choice_1 != 1 && choice_1 != 2 ){
                            clearScreen();
                            printBorderLine();
                            std::cout << "Invalid input! Input must be either '1' or '2'." << std::endl;
                        }
                    } while ( choice_1 != 1 && choice_1 != 2 );
                    clearScreen();
                    if ( choice_1 == 1 ){
                        printBorderLine();
                        if ( choice == 4 ){
                            if ( deleteUser( userName, userLine ) == true ){
                            std::cout << "User [ " << userName << " ] successfully removed from files ' sys/users.txt ' & ' sys/user-pins.txt '. " << std::endl;
                            std::cout << "Now exiting secure user environment. Returning to main menu . . . " << std::endl;
                            std::this_thread::sleep_for(std::chrono::seconds(3));
                            userExists = false;                        
                            }
                            else {
                                std::cout << "Error! User may not have been properly removed from files ' sys/users.txt ' & ' sys/user-pins.txt ', or directory ' " << userName << " ' may not have been properly deleted." << std::endl;
                            }
                        }
                        else if( choice == 5 ) {
                            purgeUserStorage( userName );
                        }
                    }
                }
            }
            else if ( choice == 6 || userExists == false ){
                suspendSettings = true;
            }
        }
        else {
            errorRefUser( userLine );
            std::this_thread::sleep_for(std::chrono::seconds(3));
            suspendSettings = true;
        }
    }
    
    
    return userExists;

}

void globalSettings(){

    suspendSettings = false;

    clearScreen();
    while (!suspendSettings){
        int choice{};
        do {
            printBorderLine();
            std::cout << "[1] Print users." << std::endl;
            std::cout << "[2] Delete user." << std::endl;
            std::cout << "[3] Purge user storage." << std::endl;
            std::cout << "[4] Go back." << std::endl;
            printBorderLine();
            std::cout << ">";
            std::cin >> choice;
            if ( choice != 1 && choice != 2 && choice != 3 && choice != 4 ){
                clearScreen();
                printBorderLine();
                std::cout << "Invalid input!" << std::endl;
                std::cout << "Input must be '1', '2', '3', or '4'." << std::endl;
            }
        } while ( choice != 1 && choice != 2 && choice != 3 && choice != 4 );

        clearScreen();
        if ( choice == 1 ){
            printBorderLine();
            printSystemFile( "sys/", "users.txt" ); 
        } 
        else if ( choice == 2 || choice == 3 ){
            std::string userName{ getUserName() };
            auto [ foundUserName, lineTotal, lineTarget ] = searchSystemFile( "users.txt", userName );
            if ( foundUserName == true ){
                int choice_1{};
                clearScreen();
                do {
                    printBorderLine();
                    std::cout << "Are you sure? "<< std::endl;
                    std::cout << "[1] Yes." << std::endl;
                    std::cout << "[2] No." << std::endl;
                    printBorderLine();
                    std::cout << ">";
                    std::cin >> choice_1;
                    if ( choice_1 != 1 && choice_1 != 2 ){
                        clearScreen();
                        printBorderLine();
                        std::cout << "Invalid input! Input must be either '1' or '2'." << std::endl;
                    }
                } while ( choice_1 != 1 && choice_1 != 2 );
                clearScreen();
                if ( choice_1 == 1 ){
                    clearScreen();
                    printBorderLine();
                    if ( choice == 2 ){
                        if ( deleteUser( userName, lineTarget ) == true ){
                            std::cout << "User [ " << userName << " ] successfully removed from files ' sys/users.txt ' & ' sys/user-pins.txt '. " << std::endl;                        
                        }
                        else {
                            std::cout << "Error! User may not have been properly removed from files ' sys/users.txt ' & ' sys/user-pins.txt ', or directory ' " << userName << " ' may not have been properly deleted." << std::endl;
                        }
                    }
                    else if ( choice == 3 ){
                        purgeUserStorage( userName );
                    }
                }
            }
            else {
                printBorderLine();
                std::cout << "User [ " << userName << " ] not found in 'users.txt'." << std::endl;  
            }
        }
        else if ( choice == 4 ){
            suspendSettings = true;
        }

    }

}
