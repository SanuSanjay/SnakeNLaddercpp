#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>

int main()
{
    std::cout << "Hello World!\n";
    int Board[100];

    for (int i = 0; i < 100; i++)
    {
        Board[i] = i + 1;
    }

    int No_Of_Players_Count;
    std::cout << "Enter the number of players: ";
    std::cin >> No_Of_Players_Count;

    // ANSI color codes
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[91m";
    const std::string GREEN = "\033[92m";
    const std::string YELLOW = "\033[93m";

    
    std::vector<int> Player_Positions(No_Of_Players_Count, 0);


    std::vector<int> Player_Turns(No_Of_Players_Count, 0);

    
    std::vector<std::string> Player_Names(No_Of_Players_Count);
    for (int i = 0; i < No_Of_Players_Count; i++)
    {
        std::cout << "Enter name of player " << i + 1 << ": ";
		std::getline(std::cin >> std::ws, Player_Names[i]);
    }

    std::vector<int> Snake_Heads = { 17, 52, 57, 88, 95 };
    std::vector<int> Snake_Tails = { 13, 29, 40, 18, 51 };

    std::vector<int> Ladder_Bases = { 3, 8, 28, 58, 75, 80 };
    std::vector<int> Ladder_Tops = { 21, 30, 84, 77, 86, 100 };

    srand(static_cast<unsigned int>(time(0)));
    std::cout << "Welcome to Snake and Ladder Game!" << std::endl;
    std::cout << "Players: ";
    for (const auto& name : Player_Names) {
        std::cout << name << " " << '\n';
    }
    std::cout << std::endl;

    bool game_over = false;

    while (!game_over)
    {
        
        for (int i = 0; i < 100; i++)
        {
            bool printed = false;

            
            for (int p = 0; p < No_Of_Players_Count; p++)
            {
                if (Player_Positions[p] == Board[i])
                {
                    std::cout << YELLOW << std::setw(3) << "P" << (p + 1) << RESET << " ";
                    printed = true;
                    break;
                }
            }

            if (!printed)
            {
                
                if (std::find(Snake_Heads.begin(), Snake_Heads.end(), Board[i]) != Snake_Heads.end()) {
                    std::cout << RED << std::setw(3) << "Sh" << RESET << " ";
                }
                
                else if (std::find(Snake_Tails.begin(), Snake_Tails.end(), Board[i]) != Snake_Tails.end()) {
                    std::cout << RED << std::setw(3) << "St" << RESET << " ";
                }
               
                else if (std::find(Ladder_Bases.begin(), Ladder_Bases.end(), Board[i]) != Ladder_Bases.end()) {
                    std::cout << GREEN << std::setw(3) << "Ls" << RESET << " ";
                }
               
                else if (std::find(Ladder_Tops.begin(), Ladder_Tops.end(), Board[i]) != Ladder_Tops.end()) {
                    std::cout << GREEN << std::setw(3) << "Le" << RESET << " ";
                }
                
                else {
                    std::cout << std::setw(4) << Board[i] << " ";
                }
            }

            if ((i + 1) % 10 == 0) {
                std::cout << "\n\n";
            }
        }

        int input;
        for (int i = 0; i < No_Of_Players_Count; i++)
        {
            std::cout << "Enter 1 to roll the dice for " << Player_Names[i] << ": ";
            std::cin >> input;

            while (input != 1) {
                std::cout << "Invalid input! Please enter 1 to roll the dice." << std::endl;
                std::cin >> input;
            }

            int dice_roll = rand() % 6 + 1;
            Player_Turns[i]++;

            std::cout << Player_Names[i] << " rolled a " << dice_roll << std::endl;
            if (Player_Positions[i] + dice_roll > 100) {
                std::cout << Player_Names[i] << " cannot move, as they are at position " << Player_Positions[i] << " and the dice roll makes the position greater than 100." << std::endl;
            }
            else {
                Player_Positions[i] += dice_roll;
                std::cout << Player_Names[i] << " is now at position " << Player_Positions[i] << std::endl;

                for (int j = 0; j < Snake_Heads.size(); j++) {
                    if (Player_Positions[i] == Snake_Heads[j]) {
                        Player_Positions[i] = Snake_Tails[j];
                        std::cout << Player_Names[i] << " encountered a snake! Moved down to " << Player_Positions[i] << std::endl;
                    }
                }

                for (int j = 0; j < Ladder_Bases.size(); j++) {
                    if (Player_Positions[i] == Ladder_Bases[j]) {
                        Player_Positions[i] = Ladder_Tops[j];
                        std::cout << Player_Names[i] << " climbed a ladder! Moved up to " << Player_Positions[i] << std::endl;
                    }
                }

                if (Player_Positions[i] == 100) {
                    std::cout << Player_Names[i] << " has won the game in " << Player_Turns[i] << " turns!" << std::endl;
                    game_over = true;
                    break;
                }
            }
        }
    }

    return 0;
}
