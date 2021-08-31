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
// This build: 2020.03.24 10:47 a.m. ////////////////////
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
// 2020.03.16 18:03 p.m.: a possibility to restart the //
// ladder using the saved in "chronicle.txt" file ///////
// data, that might be easily accessed and modified. ////
// Names no longer switch to the lower registry. Huge ///
////////////// inner architecture rework. ///////////////
///////////////////////////////////////////////////////// 

const uint32_t amount = 128, SecondsInDay = 86400;
const uint16_t InitialElo = 2000;
const uint16_t InactivityLimit = 65;
time_t t = 0;
struct tm tmp;
char MY_TIME[11];
int real_amount = 0;
int K[3];
char c = 0;
int score1 = 0, score2 = 0;
uint16_t last_update = 0, this_update = 0;
bool report = 1, table_style = 0, recalculate = 0;
const char* coef[4] = { "re-calculate", "report", "table-style", "K" };
#define DATAFILE "data.txt"
#define READFILE "result.txt"
#define TIMEFILE "data.time"
#define CHRONOFILE "chronicle.txt"

string getname(ifstream& read)
{
	string result = "";
	char c = 0;
	int x = read.tellg();
	do
	{
		read.get(c);
		if (read.eof()) return "";
	} while ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9));

//	while ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9)) read.get(c);
	read.seekg((int)read.tellg() - 1);
	while (true)
	{
		read.get(c);
		if (read.eof()) break;
		if ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9)) break;
		result += c;
	}
	if (!read.eof())
	{
		if (c == '\n') read.seekg((int)read.tellg() - 1);
		read.seekg((int)read.tellg() - 1);
		return result;
	}
	else return result;
}

uint16_t get_date(ifstream& read)
{
	int yy, mm, dd;
	char c;
	read >> yy >> c >> mm >> c >> dd;
	tmp.tm_year = yy - 1900;
	tmp.tm_mon = mm - 1;
	tmp.tm_mday = dd;
	return (uint16_t)((double)mktime(&tmp) / SecondsInDay);
}

bool are_same(const string& name1, const string& name2) // compare the names including the registry
{
	if (name1.size() != name2.size()) return false;
	int size = name1.size();
	for (int i(0); i < size; i++)
	{
		if (((name1[i] >= 65) && (name1[i] <= 90)) || ((name2[i] >= 65) && (name2[i] <= 90)))
		{
			if ((abs((int)(name1[i] - name2[i])) != 32) && (name1[i] != name2[i])) return false;
		}
		else if (name1[i] != name2[i]) return false;
	}
	return true;
}

template<class T> bool get_uint(ifstream& input, T& result) // uint8-32
{
	int size = sizeof(T);
	//checking if there is enough data
	int current_position = input.tellg();
	input.seekg(0, input.end);
	if ((int)(input.tellg()) - current_position < size)
	{
		result = -1;
		return 0;
	}
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
	return 1;
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
	if (recalculate) return this_update;
	return (((double)time(NULL)) / ((double)SecondsInDay));
}

int Time_Init(string* Player, double* Elo, uint16_t* Times, uint16_t* Games)
{
	uint8_t amount_in_ratings = 0;
#pragma region ReadTimeFile
	ifstream times;
	times.open(TIMEFILE, ifstream::in | ifstream::binary);
	if (!times.is_open() || recalculate)
	{
		ofstream create(TIMEFILE, ofstream::out);
		cout << TIMEFILE << " was created" << endl;
		create.close();
	}
	else
	{
		times.seekg(0, ifstream::end); // in case the file is empty
		if (times.tellg() < 1)
		{
			times.close();
			ofstream create(TIMEFILE, ofstream::out);
			cout << TIMEFILE << " was corrupted, so it was re-created." << endl;
			create.close();
		}
		else
		{
			times.seekg(0, times.beg);
			get_uint(times, amount_in_ratings);
			times.seekg(0, times.end);
			if ((amount_in_ratings == -1) || (times.tellg() < (5 + 6 * (int)amount_in_ratings)))
			{
				times.close();
				ofstream create(TIMEFILE, ofstream::out);
				cout << TIMEFILE << " was corrupted, so it was re-created." << endl;
				create.close();
			}
			else
			{
				times.seekg(1, times.beg);
				cout << TIMEFILE << " was successfully opened" << endl;
				char buffer[2];
				uint32_t x = 0;
				get_uint(times, x);
				last_update = (double)x / SecondsInDay;
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
	}
#pragma endregion

	for (int x(0); x < amount_in_ratings; x++)
	{
		if ((gettime() - Times[x]) > InactivityLimit)
		{
			double EloBefore = Elo[x];
			if (last_update - Times[x] > InactivityLimit) // in case the inactivity decline was updated recently
				Elo[x] = InactivityDeclineReverse(Elo[x], last_update - Times[x] - InactivityLimit);
			Elo[x] = InactivityDecline(Elo[x], gettime() - Times[x] - InactivityLimit);
			cout << endl << Player[x] << " is already inactive for " << gettime() - Times[x] << " days. His ELO was decreased by (" <<
				EloBefore - Elo[x] << ").\nNow his ELO is: " << Elo[x] << ".\n\n";
		}
	}

	while (real_amount > amount_in_ratings) // initialising those added in DATAFILE but absent in TIMEFILE
	{
		Times[amount_in_ratings] = gettime();
		Games[amount_in_ratings] = 0;
		Games[amount_in_ratings++ + amount] = 0;
	}
	return real_amount;
}

int Init(string* Player, double* Elo, uint16_t* Times, uint16_t* Games)
{
	time(&t);
	localtime_s(&tmp, &t);

#pragma region DataFile
	ifstream read;
	read.open(DATAFILE, ifstream::in);
	if (!read.is_open())
	{
		cout << DATAFILE << " was not opened!" << endl;
		return -1;
	}
	cout << DATAFILE << " was successfully opened" << endl;
	read.seekg(0, ifstream::end);
	if (read.tellg() < 62)
	{
		cout << "\nThe information in the " << DATAFILE << " is incorrect. Please, check it out.\n";
		return -1;
	}
	read.seekg(0, ifstream::beg);
	string check = getname(read);
	if (!are_same(check, coef[0]))
	{
		cout << "\nThe information in the " << DATAFILE << " is incorrect. Please, check it out.\n";
		return -1;
	}
	int pos = read.tellg();
	read.seekg(pos + 2);
	read >> recalculate;

	check = getname(read);
	if (!are_same(check, coef[1]))
	{
		cout << "\nThe information in the " << DATAFILE << " is incorrect. Please, check it out.\n";
		return -1;
	}
	pos = read.tellg();
	read.seekg(pos + 2);
	read >> report;
	
	check = getname(read);
	if (!are_same(check, coef[2]))
	{
		cout << "\nThe information in the " << DATAFILE << " is incorrect. Please, check it out.\n";
		return -1;
	}
	pos = read.tellg();
	read.seekg(pos + 2);
	read >> table_style;

	check = getname(read);
	if (!are_same(check, coef[3]))
	{
		cout << "\nThe information in the " << DATAFILE << " is incorrect. Please, check it out.\n";
		return -1;
	}
	pos = read.tellg();
	read.seekg(pos + 2);
	read >> K[0] >> K[1] >> K[2];

	while (!read.eof())
	{
		Player[real_amount] = getname(read);
		if (Player[real_amount] == "") break;
		read >> Elo[real_amount];
		real_amount++;
	}
	read.close();
#pragma endregion

#pragma region chronology
	ifstream chronology;
	chronology.open(CHRONOFILE, ifstream::in);
	if (!chronology.is_open())
	{
		if (recalculate)
		{
			cout << "It is impossible to re-calculate your rankings without the chronicle data saved!" << endl;
			return -1;
		}
		ofstream create(CHRONOFILE, ofstream::out);
		strftime(MY_TIME, sizeof(MY_TIME), "%F", &tmp);
		create << "Creation date: " << MY_TIME;
		cout << CHRONOFILE << " was created" << endl;
		create.close();
	}
	else if (recalculate)
	{
		chronology.seekg(14, ifstream::beg);
		last_update = get_date(chronology);
		this_update = last_update;
	}
	chronology.close();
#pragma endregion

	return Time_Init(Player, Elo, Times, Games);	//time initialization
}

int NameToID(string& name, string* Player, double* Elo, uint16_t* Times, uint16_t* Games)
{
	for (int i(0); i < amount; i++) if (are_same(name, Player[i])) return i;
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

int CalculateTheBlock(ifstream& read, string* Players, double* Elo, uint16_t* Times, uint16_t* Games)
{
	int result = -1;
	string file;
	if (recalculate) file = CHRONOFILE;
	else file = READFILE;
	string player1, player2;
#pragma region ReadingTheLine
	while (!read.eof())
	{
		int pos = read.tellg();
		//checking if there is enough data
		read.seekg(0, read.end);
		if ((int)(read.tellg()) - pos < 7)
		{
			cout << "\nThe information in the " << file << " is incorrect. Please, check it out.\n";
			system("PAUSE");
			return -1;
		}
		else read.seekg(pos);

		player1 = getname(read);
		if (player1 == "=========================================")
		{
			read.seekg(pos);
			result = 1;
			break;
		}
		read >> score1;
		read >> c;
		read >> score2;
		pos = read.tellg();
		read.seekg(pos + 1);
		player2 = getname(read);
		cout << "\nThe input results: " << player1 << " " << score1 << ":" << score2 << " " << player2 << endl;
		int ID1 = NameToID(player1, Players, Elo, Times, Games);
		int ID2 = NameToID(player2, Players, Elo, Times, Games);
		player1 = Players[ID1];
		player2 = Players[ID2];
		double Elo1 = Elo[ID1], Elo2 = Elo[ID2];
#pragma endregion

#pragma region CalculatingTheLine
		EloChange(Elo[ID1], Elo[ID2], (Games[ID1] + Games[ID1 + amount]), (Games[ID2] + Games[ID2 + amount]), score1, score2);
		Times[ID1] = gettime();
		Times[ID2] = gettime();
		Games[ID1] += score1;
		Games[ID2] += score2;
		Games[ID1 + amount] += score2;
		Games[ID2 + amount] += score1;
		//reporting
		cout << player1 << "\'s ELO-change is (" << setprecision(4) << (Elo[ID1] - Elo1) << "). Now his ELO is: " << Elo[ID1] << ".\n";
		cout << player2 << "\'s ELO-change is (" << (Elo[ID2] - Elo2) << "). Now his ELO is: " << Elo[ID2] << ".\n";
		result = 0;
	}
	return result;
#pragma endregion
}

int main()
{
	string Players[amount];
	double Elo[amount];
	uint16_t Times[amount];
	uint16_t Games[2 * amount]; // the first /amount/ elements are wins, then losses
	real_amount = Init(Players, Elo, Times, Games);
	if (real_amount == -1)
	{
		system("PAUSE");
		return 0;
	}

#pragma region InCaseOfReCalculation
	if (recalculate)
	{
		ifstream read_chronicle(CHRONOFILE, ifstream::in);
		if (!read_chronicle.is_open())
		{
			cout << CHRONOFILE << " was not opened!" << endl;
			system("PAUSE");
			return 0;
		}
		cout << CHRONOFILE << " was successfully opened" << endl;
		read_chronicle.seekg(27, ifstream::beg);
		int pos = read_chronicle.tellg();
		int not_the_end = 1;
		string check = "";
		while (not_the_end)
		{
			check = getname(read_chronicle);
			if (check != "=========================================")
			{
				cout << "\nThe information in the " << CHRONOFILE << " is incorrect. Please, check it out.\n";
				system("PAUSE");
				return 0;
			}
			this_update = get_date(read_chronicle);

			for (int x(0); x < real_amount; x++)
			{
				if ((gettime() - Times[x]) > InactivityLimit)
				{
					double EloBefore = Elo[x];
					if (last_update - Times[x] > InactivityLimit) // in case the inactivity decline was updated recently
						Elo[x] = InactivityDeclineReverse(Elo[x], last_update - Times[x] - InactivityLimit);
					Elo[x] = InactivityDecline(Elo[x], gettime() - Times[x] - InactivityLimit);
					cout << endl << Players[x] << " is already inactive for " << gettime() - Times[x] << " days. His ELO was decreased by (" <<
						EloBefore - Elo[x] << ").\nNow his ELO is: " << Elo[x] << ".\n\n";
				}
			}

			switch (CalculateTheBlock(read_chronicle, Players, Elo, Times, Games))
			{
			case 0:
				not_the_end = 0;
				break;
			case -1:
				cout << "\nThe information in the " << CHRONOFILE << " is incorrect. Please, check it out.\n";
				system("PAUSE");
				return 0;
			}
		}
		read_chronicle.close();
	}
#pragma endregion

#pragma region ReadFile
	else
	{
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

		if (!(CalculateTheBlock(read, Players, Elo, Times, Games) + 1))
		{
			cout << "\nThe information in the " << READFILE << " is incorrect. Please, check it out.\n";
			system("PAUSE");
			return 0;
		}
		read.close();
	}
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

	ofstream write;
#pragma region UpdatingTheDataFile
	write.open(DATAFILE, ofstream::out);
	recalculate = 0; // safety reasons
	write << coef[0] << " = " << recalculate << endl << coef[1] << " = " << report << endl << coef[2] <<
		" = " << table_style << endl << "K = " << K[0] << " " << K[1] << " " << K[2] << endl;
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
	put_uint(timetable, (uint8_t)real_amount);
	put_uint(timetable, (uint32_t)time(NULL));
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

#pragma region UpdatingChronology
	if (!recalculate)
	{
		ifstream copy(READFILE, ifstream::binary); // copying the readfile content
		copy.seekg(0, copy.end);
		int SIZE = copy.tellg();
		char* BUF = new char[SIZE];
		copy.seekg(0, copy.beg);
		copy.read(BUF, SIZE);
		copy.close();

/*		ifstream copy_chronology(CHRONOFILE, ifstream::binary); // copying the previous chronofile content
		copy_chronology.seekg(0, copy.end);
		int SIZE_ = copy_chronology.tellg();
		string CHRONO_BUF_STR = "";
		char temp[256];
		copy_chronology.seekg(0, copy.beg);
		while (!copy_chronology.eof())
		{
			copy_chronology.getline(temp, 256);
			CHRONO_BUF_STR += temp;
		}
		/*		char* CHRONO_BUF = new char[SIZE_];
				copy_chronology.seekg(0, copy.beg);
				copy_chronology.read(CHRONO_BUF, SIZE);
				copy_chronology.close(); */
//		char* CHRONO_BUF = &CHRONO_BUF_STR[0]; // Be careful! C++11! 

		ofstream update_chronology(CHRONOFILE, ofstream::binary | ofstream::app);
//		update_chronology.write(CHRONO_BUF, SIZE_);
		update_chronology.put(13);
		update_chronology.put(10);
		update_chronology << "=========================================";
		update_chronology.put(13);
		update_chronology.put(10);
		strftime(MY_TIME, sizeof(MY_TIME), "%F", &tmp);
		update_chronology << MY_TIME;
		update_chronology.put(13);
		update_chronology.put(10);
		update_chronology.write(BUF, SIZE);
		update_chronology.close();

		ofstream clearing_input(READFILE);
		clearing_input.close();
		delete[] BUF;
		cout << "The chronicle updated successfully\n";
	}
#pragma endregion

	cout << "The change has worked out succesfully\n\n";

	if (report) system("PAUSE");
	return 0;
}