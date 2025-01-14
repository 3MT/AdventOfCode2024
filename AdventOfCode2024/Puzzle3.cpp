#include "Puzzle3.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

//https://adventofcode.com/2024/day/3
//parse a string of garbage looking for strings that match the pattern mul(X,Y) (this is a "command") where X and Y are 1-3 digit ints, then multiply X and Y each time
std::string Puzzle3::SolvePuzzleA(const char* fileName)
{
    std::vector<Puzzle3::Command> cmds = ParseCommands(fileName);
    int sum = 0;
    for (const Command& cmd : cmds)
    {
        if (cmd.type == Command::MUL)
        {
            sum += cmd.Evaluate();
        }
    }
    return std::to_string(sum);
}

//https://adventofcode.com/2024/day/3#part2
//same as part 1 but this time there are "do()" and "don't()" instructions, do turns mul on and don't turns it off. Only the most recent command counts.
std::string Puzzle3::SolvePuzzleB(const char* fileName)
{
    std::vector<Puzzle3::Command> cmds = ParseCommands(fileName);
    int sum = 0;
    bool enabled = true;
    for (const Command& cmd : cmds)
    {
        switch (cmd.type)
        {
            case Command::MUL:
            {
                if (enabled) 
                    sum += cmd.Evaluate();
                break;
            }
            case Command::DO:
            {
                enabled = true;
                break;
            }
            case Command::DONT:
            {
                enabled = false;
                break;
            }
        }
    }
    return std::to_string(sum);
}

bool Puzzle3::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle03/test.txt");
    std::string resultB = SolvePuzzleB("puzzle03/test2.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "161") return false;
    if (resultB != "48") return false;
    return true;
}

std::vector<Puzzle3::Command> Puzzle3::ParseCommands(const char* fileName)
{
    //input is garbled text with mul(x,y), do() and don't() commands interspersed.
    //we must sanitize our inputs somewhat and parse the valid commands.
    std::vector<Command> output;

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            while (line.size() > 0)
            {
                try
                {
                    std::string mulString = "mul(";
                    std::string doString = "do()";
                    std::string dontString = "don't()";

                    size_t startMul = line.find("mul(");
                    size_t startDo = line.find("do()");
                    size_t startDont = line.find("don't()");

                    if (startMul == std::string::npos && startDo == std::string::npos && startDont == std::string::npos) //nothing more to parse on this line.
                    {
                        break;
                    }

                    if (startMul < startDo && startMul < startDont) //hit multiply first, parse multiply cmd
                    {
                        //question spec says we are looking for things of the pattern 'mul(X,Y)' where X and Y are 1-3 digit positive ints.

                        size_t maxDigits = 3;
                        maxDigits += 1; //1 char pad
                        size_t num1Start = startMul + mulString.size();
                        size_t comma = line.find(",", num1Start);
                        if (comma == std::string::npos || comma - num1Start > maxDigits) //invalid input
                        {
                            line = line.substr(num1Start); //cut out everything up to mul
                            continue;
                        }

                        size_t num2Start = comma + 1;
                        size_t closingParen = line.find(")", comma);
                        if (closingParen == std::string::npos || closingParen - num2Start > maxDigits) //invalid input
                        {
                            line = line.substr(num1Start); //cut out everything up to mul
                            continue;
                        }

                        std::string num1String = line.substr(num1Start, comma - num1Start);
                        std::string num2String = line.substr(num2Start, closingParen - num2Start);

                        std::size_t index = 0;
                        Command newCommand = {};
                        newCommand.type = Command::MUL;
                        newCommand.argument1 = std::stoll(num1String, &index);

                        //annoying workaround for std::stoll being trigger happy, in my case it parsed "49<" as 49 which is illegal per the questions spec. 
                        //I guess the good news is I learned another annoying caveat to std::stoll.
                        //kinda bad practice to be doing all this stuff by hand anyway, in a real project i'd use rapidjson or something along those lines rather than hand parse the input but hey.
                        if (index != num1String.size()) 
                        {
                            line = line.substr(num1Start); //cut out everything up to mul
                            continue;
                        }
                        index = 0;
                        newCommand.argument2 = std::stoll(num2String, &index);
                        //same workaround here.
                        if (index != num2String.size())
                        {
                            line = line.substr(num1Start); //cut out everything up to mul
                            continue;
                        }

                        output.push_back(newCommand);

                        //cull the part we parsed from the string
                        line = line.substr(closingParen + 1);
                    }
                    else if (startDo < startMul && startDo < startDont) //hit do cmd first, parse do
                    {
                        Command newCommand = {};
                        newCommand.type = Command::DO;
                        newCommand.argument1 = 0;
                        newCommand.argument2 = 0;
                        output.push_back(newCommand);
                        line = line.substr(startDo + doString.size()); //cull used part of the string
                    }
                    else if (startDont < startMul && startDont < startDo) //hit dont cmd first, parse dont
                    {
                        Command newCommand = {};
                        newCommand.type = Command::DONT;
                        newCommand.argument1 = 0;
                        newCommand.argument2 = 0;
                        output.push_back(newCommand);
                        line = line.substr(startDont + dontString.size()); //cull used part of the string
                    }
                }
                catch (...)
                {
                    //parse failed.
                    std::cout << "Parse Failed";
                    break;
                }
            }
        }
        myFile.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
    return output;

    //parsing this was about 5000x harder than the puzzle... gotta love c++... this is probably like 2 lines of python lmao
}
