#include <fstream>
#include <iostream>

struct HighScore
{
	char name[4];
	uint32_t score; // uint32_t is a 32-bit unsigned integer which is guaranteed 4 bytes
};

struct TimeScore
{
	char level[8];
	float time;
	bool unlocked;
};

int main()
{
	std::cout << "Text file: " << std::endl;

	// Write ASCII files. ASCII is a type of text file
	{
		std::ofstream highscores("highscores.txt"); // default mode is ios::out
		// if highscores.txt already exists, it will be overwritten
		if (!highscores)
		{
			std::cerr << "ERROR could not open highscores.txt for writing" << std::endl;
			return 1; // error
		}

		highscores << "TOM " << 100 << std::endl;
		highscores << "JIM " << 200 << std::endl;
		highscores << "LUC " << 300 << std::endl;

		if (highscores.bad())
		{
			std::cerr << "ERROR while writing to highscores.txt" << std::endl;
			return 1; // error
		}

		highscores.close();
	} // highscores would already be closed here, but we do it explicitly
		
	// Add date to a file
	{
		std::ofstream highscores("highscores.txt", std::ios::app); // append mode
		if (!highscores)
		{
			std::cerr << "ERROR could not open highscores.txt for appending" << std::endl;
			return 1; // error
		}
		highscores << "JER " << 999 << std::endl;

		if (highscores.bad())
		{
			std::cerr << "ERROR while appending to highscores.txt" << std::endl;
			return 1; // error
		}

		highscores.close();
	}
	// Read data from file
	{
		std::ifstream highscores("highscores.txt"); // default mode is ios::in
		if (!highscores)
		{
			std::cerr << "ERROR could not open highscores.txt for reading" << std::endl;
			return 1; // error
		}

		char name[4];
		int score;
		while (highscores >> name >> score) // will loop until the end of the line
		{
			std::cout << name << " " << score << std::endl;
		}

		if (highscores.bad())
		{
			std::cerr << "ERROR while reading from highscores.txt" << std::endl;
			return 1; // error
		}
		highscores.close();
	}

	std::cout << std::endl << "Binary file: " << std::endl;;

	// Writing binary files
	{
		std::ofstream highscores("highscores.bin", std::ios::binary); // binary mode
		if (!highscores)
		{
			std::cerr << "ERROR could not open highscores.bin for writing" << std::endl;
			return 1; // error
		}

		HighScore scores[] = {
			{"TOM", 100},
			{"JIM", 200},
			{"LUC", 300},
			{"JER", 999}
		};
		auto numScores = std::size(scores); // get the number of elements in the array

		highscores.write(reinterpret_cast<char*>(&numScores), sizeof(numScores)); // write the number of scores as binary data
		highscores.write(reinterpret_cast<char*>(scores), numScores * sizeof(highscores)); // write the entire array as binary data,

		if (highscores.bad())
		{
			std::cerr << "ERROR while writing to highscores.bin" << std::endl;
			return 1; // error
		}
		highscores.close();
	}

	// Read binary file
	{
		std::ifstream highscores("highscores.bin", std::ios::binary); // binary mode
		if (!highscores)
		{
			std::cerr << "ERROR could not open highscores.bin for reading" << std::endl;
			return 1; // error
		}

		size_t numEntries;

		highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // read the number of scores as binary data

		HighScore* scores = new HighScore[numEntries]; // dynamically allocate an array of HighScore structs

		highscores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(HighScore)); // read the entire array as binary data

		if (highscores.bad())
		{
			std::cerr << "ERROR while reading highscores.bin" << std::endl;
			return 1; // error
		}

		for (size_t i = 0; i < numEntries; ++i)
		{
			std::cout << scores[i].name << " " << scores[i].score << std::endl;
		}
			

		delete[] scores;

	}

	// Create a binary file for my game score system
	{
		std::ofstream highscores("LevelScoreData.bin", std::ios::binary); // binary mode
		if (!highscores)
		{
			std::cerr << "ERROR could not open LevelScoreData.bin for writing" << std::endl;
			return 1; // error
		}

		TimeScore scores[] = {
			{"Level 1", 0.f, true},
			{"Level 2", 0.f, false},
			{"Level 3", 0.f, false},
			{"Level 4", 0.f, false},
			{"Level 5", 0.f, false}
		};
		auto numScores = std::size(scores); // get the number of elements in the array

		highscores.write(reinterpret_cast<char*>(&numScores), sizeof(numScores)); // write the number of scores as binary data
		highscores.write(reinterpret_cast<char*>(scores), numScores * sizeof(highscores)); // write the entire array as binary data,

		if (highscores.bad())
		{
			std::cerr << "ERROR while writing to LevelScoreData.bin" << std::endl;
			return 1; // error
		}
		highscores.close();
	}
	return 0;


}