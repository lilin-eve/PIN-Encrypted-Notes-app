#include "checkPin.h"
#include "systemFiles.h"
#include "userFiles.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>


//  global bool value 'sessionOver'
//  'sessionOver' set to false by default
//  when 'sessionOver' is false, userVault() loops, when 'sessionOver' is true, userVault() exits loop
//  
bool sessionOver = false;


//  declaration for called function clearScreen()
void clearScreen();

//  declaration for called function printBorderLine()
void printBorderLine();

//  declaration for called function userSettings()
//  function located in file userSettings.cpp
//  
bool userSettings( const size_t& userLine );


//  definition for called & caller function typeUserNote()
//  function is type pair, returns 2 string vectors to caller function
//  function receives 1 const ref to 1 int value, locally named 'pin'
//  function calls printBorderLine() & encryptDecryptUserFile()
//  
std::pair<std::vector<std::string>, std::vector<std::string>> typeUserNote( const int& pin ){

    std::string fileInput{};
    std::vector<std::string> fileLines{};
    std::vector<size_t> fileLineWordCount{};

    printBorderLine();
    std::cout << " | Type your note below. | Type 'EXIT' followed by new-line(ENTER key) on blank line to finish. | " << std::endl;
    printBorderLine();
    while (true){
        getline( std::cin, fileInput );
        if ( fileInput == "EXIT" ){
            break;
        }
        fileLines.push_back( fileInput );
    }
    std::vector<std::string> fileWords{};
    for ( size_t i = 0; i < fileLines.size(); i++ ){
        //  fileLineCount++;
        size_t lineWordCount{0};
        std::istringstream stream(fileLines[i]);
        while ( stream >> fileInput ){
            lineWordCount++;
            fileWords.push_back( fileInput );
        }
        fileLineWordCount.push_back( lineWordCount );
        lineWordCount = 0;
    }

    std::vector<std::string> fileWordsEncrypt{ encryptDecryptUserFile( fileWords, std::to_string(pin) ) };

    std::vector<std::string> fileLinesEncrypt{};

    size_t beginWordNum{ 0 };

    //  size_t 'i' starts at 1, so as not to include fileLines[0], for some reason first line in vector was always blank, don't know why, couldn't fix it so I just got rid of it, maybe that's the caveman way to do it. but that's what i did. works good now though, doesn't delete note content and gets rid of blank line. :))
    for ( size_t i = 1; i < fileLines.size(); i++ ){
        std::string fileLineEncrypt{};
        for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
            if ( x == 0 ){
                fileLineEncrypt = fileWordsEncrypt[ beginWordNum ] + " ";
            }
            else {
                fileLineEncrypt = fileLineEncrypt + fileWordsEncrypt[ beginWordNum ] + " ";
            }
            beginWordNum++;
        }
        fileLinesEncrypt.push_back(fileLineEncrypt);
        fileLineEncrypt.clear();
    }

    std::vector<std::string> fileWordsDecrypt{ encryptDecryptUserFile( fileWordsEncrypt, std::to_string(pin) ) };

    std::vector<std::string> fileLinesDecrypt{};

    beginWordNum = 0;

    //  'fileLinesDecrypt' sourced from 'fileWordsEncrypt', not 'fileLinesEncrypt' where element 0 was ignored, so element 0 must be ignored again here to get rid of blank line
    for ( size_t i = 1; i < fileLines.size(); i++ ){
        std::string fileLineDecrypt{};
        for ( size_t x = 0; x < fileLineWordCount[i]; x++ ){
            if ( x == 0 ){
                fileLineDecrypt = fileWordsDecrypt[ beginWordNum ] + " ";
            }
            else {
                fileLineDecrypt = fileLineDecrypt + fileWordsDecrypt[ beginWordNum ] + " ";
            }
            beginWordNum++;
        }
        fileLinesDecrypt.push_back(fileLineDecrypt);
        fileLineDecrypt.clear();
    }

    return { fileLinesEncrypt, fileLinesDecrypt };

}

//  definition for called & caller function createUserNote()
//  function is type void, returns no value to caller function
//  function receives 3 const refs to 2 string values, locally named 'userName' & 'fileName', & 1 int value, locally named 'pin'
//  function calls clearScreen(), typeUserNote(), printBorderLine(), & saveUserFile()
//  
void createUserNote( const std::string& userName, const std::string& fileName, const int& pin ){

        clearScreen();
        int choice{};
        std::vector<std::string> fileLines{};
        do {
            auto [ fileLinesEncrypt, fileLinesDecrypt ] =  typeUserNote( pin );
            fileLines = fileLinesEncrypt;
            //  int pin{ getPin( userLine ) };
            //  fileWordsEncrypt = encryptUserNote( fileWords, std::to_string(pin) );
            //  std::vector<std::string> fileWordsDecrypt{ encryptUserNote( fileWords, std::to_string(pin) ) }; 
            clearScreen();
            printBorderLine();
            std::cout << "File ' users/" << userName << "/" << fileName << " ' reads: " << std::endl;
            printBorderLine();
            for ( size_t i = 0; i < fileLinesDecrypt.size(); i++ ){
                //  std::cout << fileWords[i] << std::endl;
                std::cout << "Line [ " << i + 1 << " ] | " << fileLinesDecrypt[i] << std::endl;
            }
            printBorderLine();
            std::cout << "Does this look correct?" << std::endl;
            std::cout << "[1] Yes." << std::endl;
            std::cout << "[2] No." << std::endl;
            std::cout << "[3] Discard & go back." << std::endl;
            printBorderLine();
            std::cout << ">";
            std::cin >> choice;
            if ( choice != 1 && choice != 2 && choice != 3 ){
                clearScreen();
                printBorderLine();
                std::cout << "Invalid input! Input must be '1', '2', or '3'." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        } while ( choice != 1 && choice != 3 );
        if ( choice == 1 ){
            if ( saveFile( "../users/" + userName + "/", fileName, fileLines ) == true ){
                clearScreen();
                printBorderLine();
                std::cout << "Success! Content written to file ' users/" << userName << "/" << fileName << " '." << std::endl;
                return;
            }
            else {
                clearScreen();
                printBorderLine();
                std::cout << "Error! Content unable to be written to file ' users/" << userName << "/" << fileName << " '." << std::endl;
                return;
            }
        }
        else if ( choice == 3 ){
            clearScreen();
            return;
        }  


}

//  definition for called & caller function editUserNote()
//  function is type void, returns no value to caller funciton
//  function receives 4 const refs to 2 string values, locally named 'userName' & 'fileName', 1 int value, locally named 'pin', & 1 string vector, locally named 'fileLines'
//  function calls clearScreen(), printBorderLine(), typeUserNote(), & saveUserFile()
//  
void editUserNote( const std::string& userName, const std::string& fileName, const int& pin, const std::vector<std::string>& fileLines ){

    int choice{};

    std::vector<std::string> fileLinesNew{};
    clearScreen();
    do {
        printBorderLine();
        std::cout << "File ' users/" << userName << "/" << fileName << " ' currently reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        auto [ fileLinesEncrypt, fileLinesDecrypt ] = typeUserNote(pin);
        fileLinesNew = fileLinesEncrypt;
        clearScreen();
        printBorderLine();
        std::cout << "File ' users/" << userName << "/" << fileName << " ' previously read: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        printBorderLine();
        std::cout << "File ' users/" << userName << "/" << fileName << " ' now reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLinesDecrypt.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLinesDecrypt[i] << std::endl;
        }
        printBorderLine();
        std::cout << "Does this look correct?" << std::endl;
        std::cout << "[1] Yes." << std::endl;
        std::cout << "[2] No." << std::endl;
        std::cout << "[3] Discard & go back." << std::endl;
        printBorderLine();
        std::cout << ">";
        std::cin >> choice;
        if ( choice != 1 && choice != 2 && choice != 3 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input! Input must be '1', '2', or '3'." << std::endl;
        }
    } while ( choice != 1 && choice != 3 );
        clearScreen();
       if ( choice == 1 ){
        if ( saveFile( "../users/" + userName + "/", fileName, fileLinesNew ) == true ){
                    printBorderLine();
                    std::cout << "Success! Content written to file ' users/" << userName << "/" << fileName << " '." << std::endl;
                }
                else {
                    printBorderLine();
                    std::cout << "Error! Content unable to be written to file ' users/" << userName << "/" << fileName << " '." << std::endl;
                }
    }

    return;
    
}

//  definition for called & caller function readUserNote()
//  function is type void, returns no value to caller function
//  function receives 4 const refs to 2 string values, locally named 'userName' & 'fileName', 1 int value, locally named 'pin', & 1 string vector, locally named 'fileLines'
//  function calls clearScreen(), printBorderLine(), & editUserNote()
//  
void readUserNote( const std::string& userName, const std::string& fileName, const int& pin, const std::vector<std::string>& fileLines ){

    int choice{};

    clearScreen();
    do {
        printBorderLine();
        std::cout << "File ' users/" << userName << "/" << fileName << " ' reads: " << std::endl;
        printBorderLine();
        for ( size_t i = 0; i < fileLines.size(); i++ ){
            std::cout << "Line [ " << i + 1 << " ] | " << fileLines[i] << std::endl;
        }
        printBorderLine();
        std::cout << "Would you like to: " << std::endl;
        std::cout << "[1] Edit file." << std::endl;
        std::cout << "[2] Go back to " << userName << "'s vault." << std::endl;
        printBorderLine();
        std::cout << ">";
        std::cin >> choice;
        if ( choice != 1 && choice != 2 ){
            clearScreen();
            printBorderLine();
            std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
        }
    } while ( choice != 1 && choice != 2 );

    if ( choice == 1 ){
        editUserNote( userName, fileName, pin, fileLines );
        return;
    }
    else if ( choice == 2 ){
        clearScreen();
        return;
    }
    

}

//  definition for called function getNoteName()
//  function is type string, returns string value to caller function
//  
std::string getNoteName(){

    std::string fileName{};

    printBorderLine();
    std::cout << "[ excluding file extension e.g. '.txt' ] | Enter note name: ";
    std::cin >> fileName;

    fileName = fileName + ".txt";

    return fileName;

}

//  definition for called & caller function userVault()
//  function is type void, returns no value to caller function
//  function receives 1 const ref to 1 size_t value, locally named 'userLine'
//  function calls clearScreen(), getFileLine(), printBorderLine(), getNoteName(), getUserPin(), getUserFile(), createUserNote(), editUserNote(), readUserNote(), errorSearchFile(), deleteUserFile(), userSettings(), & errorRefUser()
//  
void userVault( const size_t& userLine ){

    sessionOver = false;

    clearScreen();
    while (!sessionOver){
        auto [ gotFileLine, lineContent ] = getFileLine( "users.txt", userLine );
        if ( gotFileLine == true ){
            std::string userName{ lineContent };

            int choice{};
            do {
                printBorderLine();
                std::cout << "Hello, " << userName << ". :)" << std::endl;
                std::cout << "Pick menu option: " << std::endl;
                printBorderLine();
                std::cout << "[1] Write a new note." << std::endl;
                std::cout << "[2] Read an existing note." << std::endl;
                std::cout << "[3] Edit an existing note." << std::endl;
                std::cout << "[4] Delete an existing note." << std::endl;
                std::cout << "[5] Open user settings." << std::endl;
                std::cout << "[6] Exit vault." << std::endl;
                printBorderLine();
                std::cout << ">";
                std::cin >> choice;
                if ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 ){
                    clearScreen();
                    printBorderLine();
                    std::cout << "Invalid input! Input must be '1', '2', '3', '4', '5', or '6'." << std::endl;
                } 
            } while ( choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 );

            int choice_1{};
            clearScreen();
            if ( choice < 5 ){
                std::string fileName{ getNoteName() };
                auto [ verifyPin, pin ] = getUserPin( userLine );
                if ( verifyPin == true ){
                    auto [ gotFileContent, fileLines ] = getUserFile( userName, fileName, pin );
                    if ( choice == 1 ){
                        if ( !gotFileContent ){
                            createUserNote( userName, fileName, pin );
                        }
                        else {
                            clearScreen();
                            do {
                                printBorderLine();
                                std::cout << "File ' users/" << userName << "/" << fileName << " ' already exists." << std::endl;
                                std::cout << "Would you like to: " << std::endl;
                                std::cout << "[1] Edit existing file." << std::endl;
                                std::cout << "[2] Go back." << std::endl;
                                printBorderLine();
                                std::cout << ">";
                                std::cin >> choice_1;
                                if ( choice_1 != 1 && choice_1 != 2 ){
                                    clearScreen();
                                    printBorderLine();
                                    std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
                                }
                            } while ( choice_1 != 1 && choice_1 != 2 );
                            if ( choice_1 == 1 ){
                                editUserNote( userName, fileName, pin, fileLines );
                            }
                            else if ( choice_1 == 2 ) {
                                clearScreen();
                            }
                        }
                    }
                    else if ( choice == 2 ) {
                        if ( gotFileContent == true ){
                            readUserNote( userName, fileName, pin, fileLines );
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    }
                    else if ( choice == 3 ){
                        if ( gotFileContent == true ){
                            editUserNote( userName, fileName, pin, fileLines );
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    }
                    else if ( choice == 4 ){
                        if ( gotFileContent == true ){
                            do {
                                printBorderLine();
                                std::cout << "Are you sure you want to delete file ' users/" << userName << "/" << fileName << " ' ? " << std::endl;
                                std::cout << "[1] Yes. " << std::endl;
                                std::cout << "[2] No. " << std::endl;
                                printBorderLine();
                                std::cout << ">";
                                std::cin >> choice_1;
                                if ( choice_1 != 1 && choice_1 != 2 ){
                                    clearScreen();
                                    printBorderLine();
                                    std::cout << "Invalid input! Input must be '1' or '2'." << std::endl;
                                }
                            } while ( choice_1 != 1 && choice_1 != 2 );
                            clearScreen();
                            if ( choice_1 == 1 ){
                                if ( deleteUserFile( userName, fileName ) == true ){
                                    printBorderLine();
                                    std::cout << "File ' users/" << userName << "/" << fileName << " ' successfully deleted." << std::endl;
                                }
                                else {
                                    printBorderLine();
                                    std::cout << "Error! File ' users/" << userName << "/" << fileName << " ' unable to be deleted." << std::endl;
                                }
                            }
                        }
                        else {
                            errorSearchFile( userName, fileName );
                        }
                    
                    }
                }
            
            }
            else if ( choice == 5 ){
                if ( userSettings( userLine ) == false ){
                    sessionOver = true;
                }
            }
            else if ( choice == 6 ){
                sessionOver = true;
            }

        }
        else {
            errorRefUser( userLine );
            std::this_thread::sleep_for(std::chrono::seconds(3));
            sessionOver = true;
        }

    }
    

    return;
    
}
