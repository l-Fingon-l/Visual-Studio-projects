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
// This build: 2020.02.27 23:23 p.m. ////////////////////
/////////////////////////////////////////////////////////
// Patch-notes: 2020.02.21 15:27 p.m.: time data is no //
// longer in "time" but moved to the "data.time" file.///
// 2020.02.25 11:48 a.m.: floating K-factor added. //////
// Hence, the "data.txt" format is slightly changed. ////
// "data.time" stores the stats of a player since now. //
// 2020.02.27 23:23 p.m.: a possibility to read/write ///
// in table-mode (for google sheets, etc.) added. ///////
// Elo-model for calculation is slightly changed as /////
// well to serve the Warcraft 3 purposes better. ////////
///////////////////////////////////////////////////////// 

const uint32_t amount = 128, SecondsInDay = 86400;
const uint16_t InitialElo = 2000;
const uint16_t InactivityLimit = 30;
int real_amount = 0;
int K[3];
uint32_t last_update = 0;
bool report = 1, table_style = 1;
#define WRITEFILE "data.txt"
#define READFILE "result.txt"
#define TIMEFILE "data.time"

string getname(ifstream& read)
{
	string result = "";
	char c = 0;
	while ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9)) read.get(c);
	read.seekg((int)read.tellg() - 1);
	while (true)
	{
		read.get(c);
		if (read.eof()) break;
		if ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9)) break;
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

template<class T> void get_uint(ifstream& input, T& result) // uint8-32
{
	int size = sizeof(T);
	//checking if there is enough data
	int current_position = input.tellg();
	input.seekg(0, input.end);
	if ((int)(input.tellg()) - current_position < size)
		result = -1;
	input.seekg(current_position);

	//reading the data
	char buffer[64];
	uint64_t X = 0;
	for (int i(0); i < size; i++)
		input.get(buffer[i]);
	for (int i(size - 1); i > -1; i--)
	{
		X |= (uint8_t)buffer[i] & (uint64_t)0b11111111;
		X <<= 8;
	}
	X >>= 8;
	result = (T)X;
}

template<class T> void put_uint(ofstream& output, T input)
{
	int size = sizeof(T);
	char buffer[64];
	for (int i(0); i < size; i++)
	{
		buffer[i] = input & 0b11111111;
		input >>= 8;
	}
	output.write(buffer, size);
}

uint16_t InactivityDecline(double& Elo, uint16_t t) // returns ELO after the inactivity period t
{
	uint16_t result = 0;
	result = 21 * (Elo / (((double)t + 600)/30 + 1.0));
	return result;
}

uint16_t InactivityDeclineReverse(double& Elo, uint16_t t) // returns the ELO that was before the inactivity period t
{
	uint16_t result = 0;
	result = Elo * (((double)t + 600) / 30 + 1.0) / 21;
	return result;
}

uint16_t gettime()
{
	return (((double)time(NULL)) / ((double)SecondsInDay));
}

int Init(string file, string* Player, double* Elo, uint16_t* Times, uint16_t* Games)
{
	ifstream read;
	read.open(file, ifstream::in);
	if (!read.is_open())
	{
		cout << file << " was not opened!" << endl;
		return -1;
	}
	cout << file << " was successfully opened" << endl;
	read.seekg(0, ifstream::end);
	if (read.tellg() < 41)
	{
		cout << "\nThe information in the " << file << " is incorrect. Please, check it out.\n";
		return -1;
	}
	read.seekg(9);
	read >> report;
	int pos = read.tellg();
	read.seekg(pos + 15);
	read >> table_style;
	read.seekg(pos + 22);
	read >> K[0] >> K[1] >> K[2];
	int real_amount = 0;
	uint8_t amount_in_ratings = 0;
	while (!read.eof())
	{
		Player[real_amount] = getname(read);
		read >> Elo[real_amount];
		real_amount++;
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
		get_uint(times, amount_in_ratings);
		times.seekg(0, times.end); // in case the file is empty
		if ((amount_in_ratings == -1) || (times.tellg() < (5 + 6*(int)amount_in_ratings)))
		{
			times.close();
			ofstream create(TIMEFILE, ofstream::out);
			cout << TIMEFILE << " was corrupted, so it was re-created." << endl;
			create.close();
		}
		else
		{
			cout << TIMEFILE << " was successfully opened" << endl;
			times.seekg(ifstream::beg + 1);
			char buffer[2];
			get_uint(times, last_update);
			for (int i(0); i < amount_in_ratings; i++) // last updates
			{
				get_uint(times, Times[i]);
			}
			for (int i(0); i < amount_in_ratings; i++) // wins
			{
				get_uint(times, Games[i]);
			}
			for (int i(0); i < amount_in_ratings; i++) // losses
			{
				get_uint(times, Games[i + amount]);
			}
			times.close();
		}
	}


	for (int x(0); x < amount_in_ratings; x++)
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

	while (real_amount > amount_in_ratings) // initialising those added in WRITEFILE but absent in TIMEFILE
	{
		Times[amount_in_ratings] = gettime();
		Games[amount_in_ratings] = 0;
		Games[amount_in_ratings + amount] = 0;
		amount_in_ratings++;
	}
	return real_amount;
}

int NameToID(string& name, string* Player, double* Elo, uint16_t* Times, uint16_t* Games)
{
	for (int i(0); i < amount; i++) if (name == Player[i]) return i;
	cout << "Player \"" << name << "\" was not found in ratings" << endl;
	Player[real_amount] = name;
	Elo[real_amount] = InitialElo;
	Times[real_amount] = gettime();
	Games[real_amount] = 0;
	Games[real_amount + amount] = 0;
	real_amount++;
	return (real_amount - 1);
}

int Calculate_K(double& Elo, const int& Games) // returns K for an exact player
{
	if (Elo > 2400) return K[0];
	else if (Games > 30) return K[1];
	return K[2];
}

void EloChange(double& Elo1, double& Elo2, const int& Games1, const int& Games2, int& score1, int& score2)
{
	double E1 = (score1 + score2) / (1 + pow(10.0, (Elo2 - Elo1) / 400));
	double E2 = (score1 + score2) / (1 + pow(10.0, (Elo1 - Elo2) / 400));

	Elo1 = Elo1 + Calculate_K(Elo1, Games1) * (score1 - E1);
	Elo2 = Elo2 + Calculate_K(Elo2, Games2) * (score2 - E2);
}

int main()
{
	string Players[amount];
	double Elo[amount];
	uint16_t Times[amount];
	uint16_t Games[2 * amount]; // the first /amount/ elements are wins, then losses
	char c = 0;
	int score1 = 0, score2 = 0;
	real_amount = Init(WRITEFILE, Players, Elo, Times, Games);
	if (real_amount == -1)
	{
		system("PAUSE");
		return 0;
	}

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
		if ((int)(read.tellg()) - current_position < 7)
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
		int ID1 = NameToID(player1, Players, Elo, Times, Games);
		int ID2 = NameToID(player2, Players, Elo, Times, Games);
		double Elo1 = Elo[ID1], Elo2 = Elo[ID2];
#pragma endregion

#pragma region CalculatingTheLine
		EloChange(Elo[ID1], Elo[ID2], (Games[ID1] + Games[ID1 + amount]), (Games[ID2] + Games[ID2 + amount]), score1, score2);
		Times[ID1] = gettime();
		Times[ID2] = gettime();
		Games[ID1] += score1;
		Games[ID2] += score2;
		Games[ID1 + amount] += score2;
		Games[ID2 + amount] =+ score1;
		//reporting
		if (!report) return 0;
		cout << player1 << "\'s ELO-change is (" << setprecision(4) << (Elo[ID1] - Elo1) << "). Now his ELO is: " << Elo[ID1] << ".\n";
		cout << player2 << "\'s ELO-change is (" << (Elo[ID2] - Elo2) << "). Now his ELO is: " << Elo[ID2] << ".\n";
	}
	read.close();
#pragma endregion
	
#pragma region SortingTheRatings
	for (int i(0); i < real_amount; i++)
		for (int j(real_amount - 1); j > i; j--)
			if (Elo[j] > Elo[j - 1])
			{
				string bufferr = Players[j];
				uint16_t buffer[4];
				buffer[0] = (uint16_t)Elo[j];
				buffer[1] = Times[j];
				buffer[2] = Games[j];
				buffer[3] = Games[j + amount];

				Players[j] = Players[j - 1];
				Elo[j] = Elo[j - 1];
				Times[j] = Times[j - 1];
				Games[j] = Games[j - 1];
				Games[j + amount] = Games[j + amount - 1];

				Players[j - 1] = bufferr;
				Elo[j - 1] = buffer[0];
				Times[j - 1] = buffer[1];
				Games[j - 1] = buffer[2];
				Games[j + amount - 1] = buffer[3];
			}
#pragma endregion

#pragma region UpdatingTheDataFile
	write.open(WRITEFILE, ofstream::out);
	write << "report = " << report << endl << "table style = " << table_style << endl <<
		"K = " << K[0] << " " << K[1] << " " << K[2] << endl;
	for (int x(real_amount), y(0); x > 0; x--, y++)
	{
		write << endl << Players[y];
		if (table_style) write << (char)9;
		else write << " ";
		write << (int)Elo[y];
	}
	write.close();
#pragma endregion

#pragma region UpdatingTheTimeTable
	ofstream timetable(TIMEFILE, ofstream::out | ofstream::binary);
	last_update = time(NULL);
	put_uint(timetable, (uint8_t)real_amount);
	put_uint(timetable, (uint32_t)last_update);
	char buffer[2];
	for (int i(0); i < real_amount; i++)
		put_uint(timetable, Times[i]);
	for (int i(0); i < real_amount; i++)
		put_uint(timetable, Games[i]);
	for (int i(0); i < real_amount; i++)
		put_uint(timetable, Games[i + amount]);
	timetable.close();
	cout << "\nTime table updated successfully\n";
#pragma endregion

	ofstream clearing_input(READFILE, ofstream::out);
	clearing_input.close();
	cout << "The change has worked out succesfully\n\n";

	system("PAUSE");
	return 0;
}