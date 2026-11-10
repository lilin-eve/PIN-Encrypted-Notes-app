#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <system_error>

bool suspendSettings = false;

void clearScreen();

void printBorderLine();

//  std::tuple<bool,size_t,size_t> searchUserName( const std::string& userName );

std::tuple<bool, size_t, size_t> searchSystemFile( const std::string& fileName, const std::string& searchTarget );

bool confirmPin( const int& pin, const size_t& lineTarget );

bool confirmTargetLine( const std::string& fileName, const size_t& lineTarget, const std::string& searchTarget );

std::pair<bool, std::uintmax_t> deleteDirectory( const std::string& dirName );

bool deleteFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget );

void errorRefUser( const size_t& userLine );

int decryptPin( const int& pin );

int encryptPin( const int& pin );

std::pair<bool, std::string> getFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineNum );

std::pair<bool, std::vector<std::string>> getSystemFile( const std::string& dirName, const std::string& fileName );

std::string getUserName();

int getNewPin();

bool updateFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent );


bool changeUserName( const std::string& oldUserName, const size_t& userLine ){

    bool verifyChangeName{false};

    int checksChangeName{0};

    std::string newUserName{ getUserName() };

    const std::filesystem::path oldFilePath{ oldUserName };
    const std::filesystem::path newFilePath{ newUserName };

    if ( updateFileLine( "sys/", "users.txt", userLine, newUserName ) == true ){
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
        if ( updateFileLine( "sys/", "users.txt", userLine, oldUserName ) == true ){
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


bool changeUserPin( const size_t& userLine ){

    bool verifyChangePin{false};

    const int newPin{ getNewPin() };
    const int encryptedPin{ encryptPin(newPin)};
    
    int testPin{};
    int pinTries{0};

    clearScreen();
    do {
        printBorderLine();
        std::cout << "Confirm PIN: ";
        std::cin >> testPin;
        if ( testPin != decryptPin(encryptedPin) ){
            pinTries++;
            clearScreen();
            printBorderLine();
            std::cout << "Incorrect PIN!" << std::endl;
            std::cout << "Try again." << std::endl;
        }
        if ( pinTries == 3 ){
            break;
        }
    } while ( testPin != decryptPin(encryptedPin) );

    if ( testPin == decryptPin(encryptedPin) ){
        pinTries = 0;
        if ( updateFileLine( "sys/", "user-pins.txt", userLine, std::to_string(encryptedPin) ) == true ){
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
    else if ( pinTries == 3 ){
        pinTries = 0;
        verifyChangePin = false;
        clearScreen();
        printBorderLine();
        std::cout << "Wrong PIN was input 3 times." << std::endl;
        std::cout << "Try again later. D':" << std::endl;
    }
    
    return verifyChangePin;

}


/*
void deleteUser( std::string userName, size_t userLine ){

    int checkDeleteUser{0};

    std::ifstream readUsers{ "users.txt" };
    std::string line{};
    size_t lineNum{0};
    std::vector<std::string> users{};

    clearScreen();
    if ( readUsers.is_open() ){
        while ( getline( readUsers, line )){
            if ( lineNum != userLine ){
                users.push_back(line);
            }
            lineNum++;
        }
        readUsers.close();
        checkDeleteUser++;
    }
    else {
        printBorderLine();
        std::cout << "Error! File 'users.txt' unable to be opened. " << std::endl;
    }

    std::ifstream readUserPins{ "user-pins.txt" };
    lineNum = 0;
    std::vector<std::string> userPins{};

    if ( readUserPins.is_open() ){
        while ( getline( readUserPins, line )){
            if ( lineNum != userLine ){
                userPins.push_back(line);
            }
            lineNum++;
        }
        readUserPins.close();
        checkDeleteUser++;
    }
    else {
        printBorderLine();
        std::cout << "Error! File 'user-pins.txt' unable to be opened. " << std::endl;
    }

    std::ofstream updateUsers{ "users.txt" };

    if ( updateUsers.is_open() ){
        for ( size_t i = 0; i < users.size(); i++ ){
            updateUsers << users[i] + "\n";
        }
        updateUsers.close();
        checkDeleteUser++;
    }
    else {
        printBorderLine();
        std::cout << "Error! File 'user.txt' unable to be opened for editing. " << std::endl;
    }

    std::ofstream updateUserPins{ "user-pins.txt" };

    if ( updateUserPins.is_open() ){
        for ( size_t i = 0; i < userPins.size(); i++ ){
            updateUserPins << userPins[i] + "\n";
        }
        updateUserPins.close();
        checkDeleteUser++;
    }
    else {
        printBorderLine();
        std::cout << "Error! File 'user-pins.txt' unable to be opened for editing." << std::endl;
    }

    try {
        std::uintmax_t filesRemoved = std::filesystem::remove_all( userName );
        printBorderLine();
        std::cout << "Directory ' " << userName << "/ ' & " << filesRemoved - 1 << " files deleted." << std::endl;
        checkDeleteUser++;
    } catch ( const std::filesystem::filesystem_error& err ){
        printBorderLine();
        std::cout << err.what() << std::endl;
    }

    if ( checkDeleteUser == 5 ){
        printBorderLine();
        std::cout << "Files 'users.txt' & 'user-pins.txt' successfully re-written." << std::endl;
        std::cout << "User removed from line [ " << userLine << " ] of above files." << std::endl;
    }
    else {
        printBorderLine();
        std::cout << "Warning! User may not have been properly removed from files 'users.txt' & 'user-pins.txt', or local directory may not have been deleted." << std::endl;
        std::cout << "'verifyDeleteUser' did not pass all checks." << std::endl;
    }

}
*/

bool deleteUser( const std::string& userName, const size_t& userLine ){

    bool verifyDeleteUser{false};

    int checksDeleteUser{0};

    if ( deleteFileLine( "sys/", "users.txt", userLine ) == true ){
        checksDeleteUser++;
    }
    else {
        return verifyDeleteUser;
    }

    if ( deleteFileLine( "sys/", "user-pins.txt", userLine ) == true ){
        checksDeleteUser++;
    }
    else {
        checksDeleteUser = 0;
        return verifyDeleteUser;
    }

    auto [ confirmDeleteDir, filesRemoved ] = deleteDirectory( userName );

    if ( confirmDeleteDir == true ){
        std::cout << "Directory ' " << userName << "/ ' & " << filesRemoved << " files successfully deleted." << std::endl;
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

void printSystemFile( const std::string& dirName, const std::string& fileName ){

    auto [ gotSystemFile, fileLines ] = getSystemFile( dirName, fileName );

    if ( gotSystemFile == true ){
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << fileLines[i] << std::endl;
        }
    }
    else {
        std::cout << "Error! File ' " << dirName << "/" << fileName << " ' unable to be opened." << std::endl;
    }

}

void purgeUserStorage( std::string userName ){

    std::filesystem::path dirPath( userName + "/*" );

    try {
        std::uintmax_t filesRemoved = std::filesystem::remove_all( dirPath );
        std::cout << filesRemoved << " files successfully deleted from ' " << userName << "/ '." << std::endl;
    } catch( const std::filesystem::filesystem_error& err ) {
        printBorderLine();
        std::cout << err.what() << std::endl;
    }

}

void userSettings( const size_t& userLine ){

    auto [ gotFileLine, lineContent ] = getFileLine( "sys/", "users.txt", userLine );

    if ( gotFileLine == true ){

        std::string userName{ lineContent };
        suspendSettings = false;

        clearScreen();
        while (!suspendSettings){
            int choice{};
            do {
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
                        auto [ gotFileLine, lineContent ] = getFileLine( "sys/", "users.txt", userLine );
                        if ( gotFileLine == true ){
                            userName = lineContent;
                            std::cout << "User-name successfully changed to [ " << userName << " ] in ' sys/users.txt '. " << std::endl;
                        }
                        else {
                            errorRefUser( userLine );
                        }
                    }
                    else {

                    }
                }
                else if ( choice == 3 ){
                    if ( changeUserPin( userLine ) == true ){

                    }
                    else {

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
                    if ( choice_1 == 1 ){
                        clearScreen();
                        printBorderLine();
                        if ( choice == 4 ){
                            if ( deleteUser( userName, userLine ) == true ){
                            std::cout << "User [ " << userName << " ] successfully removed from files ' sys/users.txt ' & ' sys/user-pins.txt '. " << std::endl;                        
                            }
                            else {
                                std::cout << "Error! User may not have been properly removed from files ' sys/users.txt ' & ' sys/user-pins.txt ', or directory ' " << userName << " ' may not have been properly deleted." << std::endl;
                            }
                        }
                        else {
                            purgeUserStorage( userName );
                        }
                    }
                    else {
                        clearScreen();
                    }
                }
            }
            else if ( choice == 6 ){
                suspendSettings = true;
            }

        }
    }
    else {
        errorRefUser( userLine );
    }
    
    return;

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
        else if ( choice > 1 && choice < 4 ){
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
                std::string userName{ getUserName() };
                auto [ foundUserName, lineTotal, lineTarget ] = searchSystemFile( "users.txt", userName );
                clearScreen();
                printBorderLine();
                if ( foundUserName == true ){
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
                else {
                    printBorderLine();
                    std::cout << "User [ " << userName << " ] not found in 'users.txt'." << std::endl;
                }   
            }
        }
        else if ( choice == 4 ){
            suspendSettings = true;
        }

    }

}
