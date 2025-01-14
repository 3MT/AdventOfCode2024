#include "Puzzle14.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <set>

//we are given a list of robots in the format: p=0,4 v=3,-3
//p is initial position, v is initial velocity
//robots move by v each step, and wrap around like asteroids.
//after updating them for 100 steps we need to output a 'safety factor' which is calculated in a certain way, see CalculateSafetyFactor for details
std::string Puzzle14::SolvePuzzleA(const char* fileName)
{
    std::vector<Robot> robots = ParseFile(fileName);
    
    int64_t width = 101;
    int64_t height = 103;

    for (int i = 0; i < 100; i++)
    {
        for (Robot& robot : robots)
        {
            robot.Update(width, height);
        }
    }
	return std::to_string(CalculateSafetyFactor(robots, width, height));
}

//the same robot positions and velocities are enterred into the map and we need to update the map until a picture of a christmas tree appears
//no further specification for the christmas tree is given.
//my approach was to create a 'weirdness' heuristic and print outliers to console until i could visually ID the tree.
std::string Puzzle14::SolvePuzzleB(const char* fileName)
{
    std::vector<Robot> robots = ParseFile(fileName);

    int64_t width = 101;
    int64_t height = 103;

    //expected it to be pretty deep, turned out to only be 6.5k~
    for (int64_t i = 1; i < 1000000; i++)
    {
        for (Robot& robot : robots)
        {
            robot.Update(width, height);
        }
        int64_t weirdness = CalculateWeirdness(robots, width, height);
        if (weirdness > 400)
        {
            DrawBoard(robots, width, height);
            std::cout << "\nIteration " << i << "\n";
            std::cout << "\nWeirdness " << CalculateWeirdness(robots, width, height) << "\n";
            return std::to_string(i);
        }

        if (i % 10000 == 0)
            std::cout << " .";
    }
    return "Didn't find it!";
}

bool Puzzle14::RunTests()
{
    //test case answers provided by question
    std::vector<Robot> robots = ParseFile("puzzle14/test.txt");

    int64_t width = 11;
    int64_t height = 7;

    for (int i = 0; i < 100; i++)
    {
        for (Robot& robot : robots)
        {
            robot.Update(width, height);
        }
    }
    std::string resultA = std::to_string(CalculateSafetyFactor(robots, width, height));
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "12") return false;
    //there isn't really a viable test for part b.
    return true;
}

std::vector<Puzzle14::Robot> Puzzle14::ParseFile(const char* fileName)
{
    std::vector<Robot> robots;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        bool done = false;
        int section = 0;
        while (getline(myFile, line))
        {
            //format "p=x,y v= x,y"
            // p is position v is velocity
            Robot nextRobot;
            size_t velocityIndex = line.find('v');
            std::string pos = line.substr(2, velocityIndex - 2); //offset 2 for "p="
            std::string vel = line.substr(velocityIndex + 2); //ofset 2 for "v="
            nextRobot.position.ParseCoord(pos);
            nextRobot.velocity.ParseCoord(vel);
            robots.push_back(nextRobot);
        }
    }
    return robots;
}

int64_t Puzzle14::CalculateSafetyFactor(const std::vector<Robot>& robots, int64_t width, int64_t height)
{
    int64_t northeast = 0;
    int64_t northwest = 0;
    int64_t southwest = 0;
    int64_t southeast = 0;
    for (const Robot& robot : robots)
    {
        //note: scoring criteria is slightly odd, if you are in exactly the middle you aren't counted
        //so not using 'else' and using only < and > is intentional here. cases that don't match one of the conditions are ignored *By Design*
        if (robot.position.x < width / 2)
        {
            if (robot.position.y < height / 2)
            {
                northwest++;
            }
            if (robot.position.y > height / 2)
            {
                southwest++;
            }
        }        
        if (robot.position.x > width / 2)
        {
            if (robot.position.y < height / 2)
            {
                northeast++;
            }
            if (robot.position.y > height / 2)
            {
                southeast++;
            }
        }
    }
    return northeast * northwest * southwest * southeast;
}

int64_t Puzzle14::CalculateWeirdness(const std::vector<Robot>& robots, int64_t width, int64_t height)
{
    int64_t weirdScore = 0;
    //this is a function that tries to spot outliers in the robot distribution
    //i'm not really an expert on signal analysis, I did some quick looking into calculating entropy of the string or signal to noise ratio but it doesn't look very straightforward to calculate those things if you don't know what you're looking for
    //a lot of leads just ended with 'fundamentally intractible' or 'not calculatable' so i'm just gonna wing it and look for some weird outliers, hence 'weirdness' as this isn't a real metric
    //if i wasn't using c++ I had the idea of using some kind of simple compression (since a non-random bitmap is going to compress way better than pseudorandom noisy robot positions) and look for outliers that are smaller than average
    //but we're using c++ and i don't think there's any simple STL compression algorithms and I cbf writing one lol. Maybe run length encoding would be enough to spot the outlier? which is ~vaguely~ what my second metric below is checking for anyway
    std::set<Coord> occupiedTiles;
    for (const Robot& robot : robots)
    {
        occupiedTiles.insert(robot.position);
    }
    if (occupiedTiles.size() == robots.size())
    {
        //no overlaps which is interesting.
        weirdScore += 1000;
    }

    int blankLength = 0;
    int longestBlankSequence = 0;

    ////seems too slow?
    // 
    //// update: was giving me too many false positives, just the first test was enough to visually spot it.
    ////long sequences of blank characters would be expected if e.g. the christmas tree is centred and there's lots of whitespace around it.
    // update2: apparently the 'no overlaps' check actually isn't guaranteed to work? seems to be pretty likely
    // 
    // update3: another idea that i liked that wasn't signal processing of some kind or another was to save a series of bitmaps to disk (ideally with some simple filtering like this to pick only 'interesting' bitmaps)
    // that way you can inspect lots of elements at once and saving them with sequential id filenames means you instantly know which is the correct one.
    // 
    //for (int64_t x = 0; x < width; x++)
    //{
    //    for (int64_t y = 0; y < height; y++)
    //    {
    //        if (occupiedTiles.contains(Coord(x, y)))
    //        {
    //            if (blankLength > longestBlankSequence)
    //                longestBlankSequence = blankLength;
    //            blankLength = 0;
    //        }
    //        else
    //        {
    //            blankLength++;
    //        }
    //    }
    //}

    //weirdScore += longestBlankSequence;
    

    return weirdScore;
}

void Puzzle14::Robot::Update(int64_t worldWidth, int64_t worldHeight)
{
    position = position + velocity;

    //wrap
    if (position.x < 0)
        position.x += worldWidth;
    if (position.y < 0)
        position.y += worldHeight;

    //wrap
    position.x = position.x % worldWidth;
    position.y = position.y % worldHeight;
}

void Puzzle14::DrawBoard(const std::vector<Robot>& robots, int64_t width, int64_t height)
{
    std::cout << "\n\n";
    std::set<Coord> occupiedTiles;
    for (const Robot& robot : robots)
    {
        occupiedTiles.insert(robot.position);
    }
    for (int64_t y = 0; y < height; y++)
    {
        for (int64_t x= 0; x < width; x++)
        {
            if (occupiedTiles.contains(Coord(x, y)))
            {
                std::cout << "R";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}