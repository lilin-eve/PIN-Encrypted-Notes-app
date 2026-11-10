#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <tuple>
#include <system_error>

bool loopSearch = false;

void clearScreen();

void printBorderLine();

void checkPin( const size_t& line, int promptInput );

bool createDirectory( const std::string& dirName );

std::tuple<bool, size_t, size_t> searchSystemFile( const std::string& fileName, const std::string& searchTarget );

bool deleteFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget );

bool updateFileLine( const std::string& dirName, const std::string& fileName, const size_t& lineTarget, const std::string& newLineContent );

void errorAddUser( const std::string& userName ){
    printBorderLine();
    std::cout << "Error! Unable to add user [ " << userName << " ] to file ' sys/users.txt '." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void successAddUser( const std::string& userName, const size_t& userLine ){
    printBorderLine();
    std::cout << "User [ " << userName << " ] successfully added to line " << userLine + 1 << " of file ' sys/users.txt '." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

/*
bool addUser( const std::string& userName, const size_t& lineTarget ){

    bool addUser{false};

    std::error_code err{};

    std::ifstream getUsersCount{ "users.txt" };
    std::string line{};
    size_t lineTotal{0};

    if ( getUsersCount.is_open() ){
        while (getline( getUsersCount, line )){
            lineTotal++;
        }
        getUsersCount.close();
    }
    else {
        printBorderLine();
        std::cout << "Error! File 'users.txt' unable to be opened.";
    }

    if ( lineTotal < 5 ){
        std::ofstream addUserFile( "users.txt", std::ios::app );
        if ( addUserFile.is_open() ){
            //  std::filesystem::path userPath{ "~/" + userName };
            
            if (std::filesystem::create_directory( userPath, err )){
                std::cout << "Creating local directory '" << userPath << "/'." << std::endl;
                std::cout << "Adding user [ " << userName << " ] to line " << lineTarget << " of 'users.txt'." << std::endl;
                addUserFile << userName + "\n";
                addUser = true;
                std::this_thread::sleep_for( std::chrono::seconds(2) );
            }
            
            if ( !std::filesystem::create_directory( userName, err )){
                if ( std::filesystem::exists( userName )){
                    printBorderLine();
                    std::cout << "Error! Local directory '" << userName << "/' already exists." << std::endl;
                    addUser = false;
                }
                else if ( err ) {
                    printBorderLine();
                    std::cout << "Error! Local directory '" << userName << "/' unable to be created." << std::endl;
                    std::cout << err.message() << std::endl;
                    std::cout << err.value() << std::endl;
                    addUser = false;
                }
            }
            else {
                printBorderLine();
                std::cout << "Creating local directory '" << userName << "/'." << std::endl;
                std::cout << "Adding user [ " << userName << " ] to line " << lineTarget << " of 'users.txt'." << std::endl;
                addUserFile << userName + "\n";
                addUser = true;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
            addUserFile.close();
        }
        else {
            printBorderLine();
            std::cout << "Error! File 'users.txt' unable to be opened.";
            addUser = false;
        }
    }
    else if ( lineTotal == 5 ){
        printBorderLine();
        std::cout << "Operation could not be completed." << std::endl;
        std::cout << "User count already at max(5)." << std::endl;
        addUser = false;
    }

    return addUser;

}

std::tuple<bool,size_t,size_t> searchUserName( const std::string& userName ){

        //  function opens file 'users.txt' as input/read stream 'fileUsersInput'
        //  function creates string 'line' to temp hold each string on each line of 'users.txt'
        //  function creates int values 'lineTotal' & 'lineTarget'
        //  function creates bool 'searchUserName', false by default
        std::ifstream fileUsersInput{ "users.txt" };
        std::string line{};
        size_t lineTotal{0};
        size_t lineTarget{};
        bool searchUserName = false;

        //  if 'fileUsersInput' is open, function enters while loop
        //  'fileUsersInput' reads each line of 'users.txt' and temp stores it in 'line'
        //  at start of each loop 'lineTotal' increases by 1
        //  if 'line' = 'userName', then 'lineTarget' is set to current iteration of 'lineTotal' & 'searchUserName' is set to true
        //  after exit from while loop, function checks to make sure 'fileUsersInput' is closed again
        if ( fileUsersInput.is_open() ){
            while(getline( fileUsersInput, line )){
                if ( line.find( userName ) != std::string::npos ){
                    lineTarget = lineTotal;
                    searchUserName = true;
                }
                lineTotal++;
            } 
            fileUsersInput.close();
        }
        else {
            printBorderLine();
            std::cout << "Error! File 'users.txt' unable to be opened." << std::endl;
        }

        return {searchUserName, lineTotal, lineTarget};

}
*/

bool addUser( const std::string& userName, const size_t& userLine ){

    bool verifyAddUser{false};
    int checksAddUser{0};

    if ( updateFileLine( "sys/", "users.txt", userLine, userName ) == true ){
        checksAddUser++;
    }
    else { 
        return verifyAddUser;
    }

    if ( createDirectory( userName ) == true ){
        checksAddUser++;
    }
    else {
        checksAddUser = 0;
        if ( deleteFileLine( "sys/", "users.txt", userLine ) == true ){
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

std::string getUserName(){

    std::string userName{};

    printBorderLine();
    std::cout << "Enter User-name: ";
    std::cin >> userName;

    return userName;

}

//  definition for called & caller function searchUser()
//  
void searchUser( int promptInput ){

    loopSearch = false;

    clearScreen();
    while(!loopSearch){

        std::string userName{ getUserName() };
        int choice{};

        auto [ foundUserName, lineTotal, lineTarget ] = searchSystemFile( "users.txt", userName );

        clearScreen();
        if ( promptInput == 1 ){
            if ( foundUserName == true ){
                printBorderLine();
                std::cout << "Success! User [ " << userName << " ] found on line " << lineTarget + 1 << " of 'users.txt'." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                checkPin( lineTarget, promptInput );
                loopSearch = true;
            } 
            else {
                do {
                    printBorderLine();
                    std::cout << "No user with that name was found.\n";
                    std::cout << "Would you like to: [1] Search another username, [2] Create a user, or [3] Quit: ";
                    std::cin >> choice; 
                    if ( choice != 1 && choice != 2 && choice != 3 ){
                        printBorderLine(); 
                        std::cout << "Invalid input!" << std::endl;
                        std::cout << "Input must be '1', '2', or '3'." << std::endl;
                    }
                } while ( choice != 1 && choice != 2 && choice != 3 );
                if ( choice == 1 ){
                    clearScreen();
                    loopSearch = false;
                }
                else if ( choice == 2 ){
                    promptInput = 2;
                    clearScreen();
                    if ( addUser( userName, lineTotal ) == true ) {
                        successAddUser( userName, lineTotal );
                        checkPin( lineTotal, promptInput );
                    }
                    else {
                        errorAddUser( userName );
                    }
                }
                else if ( choice == 3 ){
                    loopSearch = true;
                    return;
                }
            }
        }
        else if ( promptInput == 2 ){
            if ( foundUserName == false ){
                clearScreen();
                if ( addUser( userName, lineTotal ) == true){
                    successAddUser( userName, lineTotal );
                    checkPin( lineTotal, promptInput );
                    loopSearch = true;
                }
                else {
                    errorAddUser( userName );
                }   
            }
            else {
                do {
                    printBorderLine();
                    std::cout << "Error! Username already taken." << std::endl;
                    std::cout << "Would you like to: [1] Create another username or [2] Quit: ";
                    std::cin >> choice;
                    if ( choice != 1 && choice != 2 ){
                        std::cout << "Invalid input!" << std::endl;
                        std::cout << "Input must be '1' or '2'." << std::endl;
                    }
                } while ( choice != 1 && choice != 2 );
                clearScreen();
               if ( choice == 1 ){
                loopSearch = false;
               }
               else if ( choice == 2){
                loopSearch = true;
               }

            }
        
        }
    
    }

}
