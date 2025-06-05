#include <iostream>  
#include <fstream>
#include <string>

struct LevelStruct  
{  
   int world;
   int level;
   float time;  
   bool unlocked;  
   int stars;
   int TimeFor3Stars;
};  

// Function to read high scores from a text file and print them
void readHighScores(int world);
void writeHighScores(int world, int level, float newScore);
void createHighScoresFile(); // Function to create the high scores file if it doesn't exist

// Variables
int oldTime;
int newTime;

int main()
{
    std::cout << "Welcome to the High Score Tracker!" << std::endl;
    createHighScoresFile();
    std::cout << "Reading high scores for World 1..." << std::endl;
    readHighScores(1);

    int world = 1, level = 1;
    std::cout << "Enter your new time for World " << world << ", Level " << level << ": ";
    std::cin >> newTime;
    writeHighScores(world, level, newTime);

    std::cout << "Your updated times for World 1:" << std::endl;
    readHighScores(1);

    return 0;
}



void writeHighScores(int world, int level, float newScore)
{
    std::fstream highscores("LevelScoreData.bin", std::ios::in | std::ios::out | std::ios::binary);
    if (!highscores)
    {
        std::cerr << "ERROR could not open LevelScoreData.bin for updating" << std::endl;
        return;
    }
    size_t numEntries = 0;
    highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));
    for (size_t i = 0; i < numEntries; ++i)
    {
        std::streampos pos = highscores.tellg();
        LevelStruct score;
        highscores.read(reinterpret_cast<char*>(&score), sizeof(LevelStruct));
        if (score.world == world && score.level == level)
        {
            if (newScore < score.time || score.time == 0.f)
            {
                score.time = newScore;
                score.unlocked = true;

				// Set stars based on the new score
				if (newScore < score.TimeFor3Stars) score.stars = 3;
				else if (newScore < score.TimeFor3Stars + 5) score.stars = 2;
				else if (newScore < score.TimeFor3Stars + 11) score.stars = 1;
				else score.stars = 0;


                highscores.seekp(pos);
                highscores.write(reinterpret_cast<char*>(&score), sizeof(LevelStruct));



                if (score.stars == 3)
                {
                    std::cout << "Congrats you earned 3 stars" << std::endl;
                }

            }
            break;
        }
    }
    highscores.close();
}

void readHighScores(int world)
{
    std::ifstream highscores("LevelScoreData.bin", std::ios::binary);
    if (!highscores)
    {
        std::cerr << "ERROR could not open LevelScoreData.bin for reading" << std::endl;
        return;
    }
    size_t numEntries = 0;
    highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries));
    LevelStruct* scores = new LevelStruct[numEntries];
    highscores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(LevelStruct));
    if (highscores.bad())
    {
        std::cerr << "ERROR while reading LevelScoreData.bin" << std::endl;
        delete[] scores;
        return;
    }
    for (size_t i = 0; i < numEntries; ++i)
    {
        if (scores[i].world == world)
            std::cout << "Level " << scores[i].level << " Time: " << scores[i].time << std::endl;
    }
    delete[] scores;
    highscores.close();
}

void createHighScoresFile()
{
    // Check if the file already exists
    std::ifstream infile("LevelScoreData.bin", std::ios::binary);
    if (infile.good()) {
        // File exists, do not overwrite
        infile.close();
        return;
    }
    infile.close();

    // File does not exist, create and initialize it
    std::ofstream highscores("LevelScoreData.bin", std::ios::binary);
    if (!highscores)
    {
        std::cerr << "ERROR could not create LevelScoreData.bin" << std::endl;
        return;
    }
    LevelStruct scores[] = {
        // World 1
        {1, 1, 0.f, false, 0, 36},
        {1, 2, 0.f, false, 0, 40},
        {1, 3, 0.f, false, 0, 45},
        {1, 4, 0.f, false, 0, 50},
        {1, 5, 0.f, false, 0, 55},
        // World 2
        {2, 1, 0.f, false, 0, 38},
        {2, 2, 0.f, false, 0, 42},
        {2, 3, 0.f, false, 0, 47},
        {2, 4, 0.f, false, 0, 52},
        {2, 5, 0.f, false, 0, 57},
        // World 3
        {3, 1, 0.f, false, 0, 39},
        {3, 2, 0.f, false, 0, 44},
        {3, 3, 0.f, false, 0, 49},
        {3, 4, 0.f, false, 0, 54},
        {3, 5, 0.f, false, 0, 59}
    };
    auto numScores = std::size(scores);
    highscores.write(reinterpret_cast<char*>(&numScores), sizeof(numScores));
    highscores.write(reinterpret_cast<char*>(scores), numScores * sizeof(LevelStruct));
    if (highscores.bad())
    {
        std::cerr << "ERROR while writing to LevelScoreData.bin" << std::endl;
        return;
    }
    highscores.close();
}

