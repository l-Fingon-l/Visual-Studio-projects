#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

// Task 1
struct Student
{
	char* name;
	int group_number;
	int mark1;
	int mark2;
	int mark3;
	int mark4;
};

Student* student_initialization(int n)
{
	Student* group = new Student[n];
	for (int i(0); i < n; i++) // filling the group of students with the random values
	{
		int name_size = rand() % 20;
		group[i].name = new char[name_size];
		for (int j(0); j < name_size; j++) // creating the name
		{
			group[i].name[j] = rand() % 26 + 97;
		}
		group[i].group_number = rand() % 5; // let it be 5 groups at all. Why not.
		group[i].mark1 = rand() % 5 + 1;
		group[i].mark2 = rand() % 5 + 1;
		group[i].mark3 = rand() % 5 + 1;
		group[i].mark4 = rand() % 5 + 1;
	}
	return group;
}

void show_marks(Student* group, int number_of_student)
{
	cout << "mark 1: " << group[number_of_student - 1].mark1 << endl;
	cout << "mark 2: " << group[number_of_student - 1].mark2 << endl;
	cout << "mark 3: " << group[number_of_student - 1].mark3 << endl;
	cout << "mark 4: " << group[number_of_student - 1].mark4 << endl;
}

float average_mark(Student* group, int number_of_student) // the average mark of the student
{
	float mark = group[number_of_student - 1].mark1;
	mark += group[number_of_student - 1].mark2 + group[number_of_student - 1].mark3 +
		group[number_of_student - 1].mark4;
	mark /= 4;
	return mark;
}

// Task 2
void getword(ifstream& fin, string* word, int* end_of_file, int* position) // getting 1 word from the stream
{
	bool in_word = false;
	char c = 0;
	fin.seekg(*position, ios_base::beg);
	while (true)
	{
		int n = 1;
		fin.read(&c, n); // in case it is just a letter
		*position = fin.tellg();
		if (c && c != ' ' && c != '.' && c != ',' && c != ';' && c != ':' && '\n' && c != EOF)
		{
			in_word = true;
			*word += c;
		}
		else
		{
			if (*position == EOF) // the end of file
			{
				*end_of_file = EOF;
				break;
			}
			if (in_word) // the end of the word
			{
				in_word = false;
				break;
			}
		}
		*position = fin.tellg();
	}
}

string the_most_common_word(const char* FileName) // finding the most common word
{
	int amount = 0, end_of_file = 0, end_of_file1 = 0, the_biggest_amount = 0;
	string the_most_common = "", word = "";
	int* eof_ = &end_of_file;
	string* word_ = &word;
	int position = 0; // the position in the file
	int* pos_ = &position;
	while (end_of_file != EOF)
	{
		word = "";
		ifstream ReadFile;
		ReadFile.open(FileName);
		if (ReadFile.is_open()) // checking if the file was opened
		{
			cout << "File was opened!\n\n" << endl;
		}
		else
		{
			cout << "File was not opened!\n\n" << endl;
			system("PAUSE");
		}
		amount = 0; // amount of times this word meets in the text
		getword(ReadFile, word_, eof_, pos_);
		int position1 = 0;
		while (end_of_file1 != EOF) // for each word running the whole document to find the same words
		{
			string temp = "";
			string* temp_ = &temp;
			int* pos_ = &position1;
			int* eof_ = &end_of_file1;
			getword(ReadFile, temp_, eof_, pos_);
			if (word == temp) amount++;
		}
		end_of_file1 = 0;
		if (amount > the_biggest_amount)
		{
			the_biggest_amount = amount;
			the_most_common = word;
		}
		ReadFile.close();
	}
	return the_most_common;
}

string the_most_common_word(const char* FileName, string except_this)
{
	int amount = 0, end_of_file = 0, end_of_file1 = 0, the_biggest_amount = 0;
	string the_most_common = "", word = "";
	int* eof_ = &end_of_file;
	string* word_ = &word;
	int position = 0; // the position in the file
	int* pos_ = &position;
	while (end_of_file != EOF)
	{
		word = "";
		ifstream ReadFile;
		ReadFile.open(FileName);
		if (ReadFile.is_open()) // checking if the file was opened
		{
			cout << "File was opened!\n\n" << endl;
		}
		else
		{
			cout << "File was not opened!\n\n" << endl;
			system("PAUSE");
		}
		amount = 0; // amount of times this word meets in the text
		getword(ReadFile, word_, eof_, pos_);
		int position1 = 0;
		while (end_of_file1 != EOF) // for each word running the whole document to find the same words
		{
			string temp = "";
			string* temp_ = &temp;
			int* pos_ = &position1;
			int* eof_ = &end_of_file1;
			getword(ReadFile, temp_, eof_, pos_);
			if (word == temp) amount++;
		}
		end_of_file1 = 0;
		if (amount > the_biggest_amount && word != except_this)
		{
			the_biggest_amount = amount;
			the_most_common = word;
		}
		ReadFile.close();
	}
	return the_most_common;
}

void Task2(const char* Filename, const char* Filename2) // finally the whole task 2 method
{
	ofstream WriteFile;
	WriteFile.open(Filename2);
	WriteFile << "The most common word is: " << the_most_common_word(Filename) << endl;
	WriteFile << "The second most common word is: " << 
		the_most_common_word(Filename, the_most_common_word(Filename)) << endl;
	WriteFile.close();
}

int main(int argc, char** argv)
{
	// Task 9.E
	srand(time(0)); // initialization of RND-generator
    const int n = 40; // amount of students in group
	Student* group = student_initialization(n); // initalization of the group
	int number_of_student = 6;
	cout << group[number_of_student - 1].name << "'s average mark, who is " << number_of_student <<
	"'s in the group is: " << average_mark(group, number_of_student) << endl << endl;
	cout << "His marks are: " << endl;
	show_marks(group, number_of_student);
	// Task 17.A
	// Scanning the file f1.txt and printing out in the f2.txt
	// file 2 words that are the most common in f1.txt 
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "Task 2\n";
	Task2("f1.txt", "f2.txt");

	system("PAUSE");
	return 0;
}