#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <conio.h>
#include <windows.h>

//const std::string* messages;

class EloDatabasePoll
{
private:
	int amount_of_players;
	int amount_of_answers;
	int compression_level;
	double* previous_step; // format: {id1, id2, value_id1, value_id2}
	std::string* Players; // names of players presented in chart
	double* Elo; // Elo-ratings of the represented players
	int* AskedPreviously; // amount of time players have been proposed in a question before

	void EloChange(int player1, int player2, bool winner); // changing the database according
														   // to the result of a single vote
	int RollPlayer(); // rolls a player considering the previous attempts
	bool StepBack(); // back to the previous answer
	void Skip(int p1, int p2); // skipping the question
	void SaveData(); // saving the data to the storage

public:
	EloDatabasePoll(void); // default constructor
	EloDatabasePoll(std::string& list); // constructor with the list of players provided

	bool Request(); // creation the question to the respondent and receipt of the result
	bool Request(int p1, int p2);  // overloaded Request with arguments
	int ReceiveAnswer(); // receipt of the answer
	bool ReactToAnswer(int p1, int p2, int answer); // reaction to the answer
	void Rankings(int* rankings); // current rankings
	void Compress(int* data, char* &compressedData, int &size, int level); // data compression block

	~EloDatabasePoll(void); // destructor saving and encrypting the data have been input before
};