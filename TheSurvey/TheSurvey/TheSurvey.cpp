#include "EloDatabase.h"
#include <ctime>
//TODO: additionally compress data save from 8 bit (256 values) to 4 bit (16)? Maybe, 5 bit (32) is the best option?
//TODO: think about the order of precompiling commands and direct instructions are executed after the build of .exe
//TODO: StepBack + Exit leads to a continue. BUG in keeping the returned values of void StepBack (should be bool)
std::string player_list = {
#include "players.txt"
};

using namespace std;

int main()
{
	// just some dirty greetings to all those stjupid old lads
	//messages;

	srand(time(0));

	cout << "\n\n\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "\t\t Welcome to the Survey on the Strongest player in recent years of Warcraft 3 history!\t\n\n";
	cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "\t What are we interested in is your own pure opinion. So do not be afraid of making the unpopular decisions!\n\n";
	cout << "  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << " ~~~~~~~~~~~   Some rules about your controls   ~~~~~~~~~~~\n";
	cout << "~~~~~~ Press arrows (either simple or ones from the NumPad)~~~~\n";
	cout << "~~~ Left arrow chooses the left player ~~~~~\n";
	cout << "         ~~~~ Right arrow chooses the player on the right ~~~~~\n";
	cout << "~~ An arrow down skips the question (in case u dont kinda like it) ~~\n";
	cout << "~~ An arrow up allows you to make 1 step back (made a mistake? Here is it) ~~\n";
	cout << " ~~~~~~~~~~~ Press Esc in case u wanna leave the Survey at this point ~~~~~~~~~~\n";
	cout << " ~~ Still the progress is gonna be saved even in case of unpredictable shutdown ~~~~\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ Let's start~! ~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

	// now we'll run the Survey code itself

	EloDatabasePoll* TheSurvey = new EloDatabasePoll(player_list); // the Database created
	while (TheSurvey->Request());

	delete TheSurvey;
	system("PAUSE");
	/*
	BYTE a = 0b1111;
	int x = 12, y = 8;
	BYTE* b = &a;
	a &= x;
	a <<= 4;
	a += y;
	system("PAUSE");*/
}
