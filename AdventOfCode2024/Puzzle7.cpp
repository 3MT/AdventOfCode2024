#include "Puzzle7.h"

#include <algorithm>
#include <assert.h>>
#include <cmath>
#include <fstream>>
#include <iostream>

//https://adventofcode.com/2024/day/7
//we are given an equation in the format z: a b c d e
//the equation is valid if some combination of operators (* and +) of the right set of digits will equate to z (order of operations is always left to right)
//then we are to sum all equations in the list that are possible.
std::string Puzzle7::SolvePuzzleA(const char* fileName)
{
    std::vector<Equation> equations;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            Equation eqn;
            eqn.ParseLine(line);
            equations.push_back(eqn);
        }
    }

    uint64_t sum = 0;
    uint64_t maxSum = 0;
    for (const Equation& eqn : equations)
    {
        if (eqn.IsCalculableAddMul())
        {
            sum += eqn.GetTarget();
        }
        maxSum += eqn.GetTarget();
    }
	return std::to_string(sum);
}

//https://adventofcode.com/2024/day/7#part2
//as part 1, but the addition of the concatenation operator, which concatenates two integers
//e.g. 42 || 92 = 4292
std::string Puzzle7::SolvePuzzleB(const char* fileName)
{
    std::vector<Equation> equations;
    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            Equation eqn;
            eqn.ParseLine(line);
            equations.push_back(eqn);
        }
    }

    uint64_t sum = 0;
    uint64_t maxSum = 0;
    for (const Equation& eqn : equations)
    {
        if (eqn.IsCalculableAddMulCon())
        {
            sum += eqn.GetTarget();
        }
        maxSum += eqn.GetTarget();
    }
    return std::to_string(sum);
}

bool Puzzle7::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle07/test.txt");
    std::string resultB = SolvePuzzleB("puzzle07/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    if (resultA != "3749") return false;
    if (resultB != "11387") return false;
    return true;
}

bool Puzzle7::Equation::IsCalculableAddMul() const
{
    uint64_t lowerBound = 0;
    uint64_t upperBound = 1;
    
    for (uint64_t component : components)
    {
        if (component == 1)
        {
            //really subtle thing here, multiplying by 1 is actually less then adding 1, and adding 1 is greater than multiplying
            //so in this case we actually reverse the calc!
            //lowerBound *= 1;
            upperBound += 1;
        }
        else
        {
            lowerBound += component;
            upperBound *= component;
        }
    }
    if (target < lowerBound || target > upperBound)
        return false; //can't calc this with these components, too big or small
    if (target == lowerBound || target == upperBound)
        return true;

    //iterate through all possible operations...
    std::string mulPermutations;

    for (size_t addCount = 0; addCount < components.size(); addCount++) //-1 since number of operations is always 1 less than total count
    {
        mulPermutations.clear();
        for (size_t i = 0; i < components.size() - 1; i++)
        {
            //order matters as next_permutation goes from first to last in sort order
            if (i < addCount)
            {
                mulPermutations.push_back('a');
            }
            else
            {
                mulPermutations.push_back('m');
            }
        }

        do
        {
            uint64_t sum = components[0];
            for (size_t i = 0; i < mulPermutations.size(); i++)
            {
                if (mulPermutations[i] == 'm')
                {
                    sum *= components[i + 1];
                }
                else
                {
                    sum += components[i + 1];
                }

                //if (sum > target)
                //{
                //    break;
                //}
            }
            if (sum == target)
            {
                return true; // found match, jobs done
            }
        } while (std::next_permutation(mulPermutations.begin(), mulPermutations.end()));

    }
    return false;
}

uint64_t ConcatIntegers(uint64_t x, uint64_t y)
{
    //potentially some accuracy issues at insanely large numbers since we are casting to doubles here
    //but I got the correct answer so I think we are pretty safe.
    //don't put this function in your spacecraft's life support system code is all I'm saying.
    int64_t digits = (int64_t)(std::log10l((long double)y)) + 1;
    return (uint64_t)(std::pow(10, digits) * x) + y;
}

bool Puzzle7::Equation::IsCalculableAddMulCon() const
{
    uint64_t lowerBound = 0;
    uint64_t upperBound = 0;

    for (uint64_t component : components)
    {
        if (component == 1)
        {
            //same edge case as above but slightly different since concating by 1 is the best way to grow
            //lowerBound *= 1;
            upperBound = ConcatIntegers(upperBound, component);
        }
        else
        {
            lowerBound += component;
            upperBound = ConcatIntegers(upperBound, component);
        }
    }
    if (target < lowerBound || target > upperBound)
        return false; //can't calc this with these components, too big or small
    if (target == lowerBound || target == upperBound)
        return true;


    //iterate through all possible operations...
    std::string mulPermutations;

    for (size_t addCount = 0; addCount < components.size(); addCount++) //-1 since number of operations is always 1 less than total count
    {
        for (size_t conCount = 0; addCount + conCount < components.size(); conCount++)
        {
            mulPermutations.clear();
            for (size_t i = 0; i < components.size() - 1; i++)
            {
                //order matters as next_permutation goes from first to last in sort order
                if (i < addCount)
                {
                    mulPermutations.push_back('a');
                }
                else if (i < addCount + conCount)
                {
                    mulPermutations.push_back('c');
                }
                else
                {
                    mulPermutations.push_back('m');
                }
            }

            do
            {
                uint64_t sum = components[0];
                for (size_t i = 0; i < mulPermutations.size(); i++)
                {
                    if (mulPermutations[i] == 'm')
                    {
                        sum *= components[i + 1];
                    }
                    else if (mulPermutations[i] == 'c')
                    {
                        /* naive string concat, this is fast enough but in release it took about 6 seconds to calc which feels a bit too slow
                        //this is probably fast enough though.
                        std::string left = std::to_string(sum);
                        std::string right = std::to_string(components[i + 1]);
                        std::string concat = left + right;
                        
                        sum = std::stoll(concat);
                        */
                        sum = ConcatIntegers(sum, components[i + 1]); //about 3x faster than string ops, still kinda slow, maybe there's a non brute force way?
                    }
                    else
                    {
                        sum += components[i + 1];
                    }

                    if (sum > target)
                    {
                        break;
                    }
                }
                if (sum == target)
                {
                    return true; // found match, jobs done
                }
            } while (std::next_permutation(mulPermutations.begin(), mulPermutations.end()));

        }
    }
    return false;
}

//equation format X: a b c d e
//X: positive integer, 'target' of eqn
//a,b,c,d... : positive integers, components of the eqn
void Puzzle7::Equation::ParseLine(std::string line)
{
    try
    {
        size_t index = 0;

        target = std::stoll(line, &index);
        index += 2; // ": "

        line = line.substr(index);
        index = 0;
        while (index < std::string::npos)
        {
            components.push_back(std::stoll(line, &index));
            index++;

            //parsing line done
            if (index >= line.length())
                break;
            
            //consume used input
            line = line.substr(index);
        }
    }
    catch (...)
    {
        std::cout << "Parse failed";
    }
}
