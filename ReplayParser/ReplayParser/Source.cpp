#include "replay.h"
#include <cmath>

uint32_t patch = 10032;
uint16_t build_v = 6108;
bool change_map = true;
#define READFILE "settings.txt"
#define CONVERSION_FOLDER "Converted/"
#define CONVERSION_SUFFIX "(converted)"

string getname(stringstream& read)
{
	string result = "";
	char c = 0;
	int x = read.tellg();
	do
	{
		read.get(c);
		if (read.eof()) return "";
	} while ((c == ' ') || (c == '\n') || (c == '\0') || (c == 9));

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

bool read_settings(const char* filename, ifstream& read)
{
	const int linesize = 100;
    char getline[linesize];
	memset(getline, 0, linesize);
    string line;
	stringstream s;
    
    // extracting patch

    read.getline(getline, linesize);
    line = getline;
	s << line;
	string check = getname(s);
	if (!are_same(check, "patch"))
	{
		cout << "\nThe information in the " << filename << " is incorrect. Please, check it out.\n";
		return false;
	}
	int pos = s.tellg();
	s.seekg(pos + 2);
	s >> patch;

	// extracting build_v
	
	s.clear();
	memset(getline, 0, linesize);
	read.getline(getline, linesize);
	line = getline;
	s << line;
	check = getname(s);
	if (!are_same(check, "target_build"))
	{
		cout << "\nThe information in the " << filename << " is incorrect. Please, check it out.\n";
		return false;
	}
	pos = s.tellg();
	s.seekg(pos + 2);
	s >> build_v;
	
	// extracting "change map"

	s.clear();
	memset(getline, 0, linesize);
	read.getline(getline, linesize);
	line = getline;
	s << line;
	check = getname(s);
	if (!are_same(check, "change_map"))
	{
		cout << "\nThe information in the " << filename << " is incorrect. Please, check it out.\n";
		return false;
	}
	pos = s.tellg();
	s.seekg(pos + 2);
	s >> change_map;

	read.getline(getline, linesize);
	return true;
}

int main()
{
	// Init

	const int replay_name_size = 500;
	char get_replay_name[replay_name_size];
	string replay_name, replay_name_converted;
	ifstream read;
	int amount = 0, amount_success = 0;
	read.open(READFILE, ifstream::in);
	if (!read.is_open())
	{
		cout << READFILE << " was not opened!" << endl;
		return false;
	}

	// Read settings

	if (!read_settings(READFILE, read))
	{
		system("PAUSE");
		return 0;
	}

	// Read and repack replays one by one

	while (!read.eof())
	{
		memset(get_replay_name, 0, replay_name_size);
		read.getline(get_replay_name, replay_name_size);
		replay_name = get_replay_name;
		if (replay_name != "")
		{
			replay_name_converted = CONVERSION_FOLDER + replay_name + CONVERSION_SUFFIX + ".w3g";
			replay_name += ".w3g";
			amount++;

			if (!repack_replay(&replay_name[0], &replay_name_converted[0], patch, build_v, change_map))
			{
				cout << "Also check out if the folder " << CONVERSION_FOLDER << " exists.\n";
				cout << "Error in repacking of " << replay_name << ".. \nSkipped.\n\n";
			}
			else amount_success++;
		}
	}

	cout << "Conversion is done. " << amount_success << " replays of " << amount << " were successfully re-packed.\n"
		<< "Version " << (int)patch << " build " << (int)build_v << endl;

	system("PAUSE");
	return 0;
}
