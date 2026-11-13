#include "checkPin.h"
#include "systemFiles.h"
#include "userFiles.h"
#include <iostream>
//  #include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <tuple>
//  #include <system_error>


bool endSearch = false;


//  declaration for called function clearScreen()
void clearScreen();

//  declaration for called function printBorderLine()
void printBorderLine();


//  definition for called & caller function addUser()
//  function is type bool, returns true or false value to caller function
//  function receives 2 const refs to 1 string value, locally named 'userName', & 1 size_t value, locally named 'userLine'
//  function calls updateFileLine() & deleteFileLine()
//  
bool addUser( const std::string& userName, const size_t& userLine ){

    bool verifyAddUser{false};
    int checksAddUser{0};

    const std::filesystem::path filePath{ "../users/" + userName };

    if ( updateFileLine( "users.txt", userLine, userName ) == true ){
        std::cout << "True." << std::endl;
        checksAddUser++;
    }
    else { 
        std::cout << "False.";
        return verifyAddUser;
    }

    if ( std::filesystem::create_directory( filePath ) == true ){
        checksAddUser++;
    }
    else {
        checksAddUser = 0;
        if ( deleteFileLine( "users.txt", userLine ) == true ){
            return verifyAddUser;
        }
        else {
            return verifyAddUser;
        }
    }

    if ( checksAddUser == 2 ){
        verifyAddUser = true;
    }

    return verifyAddUser;

}

//  definition for called & caller function errorAddUser()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 string value, locally named 'userName'
//  function calls printBorderLine()
//  
void errorAddUser( const std::string& userName ){
    printBorderLine();
    std::cout << "Error! Unable to add user [ " << userName << " ] to file ' sys/users.txt '. :/ " << std::endl;
    printBorderLine();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

//  definition for called & caller function errorMaxUsers()
//  function is type void, returns no value to caller function
//  function calls printBorderLine()
//  
void errorMaxUsers(){
    printBorderLine();
    std::cout << "User cannot be added to file ' sys/users.txt '. D': " << std::endl;
    std::cout << "User count already at max(5). Existing user must be deleted to add new user. " << std::endl;
    printBorderLine();
    std::cout << "Now exiting to main menu . . . " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

//  definition for called & caller function successAddUser()
//  function is type void, returns no value to caller function
//  function receives 2 const refs to 1 string value, locally named 'userName', & 1 size_t value, locally named 'userLine'
//  function calls printBorderLine()
//  
void successAddUser( const std::string& userName, const size_t& userLine ){
    printBorderLine();
    std::cout << "User [ " << userName << " ] successfully added to line " << userLine + 1 << " of file ' sys/users.txt '. :D " << std::endl;
    printBorderLine();
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

//  definition for called & caller function getUserName()
//  function is type string, returns string value to caller function
//  function calls printBorderLine()
//  
std::string getUserName(){

    std::string userName{};

    printBorderLine();
    std::cout << "Enter User-name: ";
    std::cin >> userName;

    return userName;

}

//  definition for called & caller function userFound()
//  function is type void, returns no value to caller function
//  function receives 2 const refs to 1 string value, locally named 'userName', & 1 size_t value, locally named 'lineTarget'
//  function calls printBorderLine()
//  
void userFound( const std::string& userName, const size_t& lineTarget ){
    printBorderLine();
    std::cout << "Success! User [ " << userName << " ] found on line " << lineTarget + 1 << " of ' /sys/users.txt '. :D " << std::endl;
    printBorderLine();
    std::this_thread::sleep_for(std::chrono::seconds(3));
} 

//  definition for called & caller function userNotFound()
//  function is type int, returns int value to caller function
//  function calls printBorderLine() & clearScreen()
//  
int userNotFound(){

    int choice{};
    do {
        printBorderLine();
        std::cout << "No user with that name was found." << std::endl;
        std::cout << "Would you like to: [1] Search another username, [2] Create a user, or [3] Exit to main menu: ";
        std::cin >> choice; 
        if ( choice != 1 && choice != 2 && choice != 3 ){
            clearScreen();
            printBorderLine(); 
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Input must be '1', '2', or '3'." << std::endl;
        }
    } while ( choice != 1 && choice != 2 && choice != 3 );

    return choice;

}

//  definition for called & caller function userNameTaken()
//  function is type int, returns int value to caller function
//  function calls printBorderLine() & clearScreen()
//  
int userNameTaken(){

    int choice{};
    do {
        printBorderLine();
        std::cout << "Username already taken. D': " << std::endl;
        std::cout << "Would you like to: [1] Create another username or [2] Exit to main menu: ";
        std::cin >> choice;
        if ( choice != 1 && choice != 2 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Input must be '1' or '2'." << std::endl;
        }
    } while ( choice != 1 && choice != 2 );

    return choice;

}

//  definition for called & caller function searchUser()
//  function is type void, returns no value to caller function
//  function receives 1 int value, locally named 'promptInput'
//  function calls clearScreen(), getUserName(), searchSystemFile(), printBorderLine(), checkPin(), addUser(), successAddUser(), & errorAddUser()
//  
void searchUser( int promptInput ){

    endSearch = false;

    clearScreen();
    while(!endSearch){

        std::string userName{ getUserName() };
        int choice{};

        auto [ foundUserName, lineTotal, lineTarget ] = searchSystemFile( "users.txt", userName );

        clearScreen();
        if ( promptInput == 1 ){
            if ( foundUserName == true ){
                userFound( userName, lineTarget );
                checkPin( lineTarget, promptInput );
                endSearch = true;
            } 
            else {
                choice = userNotFound();
                if ( choice == 1 ){
                    clearScreen();
                    endSearch = false;
                }
                else if ( choice == 2 ){
                    promptInput = 2;
                    clearScreen();
                    if ( addUser( userName, lineTotal ) == true ) {
                        successAddUser( userName, lineTotal );
                        checkPin( lineTotal, promptInput );
                        endSearch = true;
                    }
                    else {
                        errorAddUser( userName );
                        endSearch = true;
                    }
                }
                else if ( choice == 3 ){
                    endSearch = true;
                }
            }
        }
        else if ( promptInput == 2 ){
            if ( foundUserName == false && lineTotal != 5 ){
                clearScreen();
                if ( addUser( userName, lineTotal ) == true){
                    successAddUser( userName, lineTotal );
                    checkPin( lineTotal, promptInput );
                    endSearch = true;
                }
                else {
                    errorAddUser( userName );
                    endSearch = true;
                }   
            }
            else if ( lineTotal == 5 ){
                errorMaxUsers();
                endSearch = true;
            }
            else {
                choice = userNameTaken();
                clearScreen();
                if ( choice == 1 ){
                endSearch = false;
                }
                else if ( choice == 2){
                endSearch = true;
                }

            }
        
        }
    
    }

}
