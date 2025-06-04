#include <iostream>  
#include <fstream>
#include <string>

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

// Function to read high scores from a text file and print them
void readHighScores(int level);
void writeHighScores(const std::string& levelName, int newScore);

// Variables
int oldTime;
int newTime;

int main()  
{  
 
   std::cout << "Your previous times are: " << std::endl;
   readHighScores(0); // Read high scores for level 1 (index 0)


   std::cout << "Your Level 1 score: " << std::endl;
   std::cin >> newTime;
   writeHighScores("Level 1", newTime); // Write new score for Level 1

   std::cout << "Your updated times are: " << std::endl;
   readHighScores(0); // Read high scores again for level 1);

   
   return 0;  
}

void readHighScores(int level) // level to set oldTime to  
{  
   std::ifstream highscores("LevelScoreData.bin", std::ios::binary); // binary mode  
   if (!highscores)  
   {  
       std::cerr << "ERROR could not open LevelScoreData.bin for reading" << std::endl;  
       return;
   }  

   size_t numEntries = 0; // Initialize the variable to fix lnt-uninitialized-local error  

   highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // read the number of scores as binary data  

   TimeScore* scores = new TimeScore[numEntries]; // dynamically allocate an array of HighScore structs  

   highscores.read(reinterpret_cast<char*>(scores), numEntries * sizeof(TimeScore)); // read the entire array as binary data  

   if (highscores.bad())  
   {  
       std::cerr << "ERROR while reading LevelScoreData.bin" << std::endl;  
       delete[] scores; // Ensure memory cleanup before returning  
       return;
   }  
   if (level <= numEntries)
   {
       oldTime = scores[level].time;
   }

   for (size_t i = 0; i < numEntries; ++i)  
   {  
       std::cout << scores[i].level << " Time: " << scores[i].time << std::endl;  
   }  

   delete[] scores;  
   highscores.close();  
}

void writeHighScores(const std::string& levelName, int newScore)
{
    std::fstream highscores("LevelScoreData.bin", std::ios::in | std::ios::out | std::ios::binary);
    if (!highscores)
    {
        std::cerr << "ERROR could not open LevelScoreData.bin for updating" << std::endl;
        return;
    }

    size_t numEntries = 0;
    highscores.read(reinterpret_cast<char*>(&numEntries), sizeof(numEntries)); // Here the numEntries is actually set

    for (size_t i = 0; i < numEntries; ++i)
    {
        std::streampos pos = highscores.tellg(); // tellg returns the position of the input stream
        TimeScore score;
        highscores.read(reinterpret_cast<char*>(&score), sizeof(TimeScore));

        // Compare level names (ensure null-termination)
        if (std::string(score.level) == levelName)
        {
            // Only update if the new score is higher than the old one
            if (newScore > static_cast<int>(score.time))
            {
                score.time = static_cast<float>(newScore);
                score.unlocked = true;

                highscores.seekp(pos);
                highscores.write(reinterpret_cast<char*>(&score), sizeof(TimeScore));
            }
            break;
        }
    }
    highscores.close();
}
