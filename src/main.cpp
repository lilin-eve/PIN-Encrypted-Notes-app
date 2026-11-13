#include <iostream>


//  global bool 'programTermination'
//  when 'programTermination' is false, main() loops, when 'programTermination' is true, main() exits loop & returns 0
//
bool programTermination = false;


//  declaration for called function printBorderLine()
void printBorderLine();

//  declaration for called function clearScreen()
void clearScreen();

//  declaration for called function globalSettings()
//  function located in file userSettings.cpp
//  
void globalSettings();

//  declaration for called function initSystemFiles()
//  function located in file updateSystemFile.cpp
//  
bool initSystemFiles();

//  declaration for called function searchUser()
//  function located in file searchUser.cpp
//  
void searchUser( int promptInput );


//  definition for called & caller function menuMain()
//  function is type int, returns int value to caller function
//  function calls clearScreen() & printBorderLine()
//  
int menuMain(){
    
    int menuInput{};

    clearScreen();
    do {
        printBorderLine();
        std::cout << "Hello, this program allows users to store pin-secured notes in their own private vaults." << std::endl;
        printBorderLine();
        std::cout << "[1] Sign-in." << std::endl;
        std::cout << "[2] Create account." << std::endl;
        std::cout << "[3] Vault settings." << std::endl;
        std::cout << "[4] Quit." << std::endl;
        printBorderLine();
        std::cout << ">";
        std::cin >> menuInput;
        if ( menuInput != 1 && menuInput != 2 && menuInput != 3 && menuInput != 4 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input!\n";
            std::cout << "Input must be '1', '2', '3', or '4'.\n";
        }
    } while ( menuInput != 1 && menuInput != 2 && menuInput != 3 && menuInput != 4 );

    return menuInput;

}

//  definition for caller function main()
//  function is type int, returns int value as program status code
//  function calls initSystemFiles(), menuMain(), searchUser(), globalSettings(), & printBorderLine()
//  
int main(){

    if ( initSystemFiles() == true ){
        while (!programTermination){
            int promptInput{ menuMain() };

            if ( promptInput == 1 || promptInput == 2 ){
                searchUser( promptInput );
            }
            else if ( promptInput == 3 ){
                globalSettings();
            }
            else if ( promptInput == 4 ){
                printBorderLine();
                std::cout << "Exiting program . . ." << std::endl;
                programTermination = true;
            }

        }
    }
    else {
        printBorderLine();
        std::cout << "System files directory ' sys/ ', & files ' sys/users.txt ' & ' sys/user-pins.txt ' unable to be initialized & verified properly." << std::endl;
        std::cout << "Program now shutting down . . . " << std::endl;
    }

    return 0;
    
}
