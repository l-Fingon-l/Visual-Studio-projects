#include "EloDatabase.h"

const int InitialElo = 2000;
const int K = 32;
const int max_players[] = { 256, 16 }; // for the error message
const int arrow_left = 75, arrow_right = 77, arrow_top = 72, arrow_down = 80, Esc = 27;
const std::string default_list = "4 Moon Infi Lyn 120", storage = "result";

EloDatabasePoll::EloDatabasePoll(void) // default constructor
{
	amount_of_players = 0;
	compression_level = 1;                  // 0 = char, 1 = nibble
	std::stringstream s(default_list);
	s >> amount_of_players;
	Players = new std::string[amount_of_players]; // names array created
	for (int i(0); i < amount_of_players; i++) s >> Players[i]; // names array complete
	Elo = new double[amount_of_players]; // ratings array created
	for (int i(0); i < amount_of_players; i++) Elo[i] = InitialElo; // ratings initialised
	AskedPreviously = new int[amount_of_players]; // array for amount of mentions created
	for (int i(0); i < amount_of_players; i++) AskedPreviously[i] = 1;
	amount_of_answers = 0;
	previous_step = new double[4];
	for (int i(0); i < 4; i++) previous_step[i] = 0; // initialised
}

EloDatabasePoll::EloDatabasePoll(std::string& list) // constructor with the list of players provided
{
	amount_of_players = 0;
	compression_level = 1;                  // 0 = char, 1 = nibble
	std::stringstream s(list);
	s >> amount_of_players;
	if (amount_of_players > max_players[compression_level]) compression_level--; // it means strong compression 
	                                                                //won't handle it so we decrease it's level
	if (amount_of_players > max_players[compression_level]) {
		std::cout << "Too much ("
			<< max_players[compression_level] << "+) players! Programme's gonna crush!\n";
		exit(1);
	}
	Players = new std::string[amount_of_players]; // names array created
	for (int i(0); i < amount_of_players; i++) s >> Players[i]; // names array complete
	Elo = new double[amount_of_players]; // ratings array created
	for (int i(0); i < amount_of_players; i++) Elo[i] = InitialElo; // ratings initialised
	AskedPreviously = new int[amount_of_players]; // array for amount of mentions created
	for (int i(0); i < amount_of_players; i++) AskedPreviously[i] = 1;
	amount_of_answers = 0;
	previous_step = new double[4];
	for (int i(0); i < 4; i++) previous_step[i] = 0; // initialised
}

void EloDatabasePoll::EloChange(int p1, int p2, bool winner) // changing the database according
{	                                                         // to the result of a single vote
	double E1 = 1 / (1 + pow(10.0, (Elo[p2] - Elo[p1]) / 400));
	double E2 = 1 / (1 + pow(10.0, (Elo[p1] - Elo[p2]) / 400));
	previous_step[0] = p1;
	previous_step[1] = p2;
	previous_step[2] = Elo[p1];
	previous_step[3] = Elo[p2];    // backing up the previous step
	Elo[p1] = Elo[p1] + K * (!winner - E1);
	Elo[p2] = Elo[p2] + K * (winner - E2);
	AskedPreviously[p1]++;
	AskedPreviously[p2]++;
	amount_of_answers++;
}

int EloDatabasePoll::RollPlayer() // rolls a player considering the previous attempts
{
	int ID = -1, amount = 0;
	for (int i(0); i < amount_of_players; i++) amount += AskedPreviously[i];
	int choice = rand() % amount;
	int i = 0;
	while (choice >= 0) // finding the real ID of the chosen one
	{
		choice -= AskedPreviously[i];
		ID++;
		i++;
	}
	return ID;
}

bool EloDatabasePoll::Request() // creation the question to the respondent and receipt of the result
{
	int ID1 = RollPlayer();
	int ID2 = RollPlayer();
	std::cout << std::endl << amount_of_answers + 1 << ")  "
		<< Players[ID1] << "    or    " << Players[ID2] << std::endl; // a request
	return ReactToAnswer(ID1, ID2, ReceiveAnswer());
}

bool EloDatabasePoll::Request(int p1, int p2) // overloaded Request with arguments
{
	std::cout << std::endl << amount_of_answers + 1 << ")  "
		<< Players[p1] << "    or    " << Players[p2] << std::endl; // a request
	return ReactToAnswer(p1, p2, ReceiveAnswer());
}

int EloDatabasePoll::ReceiveAnswer() // receipt of the answer
{
	int answer = 0;
	do
	{
		int caught = 0;
		while (!caught)
		{
			answer = _getch();
			if (answer == 0 || answer == 224)
			{
				answer = _getch();
				caught++;
			}
			else if (answer == 27)
			{
				std::cout << "Are you really sure to exit?\n";
				std::cout << "Yes, exit!               or                No, back.\n";
				int confirmation = 0;
				int caught_ = 0;
				while (!caught_)
				{
					confirmation = _getch();
					if (confirmation == 0 || confirmation == 224)
					{
						confirmation = _getch();
						if(confirmation == arrow_left || confirmation == arrow_right)
						caught_++;
					}
				}
				if (confirmation == arrow_left) caught++;
				else std::cout << "Now press the answer to the question\n";
			}
		}
	} 
	while (answer == 72 && amount_of_answers == 0); // inability to step back if you haven't submitted any answers yet
	return answer;
}

bool EloDatabasePoll::ReactToAnswer(int p1, int p2, int answer) // reaction to the answer
{
	bool to_continue = true;
	switch (answer)
	{
	case arrow_left:
		EloChange(p1, p2, 0);
		break;
	case arrow_right:
		EloChange(p1, p2, 1);
		break;
	case arrow_top:
		to_continue = StepBack();
		break;
	case arrow_down:
		Skip(p1, p2);
		break;
	case Esc:                      // the exit case
		to_continue = false;
		break;
	}
	return to_continue;
}

bool EloDatabasePoll::StepBack() // back to the previous answer
{
	int id1 = previous_step[0], id2 = previous_step[1];
	AskedPreviously[id1]--;
	AskedPreviously[id2]--;
	Elo[id1] = previous_step[2];
	Elo[id2] = previous_step[3];
	amount_of_answers--;
	return Request(id1, id2); // a repeated request
}

void EloDatabasePoll::Skip(int p1, int p2) // skipping the question
{
	previous_step[0] = p1;
	previous_step[1] = p2;
	previous_step[2] = Elo[p1];
	previous_step[3] = Elo[p2];
	AskedPreviously[p1]++;
	AskedPreviously[p2]++;
	std::cout << "~ The question: " << Players[p1] << " vs " << Players[p2] << " skipped ~\n";
}

void EloDatabasePoll::SaveData() // saving the data to the storage
{
#pragma region _creation of the data to be saved later_

	int* rankings = new int[amount_of_players]; // a storage for a future rankings
	Rankings(rankings);   // receipt of the current rankings
/*
ya daunich.......
	int uncompressedSize = 1, amount = amount_of_players;
	for (int digit(1), int subtract(9); amount > 0; digit++)
	{
		if (amount <= subtract)
		{
			uncompressedSize += (digit + 1) * amount;
			break;
		}
		uncompressedSize += (digit + 1) * subtract ;
		amount -= subtract;
		subtract *= 10;
	}
	uncompressedSize += 1; */
	char* compressedData = 0;
	int size = 0;
	Compress(rankings, compressedData, size, compression_level); // the result of a single survey
	delete[] rankings;
#pragma endregion

#pragma region _save compressed data to disk_

	std::ofstream out;
	out.open(storage, std::ofstream::binary);
	out.write(compressedData, size);
	out.close();
	delete[] compressedData;
#pragma endregion
}

void EloDatabasePoll::Rankings(int* rankings) // current rankings 
{
	// explicit rankings firstly by bubble sorting
	int* elo = new int[amount_of_players]; // for elo
	for (int i(0); i < amount_of_players; i++) elo[i] = Elo[i];
	// instead of 2 full bubbles and n^4 operations, we are having n^2
	for (int j(0); j < amount_of_players; j++)
	{
		int max_elo = 0, id = 0;
		for (int i(0); i < amount_of_players; i++)
		{
			if (elo[i] > max_elo)
			{
				max_elo = elo[i];
				id = i;
			}
		}
		elo[id] = 0;
		rankings[j] = id;
	}
	delete[] elo;
}

void EloDatabasePoll::Compress(int* data, char* &compressedData, int &size, int level) // data compression block
{
	switch (level)
	{
	case 0:
		size = amount_of_players;
		compressedData = new char[amount_of_players];
		for (int i(0); i < amount_of_players; i++)
		{
			compressedData[i] = data[i];
		}
		break;

	case 1:           // each byte contains now 2 numbers from the original array
		size = amount_of_players / 2 + amount_of_players % 2;
		compressedData = new char[size];
		for (int i(0), j(0); i < size; i++) // we'll work with each byte in each iteration
		{
			compressedData[i] = 0b1111;     // clearing these 8 bits
			compressedData[i] &= data[j++]; // the left 4 bits contain the 1-st value
			compressedData[i] <<= 4; // moving the value to the first 4 bits
			if (j == amount_of_players) break; // in case of odd amount of players the last 4 bits are 0s
			compressedData[i] += data[j++]; // the left and the whole byte is filled with useful values
			// they come as: 4bytes(value1)4bytes(value2). And the j is updated as well
		}
		break;
	}
}

EloDatabasePoll::~EloDatabasePoll(void) // destructor saving and encrypting the data input before
{
	SaveData();

	delete[] Players;
	delete[] Elo;
	delete[] AskedPreviously;
	delete[] previous_step;
}