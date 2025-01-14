#include "Puzzle21.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <queue>
#include <assert.h>

#include "Coord.h"

//https://adventofcode.com/2024/day/21
//For this puzzle we have a keypad (see below for a diagram) and multiple directional pads (also below)
//the keypad is controlled by a robot that can press buttons, and so are the directional pads.
//each dpad controls the robot below it, so if a robot presses the left key on a dpad, the lower robot would move 1 button left.
//the A key tells the robot to press the button it is currently over.
//finally, robots can never exit the grid or hover over the blank key.
//our goal for this puzzle is to find the shortest sequence of keys the highest level robot can input to give the given keypad input.
//for each sequence we calculate a complexity, this is the numerical value for the numpad and the number of key presses required for the highest level directional pad.
//so if you had to input '123' and it took you 10 presses on the highest dirpad, it would have a complexity of 1230.
//we are to sum the complexity scores for each input given for a system with a keypad and 2 directional pad robots (and us at the top, pressing the highest level dpad)
std::string Puzzle21::SolvePuzzleA(const char* fileName)
{
    std::vector<std::string> codes;
    std::fstream file(fileName);

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            codes.push_back(line);
        }
    }

    //+---+---+---+
    //| 7 | 8 | 9 |
    //+---+---+---+
    //| 4 | 5 | 6 |
    //+---+---+---+
    //| 1 | 2 | 3 |
    //+---+---+---+
    //    | 0 | A |
    //    +---+---+
    std::map<char, Coord> numKeys;
    numKeys['0'] = Coord(1, 3);
    numKeys['1'] = Coord(0, 2);
    numKeys['2'] = Coord(1, 2);
    numKeys['3'] = Coord(2, 2);
    numKeys['4'] = Coord(0, 1);
    numKeys['5'] = Coord(1, 1);
    numKeys['6'] = Coord(2, 1);
    numKeys['7'] = Coord(0, 0);
    numKeys['8'] = Coord(1, 0);
    numKeys['9'] = Coord(2, 0);
    numKeys['A'] = Coord(2, 3);

    //    +---+---+
    //    | ^ | A |
    //+---+---+---+
    //| < | v | > |
    //+---+---+---+
    std::map<char, Coord> dirKeys;
    dirKeys['^'] = Coord(1, 0);
    dirKeys['<'] = Coord(0, 1);
    dirKeys['>'] = Coord(2, 1);
    dirKeys['v'] = Coord(1, 1);
    dirKeys['A'] = Coord(2, 0);

    Coord blankNumCoord(0, 3);
    Coord blankDirCoord(0, 0);
    int64_t scoreSum = 0;
    std::vector<std::map<std::string, int64_t>> memoisationTable;

    int depth = 2;
    memoisationTable.resize(depth);

    for (std::string& code : codes)
    {
        int64_t sum = 0;

        std::string initialMoveSet;
        char lastKey = 'A';
        for (int i = 0; i < code.size(); i++)
        {
            if (lastKey == code[i])
                initialMoveSet.append("A");
            initialMoveSet.append(GetBestNumpadPath(numKeys[lastKey], numKeys[code[i]]));
            lastKey = code[i];
        }



        std::vector<std::string> subPaths = SplitMoveset(initialMoveSet);
        for (std::string& path : subPaths)
        {
            sum += DetermineMovelength(path, 0, depth, dirKeys, memoisationTable);
        }
        scoreSum += ScoreMoveset(sum, code);
    }

    return std::to_string(scoreSum);
}

std::map<Coord, int64_t> c_Requests; //debug


//https://adventofcode.com/2024/day/21#part2
//as part 1, however now there are 25 dpad robots in between us and the keypad.
//this results in an utterly insane level of growth, similar to the stones puzzle earlier we track frequencies and memoise how each keypad expands as it rises the input hierarchy.
std::string Puzzle21::SolvePuzzleB(const char* fileName)
{
    std::vector<std::string> codes;
    std::fstream file(fileName);

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            codes.push_back(line);
        }
    }

    //+---+---+---+
    //| 7 | 8 | 9 |
    //+---+---+---+
    //| 4 | 5 | 6 |
    //+---+---+---+
    //| 1 | 2 | 3 |
    //+---+---+---+
    //    | 0 | A |
    //    +---+---+
    std::map<char, Coord> numKeys;
    numKeys['0'] = Coord(1, 3);
    numKeys['1'] = Coord(0, 2);
    numKeys['2'] = Coord(1, 2);
    numKeys['3'] = Coord(2, 2);
    numKeys['4'] = Coord(0, 1);
    numKeys['5'] = Coord(1, 1);
    numKeys['6'] = Coord(2, 1);
    numKeys['7'] = Coord(0, 0);
    numKeys['8'] = Coord(1, 0);
    numKeys['9'] = Coord(2, 0);
    numKeys['A'] = Coord(2, 3);

    //    +---+---+
    //    | ^ | A |
    //+---+---+---+
    //| < | v | > |
    //+---+---+---+
    std::map<char, Coord> dirKeys;
    dirKeys['^'] = Coord(1, 0);
    dirKeys['<'] = Coord(0, 1);
    dirKeys['>'] = Coord(2, 1);
    dirKeys['v'] = Coord(1, 1);
    dirKeys['A'] = Coord(2, 0);

    Coord blankNumCoord(0, 3);
    Coord blankDirCoord(0, 0);
    int64_t scoreSum = 0;
    std::vector<std::map<std::string, int64_t>> memoisationTable;

    int depth = 25;
    memoisationTable.resize(depth);

    for (std::string& code : codes)
    {
        int64_t sum = 0;

        std::string initialMoveSet;
        char lastKey = 'A';
        for (int i = 0; i < code.size(); i++)
        {
            if (lastKey == code[i])
                initialMoveSet.append("A");
            initialMoveSet.append(GetBestNumpadPath(numKeys[lastKey], numKeys[code[i]]));
            lastKey = code[i];
        }



        std::vector<std::string> subPaths = SplitMoveset(initialMoveSet);
        for (std::string& path : subPaths)
        {
            sum += DetermineMovelength(path, 0, depth, dirKeys, memoisationTable);
        }
        scoreSum += ScoreMoveset(sum, code);
    }

    return std::to_string(scoreSum);
}


bool Puzzle21::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle21/test.txt");
    //part b has no test case sadly. would have been REALLY helpful.
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "126384") return false;
    return true;
}

//every time we hit 'A' we know all keypad bots above us are synced, so it's a good place to split as the state is 'reset'.
std::vector<std::string> Puzzle21::SplitMoveset(const std::string& input)
{
    std::vector<std::string> output;
    size_t lastIndex = 0;
    size_t index = input.find('A');
    while (index < input.size())
    {
        output.push_back(input.substr(lastIndex, (index + 1) - lastIndex));
        lastIndex = index + 1;
        index = input.find('A', lastIndex);
    }
    return output;
}

std::string Puzzle21::GetBestDirPath(Coord start, Coord end)
{
    std::string horizontalMove;
    horizontalMove.resize(abs(start.x - end.x), start.x > end.x ? '<' : '>');
    std::string verticalMove;
    verticalMove.resize(abs(start.y - end.y), start.y > end.y ? '^' : 'v');
    
    bool horizontalFirst;
    //special case < key
    if (start == Coord(0, 1))
    {
        horizontalFirst = true;
    }
    else if (end == Coord(0, 1))
    {
        horizontalFirst = false;
    }
    else if (start.x > end.x)
    {
        horizontalFirst = true;
    }
    else
    {
        horizontalFirst = false;
    }

    if (horizontalFirst)
    {
        return horizontalMove + verticalMove + "A";
    }
    else
    {
        return verticalMove + horizontalMove + "A";
    }
}

std::string Puzzle21::GetBestNumpadPath(Coord start, Coord end)
{
    std::string horizontalMove;
    horizontalMove.resize(abs(start.x - end.x), start.x > end.x ? '<' : '>');
    std::string verticalMove;
    verticalMove.resize(abs(start.y - end.y), start.y > end.y ? '^' : 'v');

    bool horizontalFirst;
    //special case moving from bottom row to left col
    if (start.y == 3 && end.x == 0)
    {
        horizontalFirst = false;
    }
    else if (start.x == 0 && end.y == 3)
    {
        horizontalFirst = true;
    }
    else if (start.x > end.x)
    {
        horizontalFirst = true;
    }
    else
    {
        horizontalFirst = false;
    }

    if (horizontalFirst)
    {
        return horizontalMove + verticalMove + "A";
    }
    else
    {
        return verticalMove + horizontalMove + "A";
    }
}

int64_t Puzzle21::DetermineMovelength(const std::string& keyPresses, int64_t depth, int64_t maxDepth, std::map<char, Coord> dirKeys, std::vector<std::map<std::string, int64_t>>& memoisationTable)
{
    if (depth == maxDepth)
    {
        return keyPresses.size();
    }
    //check if memoised
    if (memoisationTable[depth].contains(keyPresses))
    {
        return memoisationTable[depth][keyPresses];
    }

    //generate best path
    std::string initialMoveSet;
    char lastKey = 'A';
    for (int i = 0; i < keyPresses.size(); i++)
    {
        initialMoveSet.append(GetBestDirPath(dirKeys[lastKey], dirKeys[keyPresses[i]]));
        lastKey = keyPresses[i];
    }

    //DetermineMoveLength() of sub paths
    std::vector<std::string> subPaths = SplitMoveset(initialMoveSet);
    int64_t sum = 0;
    for (std::string& path : subPaths)
    {
        sum += DetermineMovelength(path, depth + 1, maxDepth, dirKeys, memoisationTable);
    }

    //memoise the input parameters
    memoisationTable[depth][keyPresses] = sum;
    return sum;
}

int64_t Puzzle21::ScoreMoveset(int64_t movesetLength, const std::string& keyCombo)
{
    int64_t keyComboNumeric = 0;
    try
    {
        std::string key(keyCombo.begin(), keyCombo.end());
        keyComboNumeric = std::stoll(key);
    }
    catch (...)
    {
        std::cout << "Parse failure";
    }
    return movesetLength * keyComboNumeric;
}
