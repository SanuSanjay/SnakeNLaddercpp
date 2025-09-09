#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include<chrono>
#include<thread>
#include <algorithm>
#include<stdexcept>
#include<limits>

// Need access to the board globally

int board[100];

class GameDetails
{
private:
	// Locations of snakes and ladders
	std::vector<int> ladder_base_ = { 3, 8, 28, 37, 58, 75, 80 };
	std::vector<int> ladder_end_ = { 21, 30, 84, 67, 77, 86, 96 };

	std::vector<int> snake_head_ = { 17, 52, 57, 79, 88, 95 };
	std::vector<int> snake_tail_ = { 13, 29, 40, 25, 18, 51 };

	// Board colors
	const std::string RESET_ = "\033[0m";
	const std::string RED_ = "\033[91m";
	const std::string GREEN_ = "\033[92m";
	const std::string YELLOW_ = "\033[93m";

	// Colors for players
	const std::string MAGENTA_ = "\033[35m";
	const std::string CYAN_ = "\033[36m";
	const std::string ORANGE_ = "\033[38;2;255;165;0m";
	const std::string PINK_ = "\033[38;2;255;192;203m";

	std::vector<std::string> colors_ = { ORANGE_, MAGENTA_, CYAN_, PINK_ };

public:
	void print_players(const std::vector<std::string>& player_names);

	void print_board(int board[], int num_players, std::vector<int>& player_positions);

	bool process_player_turn(int num_players, std::vector<int>& player_positions,
		std::vector<int>& player_turns, std::vector<std::string>& player_names);

	bool check_for_cheats(char is_cheat, std::vector<std::string>& player_names,
		std::vector<int>& player_positions, std::vector<int>& player_turns, int current_player);

	std::string to_lower_string(std::string& temp);

	void is_number_of_players_valid(int* num_players);

	int dice_roll(std::vector<int>& player_positions, std::vector<int>& player_turns, std::vector<std::string>& player_names, int current_player);

	bool play_again();
};

std::string GameDetails::to_lower_string(std::string& temp)
{
	for (char& ch : temp) {
		ch = std::tolower(static_cast<unsigned char>(ch));
	}
	return temp;
}


bool GameDetails::play_again()
{
	std::string option;
	std::cout << "Enter 'restart' to play again or 'exit' to quit: ";
	std::getline(std::cin >> std::ws, option);

	while (to_lower_string(option) != "exit" && to_lower_string(option) != "restart") {
		std::cout << "Invalid option! Please re-enter: ";
		std::getline(std::cin >> std::ws, option);
	}

	if (to_lower_string(option) == "exit") {
		std::cout << "Game Over!" << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return false; // end game
	}

	std::cout << "\nGame restarted!\n\n";
	return true; // replay
}

void GameDetails::is_number_of_players_valid(int* num_players)
{
	std::cin.exceptions(std::ios::failbit);
retry:
	try
	{
		std::cin >> *num_players;
		if (*num_players >= 1 && *num_players <= 4)
			return;
		else
			throw std::ios_base::failure("Invalid input");
	}
	catch (const std::ios_base::failure& e)
	{
		std::cerr << "Invalid input. Please enter a number between (1 - 4).\n";
		//std::cout << e.what() << '\n'; use this if you want to see the error message
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		goto retry;
	}
}

void GameDetails::print_players(const std::vector<std::string>& player_names)
{
	int player_number = 1;
	for (const auto& name : player_names)
	{
		std::cout << colors_[player_number - 1] << "Player " << player_number
			<< " " << name << RESET_ << '\n';
		++player_number;
	}
	std::cout << "\n\n";
}

void GameDetails::print_board(int board[], int num_players, std::vector<int>& player_positions)
{
	for (int i = 0; i < 100; i++)
	{
		bool is_printed = false;

		for (int p = 0; p < num_players; p++)
		{
			if (player_positions[p] == board[i])
			{
				std::cout << colors_[p] << std::setw(4) << "P" << (p + 1) << RESET_ << " ";
				is_printed = true;
				break;
			}
		}

		if (!is_printed)
		{
			if (std::find(snake_head_.begin(), snake_head_.end(), board[i]) != snake_head_.end())
				std::cout << RED_ << std::setw(3) << "SH" << RESET_ << " ";
			else if (std::find(snake_tail_.begin(), snake_tail_.end(), board[i]) != snake_tail_.end())
				std::cout << RED_ << std::setw(3) << "ST" << RESET_ << " ";
			else if (std::find(ladder_base_.begin(), ladder_base_.end(), board[i]) != ladder_base_.end())
				std::cout << GREEN_ << std::setw(3) << "LS" << RESET_ << " ";
			else if (std::find(ladder_end_.begin(), ladder_end_.end(), board[i]) != ladder_end_.end())
				std::cout << GREEN_ << std::setw(3) << "LE" << RESET_ << " ";
			else
				std::cout << std::setw(4) << board[i] << " ";
		}

		if ((i + 1) % 10 == 0)
			std::cout << "\n\n";
	}
}

bool GameDetails::check_for_cheats(char is_cheat, std::vector<std::string>& player_names,
	std::vector<int>& player_positions, std::vector<int>& player_turns, int current_player)
{
	switch (tolower(is_cheat))
	{
	case 'g':
		// I mean its in the name
		std::cout << "\nGOD MODE ACTIVATED!\n";
		std::cout << player_names[current_player] << " has reached position 100!\n";
		std::cout << colors_[current_player] << std::setw(4) << "P" << (current_player + 1) << RESET_ << " ";
		std::cout << colors_[current_player] << player_names[current_player] << " has won the game in " << player_turns[current_player] << " turns!" << RESET_ << "\n\n";
		player_positions[current_player] = 100;
		print_board(board, (int)player_names.size(), player_positions);
		std::cout << "Congratulations " << player_names[current_player] << "!\n";
		return false; // end game
		break;

	case 'r':
		// Sets every player's position to 0 except the current player
		std::cout << "\nRESETTING ALL PLAYERS TO START POSITION!\n";
		for (int i = 0; i < player_positions.size(); i++)
		{
			if (i != current_player)
			{
				player_positions[i] = 0;
				std::cout << player_names[i] << " has been reset to position 0.\n";
			}
		}
		print_board(board, (int)player_names.size(), player_positions);
		return true;
		break;

	default:
		return false;
		break;
	}
}

int GameDetails::dice_roll(std::vector<int>& player_positions, std::vector<int>& player_turns, std::vector<std::string>& player_names, int current_player)
{
	std::cout << colors_[current_player] << "Rolling Dice!" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	int dice = rand() % 6 + 1;
	std::cout << player_names[current_player] << " rolled : " << dice << '\n';

	if (dice == 6)
	{
		char player_input = '\0';
		std::cout << "You rolled a 6! You get to roll again " <<'\n';
		player_positions[current_player] += dice;
		player_turns[current_player] += 1;

		while (tolower(player_input) != 'l')
		{
			std::cout << "Please input l or L to roll the dice\n";
			std::cin >> player_input;
		}

		std::cout << "Rolling Dice!" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		dice = rand() % 6 + 1;
		std::cout << "You rolled : " << dice << '\n';
	}

	player_positions[current_player] += dice;
	player_turns[current_player] += 1;
	return dice;
}


bool GameDetails::process_player_turn(int num_players, std::vector<int>& player_positions,
	std::vector<int>& player_turns, std::vector<std::string>& player_names)
{
	char player_input;
	for (int i = 0; i < num_players; i++)
	{
		std::cout << colors_[i] << "Player " << (i + 1) << " " << player_names[i] << "'s turn." << '\n';
		std::cout << "Enter l or L to roll the dice ";
		std::cin >> player_input;

		if (check_for_cheats(player_input, player_names, player_positions, player_turns, i) == true)
		{
			std::cout << colors_[i] << player_names[i] << "'s turn again" << '\n';
			std::cout << "Enter l or L to roll the dice\n";
			std::cin >> player_input;
		}

		if (player_positions[i] == 100)
		{
			// if the player has already won,end current game loop
			std::cout << RESET_;
			return true;
		}


		while (tolower(player_input) != 'l')
		{
			std::cout << "Please input l or L to roll the dice\n";
			std::cin >> player_input;
		}

		int dice = dice_roll(player_positions, player_turns, player_names, i);

		if (player_positions[i] > 100)
		{
			std::cout << colors_[i] << "Position out of the board, resetting to previous location\n" <<RESET_;
			player_positions[i] -= dice;
			player_turns[i] -= 1;
		}

		std::cout << colors_[i] << player_names[i] << " is now at position " << player_positions[i] << RESET_ << "\n\n";

		for (int j = 0; j < snake_head_.size(); j++) {
			if (player_positions[i] == snake_head_[j]) {
				player_positions[i] = snake_tail_[j];
				std::cout << colors_[i] << player_names[i] << " encountered a snake! Moved down to " << player_positions[i] << RESET_ << '\n';
				std::cout << '\n';
			}
		}

		for (int j = 0; j < ladder_base_.size(); j++) {
			if (player_positions[i] == ladder_base_[j]) {
				player_positions[i] = ladder_end_[j];
				std::cout << colors_[i] << player_names[i] << " climbed a ladder! Moved up to " << player_positions[i] << RESET_ << '\n';
				std::cout << '\n';
			}
		}

		if (player_positions[i] == 100) {
			std::cout << colors_[i] << std::setw(4) << "P" << (i + 1) << RESET_ << " ";
			std::cout << '\n';
			print_board(board, num_players, player_positions);
			std::cout << colors_[i] << player_names[i] << " has won the game in " << player_turns[i] << " turns!\n";
			std::cout << "Congratulations " << player_names[i] << RESET_ << "!\n";
			return true;
		}
	}
	return false;
}

void game_loop()
{
	while (true)
	{
		GameDetails game;

		int num_players = 0;
		std::vector<std::string> player_names;

		for (int i = 0; i < 100; i++)
		{
			board[i] = i + 1;
		}

		std::cout << "Welcome to the Snake and Ladder Game!\n";
		std::cout << "Press 'Enter' to start the game\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "\nGame started!\n";
		std::cout << "Enter the number of players (1 - 4)\n";

		game.is_number_of_players_valid(&num_players);

		player_names.resize(num_players);
		std::vector<int> player_positions(num_players, 0);
		std::vector<int> player_turns(num_players, 0);

		std::cout << "Enter the names of the players\n\n";
		for (int i = 0; i < num_players; i++)
		{
			std::cout << "Enter the name of Player " << i + 1 << " ";
			std::getline(std::cin >> std::ws, player_names[i]);
			std::cout << '\n';
		}

		game.print_players(player_names);

		bool game_over = false;
		srand(static_cast<unsigned int>(time(0)));

		while (!game_over)
		{
			game.print_board(board, num_players, player_positions);
			game_over = game.process_player_turn(num_players, player_positions, player_turns, player_names);
		}

		if (!game.play_again())
		{
			break;
		}
	}
}


int main()
{
	game_loop();
	return 0;
}
