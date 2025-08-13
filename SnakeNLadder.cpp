#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<ctime>
#include<algorithm>



int main()
{

	// Setting board to have all locations from 1 to 100
	int Board[100];
	for (int i = 0; i < 100; i++)
	{
		Board[i] = i + 1;
	}

	//Player info
	int No_Of_Players = 0;
	std::vector<std::string> PlayersNames;

	// Locations of snakes and ladders
	std::vector<int> LadderBase = { 3, 8, 28, 37, 58, 75, 80 };
	std::vector<int> LadderEnd = { 21, 30, 84, 67, 77, 86, 96 };

	std::vector<int> SnakeHead = { 17, 52, 57, 79, 88, 95 };
	std::vector<int> SnakeTail = { 13, 29, 40, 25, 18, 51 };

	// Board colors
	const std::string RESET = "\033[0m";
	const std::string RED = "\033[91m";
	const std::string GREEN = "\033[92m";
	const std::string YELLOW = "\033[93m";

	// Colors for players
	const std::string MAGENTA = "\033[35m";
	const std::string CYAN = "\033[36m";
	const std::string ORANGE = "\033[38;2;255;165;0m";
	const std::string PINK = "\033[38;2;255;192;203m";

	std::vector<std::string> Colors = { ORANGE, MAGENTA, CYAN, PINK };


	std::cout << "Welcome to the Snake and Ladder Game!" << std::endl;
	std::cout << "Press 'Enter' to start the game " << std::endl;
	std::cin.ignore();

	std::cout << "\nGame started!\n";

	std::cout << "Enter the number of players (1-4)" << '\n';
	std::cin >> No_Of_Players;

	while (No_Of_Players > 4 || No_Of_Players < 1)
	{
		std::cout << "The number of players is invalid. Please try again!" << '\n';
		std::cin >> No_Of_Players;
	}

	//Resizing vector to store the names of the players
	PlayersNames.resize(No_Of_Players);


	//Two vectors to store each players stats
	std::vector<int> Player_Positions(No_Of_Players, 0);
	std::vector<int> Player_Turns(No_Of_Players, 0);

	std::cout << "Enter the names of the players" << '\n';
	std::cout << '\n';

	for (int i = 0; i < No_Of_Players; i++)
	{
		std::cout << "Enter the name of Player " << i + 1 << " ";
		std::getline(std::cin >> std::ws, PlayersNames[i]);
		std::cout << '\n';
	}

	int PlayerNumber = 1;
	for (const auto& name : PlayersNames)
	{
		std::cout << Colors[PlayerNumber - 1] << "Player " << PlayerNumber << " " << name << RESET << '\n';
		++PlayerNumber;
	}

	std::cout << '\n';
	std::cout << '\n';

	bool GameOver = false;
	srand(static_cast<unsigned int>(time(0)));


	//Game loop
	while (!GameOver)
	{
		for (int i = 0; i < 100; i++)
		{
			bool isPrinted = false;

			for (int p = 0; p < No_Of_Players; p++)
			{
				//P is used to iterate thru the players and i is the current board location to print in
				if (Player_Positions[p] == Board[i])
				{
					std::cout << Colors[p] << std::setw(4) << "P" << (p + 1) << RESET << " ";;
					isPrinted = true;
					break;
				}
			}

			if (!isPrinted) {
				if (std::find(SnakeHead.begin(), SnakeHead.end(), Board[i]) != SnakeHead.end())
				{
					std::cout << RED << std::setw(3) << "SH" << RESET << " ";
				}

				else if (std::find(SnakeTail.begin(), SnakeTail.end(), Board[i]) != SnakeTail.end())
				{
					std::cout << RED << std::setw(3) << "ST" << RESET << " ";
				}

				else if (std::find(LadderBase.begin(), LadderBase.end(), Board[i]) != LadderBase.end())
				{
					std::cout << GREEN << std::setw(3) << "LS" << RESET << " ";
				}

				else if (std::find(LadderEnd.begin(), LadderEnd.end(), Board[i]) != LadderEnd.end())
				{
					std::cout << GREEN << std::setw(3) << "LE" << RESET << " ";
				}

				else
				{
					std::cout << std::setw(4) << Board[i] << " ";
				}
			}

			if ((i + 1) % 10 == 0) {
				std::cout << "\n\n";
			}
		}

		int PlayerInput = 0;
		for (int i = 0; i < No_Of_Players; i++)
		{
			std::cout << "Enter 1 to roll the dice ";
			std::cin >> PlayerInput;

			while (PlayerInput != 1)
			{
				std::cout << "Please input 1 to roll the dice" << '\n';
				std::cin >> PlayerInput;
			}

			int dice = rand() % 6 + 1;
			std::cout << "You rolled :" << dice << '\n';

			Player_Positions[i] += dice;
			Player_Turns[i] += 1;

			if (Player_Positions[i] > 100)
			{
				std::cout << "Position out of the board, resetting your position to previous location" << '\n';
				Player_Positions[i] -= dice;
				Player_Turns[i] -= 1;
			}
			
			std::cout << PlayersNames[i] << " is now at position " << Player_Positions[i] << std::endl;
			std::cout << '\n';

			for (int j = 0; j < SnakeHead.size(); j++) {
				if (Player_Positions[i] == SnakeHead[j]) {
					Player_Positions[i] = SnakeTail[j];
					std::cout << PlayersNames[i] << " encountered a snake! Moved down to " << Player_Positions[i] << std::endl;
				}
			}

			for (int j = 0; j < LadderBase.size(); j++) {
				if (Player_Positions[i] == LadderBase[j]) {
					Player_Positions[i] = LadderEnd[j];
					std::cout << PlayersNames[i] << " climbed a ladder! Moved up to " << Player_Positions[i] << std::endl;
				}
			}

			if (Player_Positions[i] == 100) {
				std::cout << Colors[i] << std::setw(4) << "P" << (i + 1) << RESET << " ";;
				std::cout << PlayersNames[i] << " has won the game in " << Player_Turns[i] << " turns!" << std::endl;
				GameOver = true;
				break;
			}
		}
	}

	return 0;
}
