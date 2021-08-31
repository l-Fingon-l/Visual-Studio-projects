#include <cstdio>
#include <fstream>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

/////////////////////////////////////////////////////////
// Elo-ratings for Warcraft 3 by Fingon. ////////////////
// Initial creation date: 2020.02.09 ////////////////////
// This build: 2020.02.21 14:08 p.m. ////////////////////
/////////////////////////////////////////////////////////
// Patch-note: 2020.02.21 15:27 p.m.: time data is no ///
// longer in "time" but moved to the "data.time" file.///
///////////////////////////////////////////////////////// 

const int amount = 128, SecondsInDay = 86400;
const int InitialElo = 2000;
const uint16_t InactivityLimit = 30;
int K, real_amount = 0, last_update = 0;
bool report = 1;
#define WRITEFILE "data.txt"
#define READFILE "result.txt"
#define TIMEFILE "data.time"

string getname(ifstream& read)
{
	string result = "";
	char c = 0;
	while ((c == ' ') || (c == '\n') || (c == '\0')) read.get(c);
	read.seekg((int)read.tellg() - 1);
	while (true)
	{
		read.get(c);
		if (read.eof()) break;
		if ((c == ' ') || (c == '\n') || (c == '\0')) break;
		if ((c >= 65) && (c <= 90)) result += c + 32;
		else result += c;
	}
	if (!read.eof())
	{
		read.seekg((int)read.tellg() - 1);
		return result;
	}
	else return result;
}

int get_int32(ifstream& input)
{
	//checking if there is enough data
	int current_position = input.tellg();
	input.seekg(0, input.end);
	if ((int)(input.tellg()) - current_position < 4)
		return -1;
	input.seekg(current_position);

	//reading the data
	char buffer[4];
	uint64_t T = 0;
	for (int i(0); i < 4; i++)
		input.get(buffer[i]);
	for (int i(3); i >= 0; i--)
	{
		T |= (uint8_t)buffer[i] & (uint64_t)0b11111111;
		T <<= 8;
	}
	T >>= 8;
	return T;
}

void put_int32(ofstream& output, uint32_t T)
{
	char buffer[4];
	for (int i(0); i < 4; i++)
	{
		buffer[i] = T & 0b11111111;
		T >>= 8;
	}
	output.write(buffer, 4);
}

int InactivityDecline(double& Elo, int t) // returns ELO after the inactivity period t
{
	int result = 0;
	result = 21 * (Elo / (((double)t + 600)/30 + 1.0));
	return result;
}

int InactivityDeclineReverse(double& Elo, int t) // returns the ELO that was before the inactivity period t
{
	int result = 0;
	result = Elo * (((double)t + 600) / 30 + 1.0) / 21;
	return result;
}

int gettime()
{
	return (((double)time(NULL)) / ((double)SecondsInDay));
}

int Init(string file, string*& Player, double*& Elo, int*& Times)
{
	ifstream read;
	read.open(file, ifstream::in);
	if (read.is_open())
	{
		cout << file << " was successfully opened" << endl;
		read.seekg(9);
		read >> report;
		int pos = read.tellg();
		read.seekg(pos + 5);
		read >> K;
		int i = 0, j = 0;
		while (!read.eof())
		{
			Player[i] = getname(read);
			read >> Elo[i];
			i++;
		}
		read.close();

		//time initialization
		ifstream times;
		times.open(TIMEFILE, ifstream::in | ifstream::binary);
		if (!times.is_open())
		{
			ofstream create(TIMEFILE, ofstream::out);
			cout << TIMEFILE << " was created" << endl;
			create.close();
		}
		else
		{
			times.seekg(0, times.end); // in case the file is empty
			if (times.tellg() < 6)
			{
				cout << times.tellg();
				times.close();
				ofstream create(TIMEFILE, ofstream::out);
				cout << TIMEFILE << " was created" << endl;
				create.close();
			}
			else
			{
				cout << TIMEFILE << " was successfully opened" << endl;
				times.seekg(ifstream::beg);
				char buffer[2];
				last_update = get_int32(times);
				while (!times.eof())
				{
					uint16_t T = 0b11111111;
					times.get(buffer[0]);
					if (times.eof()) break;
					times.get(buffer[1]);
					T &= buffer[1];
					T <<= 8;
					Times[j] = T | (uint8_t)buffer[0];
					j++;
				}
				times.close();
			}
		}
		

		for (int x(0); x < j; x++)
		{
			if ((gettime() - Times[x]) > InactivityLimit)
			{
				double EloBefore = Elo[x];
				if ((double)last_update / SecondsInDay - Times[x] > InactivityLimit) // in case the inactivity decline was updated recently
					Elo[x] = InactivityDeclineReverse(Elo[x], (double)last_update / SecondsInDay - Times[x] - InactivityLimit);
				Elo[x] = InactivityDecline(Elo[x], gettime() - Times[x] - InactivityLimit);
				cout << endl << Player[x] << " is already inactive for " << gettime() - Times[x] << " days. His ELO was decreased by (" <<
					EloBefore - Elo[x] << ").\nNow his ELO is: " << Elo[x] << ".\n\n";
			}
		}

		while (i > j)
		{
			Times[j] = gettime();
			j++;
		}
		return i;
	}
	else cout << file << " was not opened!" << endl;
	return -1;
}

int NameToID(string& name, string*& Player, double*& Elo, int*& Times)
{
	for (int i(0); i < amount; i++) if (name == Player[i]) return i;
	cout << "Player \"" << name << "\" was not found in ratings" << endl;
	Player[real_amount] = name;
	Elo[real_amount] = InitialElo;
	Times[real_amount] = gettime();
	real_amount++;
	return (real_amount - 1);
}

void EloChange(double &Elo1, double &Elo2, bool winner)
{
	double E1 = 1 / (1 + pow(10.0, (Elo2 - Elo1) / 400));
	double E2 = 1 / (1 + pow(10.0, (Elo1 - Elo2) / 400));

	Elo1 = Elo1 + K * (!winner - E1);
	Elo2 = Elo2 + K * (winner - E2);
}

int main()
{
	string Players[amount];
	double Elo[amount];
	int Times[amount];
	string* players = Players;
	double* elo = Elo;
	int* times = Times;
	char c = 0;
	int score1 = 0, score2 = 0;
	real_amount = Init(WRITEFILE, players, elo, times);

	ifstream read(READFILE, ifstream::in);
	if (!read.is_open())
	{
		cout << READFILE << " was not opened!" << endl;
		system("PAUSE");
		return 0;
	}
	else
	{
		//checking if there is enough data
		int current_position = read.tellg();
		read.seekg(0, read.end);
		if ((int)(read.tellg()) - current_position < 5)
		{
			cout << "\nThe information in the " << READFILE << " is incorrect. Please, check it out.\n";
			system("PAUSE");
			return 0;
		}
		else read.seekg(0, read.beg);
	}
	cout << READFILE << " was successfully opened" << endl;
	string player1, player2;
	ofstream write;

#pragma region ReadingTheLine
	while (!read.eof())
	{
		player1 = getname(read);
		read >> score1;
		read >> c;
		read >> score2;
		int pos = read.tellg();
		read.seekg(pos + 1);
		player2 = getname(read);
		cout << "\nThe input results: " << player1 << " " << score1 << ":" << score2 << " " << player2 << endl;
		int ID1 = NameToID(player1, players, elo, times);
		int ID2 = NameToID(player2, players, elo, times);
		double Elo1 = Elo[ID1], Elo2 = Elo[ID2];
#pragma endregion

#pragma region CalculatingTheLine
		for (int rounds(score1 + score2); rounds > 0; rounds--)
		{
			while (score1)
			{
				EloChange(Elo[ID1], Elo[ID2], false);
				Times[ID1] = gettime();
				Times[ID2] = gettime();
				score1--;
				rounds--;
			}
			while (score2)
			{
				EloChange(Elo[ID1], Elo[ID2], true);
				Times[ID1] = gettime();
				Times[ID2] = gettime();
				score2--;
				rounds--;
			}
		}

		//reporting
		if (!report) return 0;
		cout << player1 << "\'s ELO-change is (" << setprecision(4) << (Elo[ID1] - Elo1) << "). Now his ELO is: " << Elo[ID1] << ".\n";
		cout << player2 << "\'s ELO-change is (" << (Elo[ID2] - Elo2) << "). Now his ELO is: " << Elo[ID2] << ".\n";
	}
	read.close();
#pragma endregion
	
#pragma region UpdatingTheDataFile
	write.open(WRITEFILE, ofstream::out);
	write << "report = " << report << endl << "K = " << K << endl;
	for (int x(real_amount), y(0); x > 0; x--, y++) write << endl << Players[y] << " " << (int)Elo[y];
	write.close();
#pragma endregion

#pragma region UpdatingTheTimeTable
	ofstream timetable(TIMEFILE, ofstream::out | ofstream::binary);
	last_update = time(NULL);
	put_int32(timetable, last_update);
	char buffer[2];
	for (int i(0); i < real_amount; i++)
	{
		buffer[0] = (uint16_t)Times[i] & (uint16_t)0b11111111;
		buffer[1] = ((uint16_t)Times[i] - ((uint16_t)Times[i] & (uint16_t)0b11111111)) >> 8;
		timetable.write(buffer, 2);
	}
	timetable.close();
	cout << "\nTime table updated successfully\n";
#pragma endregion

	ofstream clearing_input(READFILE, ofstream::out);
	clearing_input.close();
	cout << "\nThe change has worked out succesfully\n";

	system("PAUSE");
	return 0;
}