#include <iostream>
#include <string>
#include <fstream>


bool readFile(char path[]) {
	std::cout << "READING FROM: " << path << "\n\n";

	std::ifstream fin;

	fin.open(path);

	if (!fin) {
		std::cout << "ERROR -> CANNOT READING FROM: " << path << "\n";
		fin.close();
		return false;
	}
	else {
		std::string line;
		while (std::getline(fin, line)) {
			std::cout << line << "\n";
		}
	}

	fin.close();

	return true;
}

bool writeToFile(char path[], const char text[]= "oh no this is a laba poooo... kripte") {
	std::cout << "WRITTING TO: " << path << "\n";

	std::ofstream fout;

	fout.open(path);

	if (!fout) {
		std::cout << "ERROR -> CANNOT WRITTEN TO: " << path << "\n";
		fout.close();
		return false;
	}
	else {
		fout << text;
		fout.close();
	}


	return true;
}

int main(int argc, char* argv[]) 
{
	if (argc == 3) 
	{
		if (!strcmp(argv[1], "-r")) 
		{ 
			readFile(argv[2]);
		}
		else if (!strcmp(argv[1], "-w")) 
		{
			writeToFile(argv[2]);
		}
		else
		{
			std::cout << "ERROR\n";
		}
	}

	else if (argc == 4)
	{
		if (!strcmp(argv[1], "-w"))
		{
			writeToFile(argv[2], argv[3]);
		}

		else
		{
			std::cout << "ERROR\n";
		}
	}

	else 
	{
		std::cout << "ERROR\n";
	}


	return 0;
}