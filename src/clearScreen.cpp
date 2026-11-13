#include <iostream>

//  definition for called function clearScreen()
//  function is void, returns no value to caller
//  function prints ANSI escape key for clearing console screen output, prints new-line, & function ends, returns to caller
//  
void clearScreen(){

    std::cout << "\033[2J" << std::endl;

}
