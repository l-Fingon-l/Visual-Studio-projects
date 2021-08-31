#include <cstdio>
#include <fstream>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

// Elo-ratings for Warcraft 3 by Fingon.
// Initial creation date: 2020.02.09
// This build: 2020.02.10 01:59 a.m.

const int amount = 128;
const int InitialElo = 2000;
int K, real_amount = 0;
bool report = 1;
#define WRITEFILE "data.txt"
#define READFILE "result.txt"

string getname(ifstream& read)
{
	string result = "";
	char c = 0;
	while ((c == ' ') || (c == '\n')) read.get(c);
	result += c;
	while (true)
	{
		read.get(c);
		if ((c == ' ') || (c == '\n')) break;
		if ((c >= 65) && (c <= 90)) result += c + 32;
		else result += c;
	}
	read.seekg((int)read.tellg() - 1);
	return result;
}

int Init(string file, string*& Player, double*& Elo)
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
		int i = 0;
		while (!read.eof())
		{
			read >> Player[i];
			read >> Elo[i];
			i++;
		}
		read.close();
		return i;
	}
	else cout << file << " was not opened!" << endl;
	return -1;
}

int NameToID(string& name, string*& Player, double*& Elo)
{
	for (int i(0); i < amount; i++) if (name == Player[i]) return i;
	cout << "Player \"" << name << "\" was not found in ratings" << endl;
	Player[real_amount] = name;
	Elo[real_amount] = InitialElo;
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
	string* players = Players;
	double* elo = Elo;
	char c = 0;
	int score1 = 0, score2 = 0;
	real_amount = Init(WRITEFILE, players, elo);

	ifstream read(READFILE, ifstream::in);
	if (!read.is_open())
	{
		cout << READFILE << " was not opened!" << endl;
		return -1;
	}
	else cout << READFILE << " was successfully opened" << endl;
	string player1, player2;

#pragma region CalculatingTheLine
	while (!read.eof())
	{
		read >> player1;
		read >> score1;
		read >> c;
		read >> score2;
		int pos = read.tellg();
		read.seekg(pos + 1);
		read >> player2;
		cout << "\nThe input results: " << player1 << " " << score1 << ":" << score2 << " " << player2 << endl;
		int ID1 = NameToID(player1, players, elo);
		int ID2 = NameToID(player2, players, elo);
		double Elo1 = Elo[ID1], Elo2 = Elo[ID2];

		for (int rounds(score1 + score2); rounds > 0; rounds--)
		{
			while (score1)
			{
				EloChange(Elo[ID1], Elo[ID2], false);
				score1--;
				rounds--;
			}
			while (score2)
			{
				EloChange(Elo[ID1], Elo[ID2], true);
				score2--;
				rounds--;
			}
		}

		//writing
		ofstream write(WRITEFILE, ofstream::out);
		write << "report = " << report << endl << "K = " << K << endl;
		for (int x(real_amount), y(0); x > 0; x--, y++) write << endl << Players[y] << " " << (int)Elo[y];
		write.close();

		//reporting
		if (!report) return 0;
		cout << player1 << "\'s Elo-change is (" << setprecision(4) << (Elo[ID1] - Elo1) << "). Now his Elo is: " << Elo[ID1] << ".\n";
		cout << player2 << "\'s Elo-change is (" << (Elo[ID2] - Elo2) << "). Now his Elo is: " << Elo[ID2] << ".\n";
	}
	read.close();
#pragma endregion

	cout << "\nThe change has worked out succesfully\n";

	cin.get();
	return 0;
}