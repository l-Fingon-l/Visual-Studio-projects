#include "EloDatabase.h"

const int InitialElo = 2000;
const int K = 32;
const int arrow_left = 75, arrow_right = 77, arrow_top = 72, arrow_down = 80, Esc = 27;
//const std::string default_list = "5 Moon Infi Lyn 120 Fly100% ReminD";
const char* default_list = {
#include "players.txt"
}

EloDatabasePoll::EloDatabasePoll(void) // default constructor
{
	amount_of_players = 0;
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

EloDatabasePoll::EloDatabasePoll(std::string &list) // constructor with the list of players provided
{
	amount_of_players = 0;
	std::stringstream s(list);
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
    std::cout << std::endl << amount_of_answers << ")  "
		<< Players[ID1] << "    or    " << Players[ID2] << std::endl; // a request
	return ReactToAnswer(ID1, ID2, ReceiveAnswer());
}

bool EloDatabasePoll::Request(int p1, int p2) // overloaded Request with arguments
{
	std::cout << std::endl << amount_of_answers << ")  "
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
			else if (answer == 27) caught++;
		}
	}
	while (answer == 72 && amount_of_answers == 0)
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
		StepBack();
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

void EloDatabasePoll::StepBack() // back to the previous answer
{
	int id1 = previous_step[0], id2 = previous_step[1];
	AskedPreviously[id1]--;
	AskedPreviously[id2]--;
	Elo[id1] = previous_step[2];
	Elo[id2] = previous_step[3];
	amount_of_answers--;
	PollRequest(id1, id2); // a repeated request
}

void EloDatabasePoll::Skip(int p1, int p2) // skipping the question
{
	previous_step[0] = p1;
	previous_step[1] = p2;
	previous_step[2] = Elo[p1];
	previous_step[3] = Elo[p2];
	AskedPreviously[p1]++;
	AskedPreviously[p2]++;
	std::cout << "~ The question: " << Players[p1] << " vs " << Players[p2] << " skipped ~/n";
}

EloDatabasePoll::~EloDatabasePoll(void) // destructor saving and encrypting the data input before
{


	delete[] Players;
	delete[] Elo;
	delete[] AskedPreviously;
	delete[] previous_step;
} 

struct Save
{

};