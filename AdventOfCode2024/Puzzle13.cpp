#include "Puzzle13.h"

#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//Given a claw machine with properties:
//Button A: X+94, Y+34
//Button B: X+22, Y+67
//Prize: X=8400, Y=5400
//where button A moves the claw X and Y units, B does the same, and Prize is the coordinates of our target position
//and button A costs 3 coins while button B costs 1 coin
//we are tasked with finding how many coins it would take to win all possible prizes (we do not attempt impossible claw machines)
std::string Puzzle13::SolvePuzzleA(const char* fileName)
{
    std::vector<ClawMachine> machines;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        bool done = false;
        int section = 0;
        ClawMachine nextMachine;
        while (getline(myFile, line))
        {
            //format
            //Button A: X+69, Y+23
            //Button B: X+27, Y+71
            //Prize: X=18641, Y=10279
            if (section == 0)
            {
                size_t xpos = line.find("X+") + 2;
                size_t ypos = line.find("Y+") + 2;
                nextMachine.buttonA.x = std::stoll(line.substr(xpos, 2));
                nextMachine.buttonA.y = std::stoll(line.substr(ypos, 2));
            }
            else if (section == 1)
            {
                size_t xpos = line.find("X+") + 2;
                size_t ypos = line.find("Y+") + 2;
                nextMachine.buttonB.x = std::stoll(line.substr(xpos, 2));
                nextMachine.buttonB.y = std::stoll(line.substr(ypos, 2));
            }
            else if (section == 2)
            {
                size_t xpos = line.find("X=") + 2;
                size_t xposEnd = line.find(",");
                size_t ypos = line.find("Y=") + 2;
                nextMachine.target.x = std::stoll(line.substr(xpos, xposEnd - xpos));
                nextMachine.target.y = std::stoll(line.substr(ypos));
            }
            else if (section == 3)
            {
                machines.push_back(nextMachine);
                nextMachine = ClawMachine();
            }
            section++;
            if (section > 3)
                section = 0;
        }
    }

    int sum = 0;

    for (ClawMachine& machine : machines)
    {
        int cost = machine.FindWinSolver();
        if (cost > 0)
            sum += cost;
    }

    return std::to_string(sum);
}

//same as part 1 however we add 10000000000000 (yes thats not a typo) to the X and Y coords of the prize position.
std::string Puzzle13::SolvePuzzleB(const char* fileName)
{
    std::vector<ClawMachine> machines;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        bool done = false;
        int section = 0;
        ClawMachine nextMachine;
        while (getline(myFile, line))
        {
            //Button A: X+69, Y+23
            //Button B: X+27, Y+71
            //Prize: X=18641, Y=10279
            if (section == 0)
            {
                size_t xpos = line.find("X+") + 2;
                size_t ypos = line.find("Y+") + 2;
                nextMachine.buttonA.x = std::stoll(line.substr(xpos, 2));
                nextMachine.buttonA.y = std::stoll(line.substr(ypos, 2));
            }
            else if (section == 1)
            {
                size_t xpos = line.find("X+") + 2;
                size_t ypos = line.find("Y+") + 2;
                nextMachine.buttonB.x = std::stoll(line.substr(xpos, 2));
                nextMachine.buttonB.y = std::stoll(line.substr(ypos, 2));
            }
            else if (section == 2)
            {
                size_t xpos = line.find("X=") + 2;
                size_t xposEnd = line.find(",");
                size_t ypos = line.find("Y=") + 2;
                nextMachine.target.x = std::stoll(line.substr(xpos, xposEnd - xpos)) + 10000000000000;
                nextMachine.target.y = std::stoll(line.substr(ypos)) + 10000000000000;
            }
            else if (section == 3)
            {
                machines.push_back(nextMachine);
                nextMachine = ClawMachine();
            }
            section++;
            if (section > 3)
                section = 0;
        }
    }

    int64_t sum = 0;

    for (ClawMachine& machine : machines)
    {
#if DEBUG_OUTPUT
        std::cout << ". "; //visualising how fast processing is going.
#endif
        int64_t cost = machine.FindWinSolver();
        if (cost > 0)
            sum += cost;
    }

    return std::to_string(sum);
}

bool Puzzle13::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle13/test.txt");
    //sadly no test data for b :(
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "480") return false;
    return true;
}

int64_t Puzzle13::ClawMachine::FindWinSolver()
{
    //solved on paper, essentially it's just solving two linear equations with two variables
    //was a reasonably straightforward solve by substitution, hard to express in a comment but more or less:
            //Button A: X+69, Y+23
            //Button B: X+27, Y+71
            //Prize: X=18641, Y=10279

    //EQNs:
    // 69a + 27b = 18641
    // 23a + 71b = 10279
    // 
    //solve by substitution:
    // a = ( 18641 - 27b ) / 69
    // substitute in eqn 2
    // 23a + ( 71 ( ( 18641 - 27b ) / 69 ) )
    // 
    //etc. etc., swap out the constants for constant variables (lol)
    //and it subsitutes and sorts out to:

            //Button A: X+M, Y+P
            //Button B: X+N, Y+Q
            //Prize: X=O, Y=R

    // b = (RM - PO) / (MQ - PN)
    // a = (O - Nb) / M

    int64_t M, N, O, P, Q, R;
    M = buttonA.x;
    N = buttonB.x;
    O = target.x;
    P = buttonA.y;
    Q = buttonB.y;
    R = target.y;

    int64_t b = ((R * M) - (P * O)) / ((M * Q) - (P * N));
    int64_t a = (O - (N * b)) / M;

    Coord sum = (buttonB * b) + (buttonA * a);
    if (sum == target) //if the sum doesn't match, the solution doesn't match the target, interestingly all the inputs are pretty close to the solution (+/- ~20), so potentially in a more real life situation we would wanna validate our inputs a bit more than here to avoid div by zero
        return b + (3 * a);
    else
        return -1;
}

int64_t Puzzle13::ClawMachine::FindWinNaive()
{
    //first naive solution.
    float startratio = (float)target.x / (float) target.y;
    float aRatio = (float)buttonA.x / (float)buttonA.y;
    float bRatio = (float)buttonB.x / (float) buttonB.y;

    Coord runningTotal(0, 0);
    bool partA = true;
    int64_t cost = 0;

    bool growing = startratio < bRatio;

    for (size_t i = 0; i < 100; i++)
    {
        Coord diff = target - runningTotal;
        float diffRatio = (float)diff.x / (float)diff.y;

        if (diffRatio == bRatio)
        {
            partA = false;
            i = 0; //reset the press counter for part 2 (100 presses per button max)
        }
        if (growing && diffRatio > bRatio)
        {
            return -1; //overshot
        }
        if (!growing && diffRatio < bRatio)
        {
            return -1; //undershot
        }
        if (partA)
        {
            runningTotal = runningTotal + buttonA;
            cost += 3;
        }
        else
        {
            runningTotal = runningTotal + buttonB;
            cost += 1;
        }
        if (runningTotal == target)
        {
            return cost;
            break;
        }
    }
    return -1;
}
