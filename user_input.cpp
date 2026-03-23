#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    // Must have exactly one argument
    if (argc != 2)
    {
        std::cout << "USAGE: ./user_input.exe <name>" << std::endl;
        return 1;
    }

    std::string name = argv[1];

    std::cout << "Hello " << name << ". Do you want to play a game?" << std::endl;

    std::string response;
    std::cin >> response;

    // Validate input
    if (response != "yes" && response != "Yes" &&
        response != "no" && response != "No")
    {
        std::cout << "Invalid input" << std::endl;
        return 2;
    }

    // If user says no, exit normally
    if (response == "no" || response == "No")
    {
        return 0;
    }

    // Print Your Fleet
    std::cout << "Your Fleet" << std::endl;

    for (int i = 0; i <= 10; i++)
        std::cout << i << " ";
    std::cout << std::endl;

    for (char row = 'A'; row <= 'J'; row++)
        std::cout << row << std::endl;

    // Print Enemy Waters
    std::cout << "Enemy Waters" << std::endl;

    for (int i = 0; i <= 10; i++)
        std::cout << i << " ";
    std::cout << std::endl;

    for (char row = 'A'; row <= 'J'; row++)
        std::cout << row << std::endl;

    return 0;
}
