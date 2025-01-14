#include "Puzzle19.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>

//https://adventofcode.com/2024/day/19
//given a series of 'sub-towels' that are colored some combo of rgb (represented as strings of literally rgb)
//we are tasked with finding if other sequences of rgb can be constructed by combining sub-towel designs.
//part A is to determine how many requested designs are possible with the given set of sub towels.
std::string Puzzle19::SolvePuzzleA(const char* fileName)
{
	std::vector<std::set<std::string>> patterns; //patterns by length
	std::vector<std::string> designs;
	ParseFile(fileName, patterns, designs);

	int64_t possibleSum = 0;
	for (size_t i = 0; i < designs.size(); i++)
	{
		if (IsDesignPossible(designs[i], patterns))
		{
			possibleSum++;
		}
	}
	return std::to_string(possibleSum);
}

//https://adventofcode.com/2024/day/19#part2
//part b is to count exhaustively every possible way to produce the same designs as part 1.
//this becomes gigantic immediately so we needed an approach to shrink the numbers to manageable amounts
//so I went with a depth first search with memoisation to speed up checks.
std::string Puzzle19::SolvePuzzleB(const char* fileName)
{
	std::vector<std::set<std::string>> patterns; //patterns by length
	std::vector<std::string> designs;
	ParseFile(fileName, patterns, designs);

	int64_t possibleSum = 0;
	std::map<std::string, int64_t> cachedScores; //score is number of valid sub designs
	for (size_t i = 0; i < designs.size(); i++)
	{
		int64_t sum = CountPossibleDesigns(designs[i], cachedScores, patterns);
		possibleSum += sum;
	}
	return std::to_string(possibleSum);
}

void Puzzle19::ParseFile(const char* fileName, std::vector<std::set<std::string>>& outPatterns, std::vector<std::string>& outDesigns)
{
	std::ifstream myFile(fileName);

	//parse.
	if (myFile.is_open())
	{
		std::string line;
		int64_t lineCount = 0;
		while (getline(myFile, line))
		{
			try
			{
				//comma separated patterns
				if (lineCount == 0)
				{
					size_t commaIndex = 0;
					while (commaIndex != std::string::npos)
					{
						commaIndex = line.find(", ");
						if (commaIndex != std::string::npos)
						{
							if (outPatterns.size() <= commaIndex)
								outPatterns.resize(commaIndex + 1);
							outPatterns[commaIndex].insert(line.substr(0, commaIndex));
							line = line.substr(commaIndex + 2);
						}
						else
						{
							//last string
							if (outPatterns.size() <= line.size())
								outPatterns.resize(commaIndex + 1);
							outPatterns[line.size()].insert(line);
							line = line.substr(commaIndex + 2);
						}
					}
				}
				if (lineCount > 1)
				{
					outDesigns.push_back(line);
				}
				lineCount++;
			}
			catch (...) //catch failed stoll
			{
				std::cout << "Parse failed";
			}
		}
	}
}

bool Puzzle19::RunTests()
{
	//test case answers provided by question
	std::string resultA = SolvePuzzleA("puzzle19/test.txt");
	std::string resultB = SolvePuzzleB("puzzle19/test.txt");
#if DEBUG_OUTPUT
	std::cout << "A: " << resultA << "\n";
	std::cout << "B: " << resultB << "\n";
#endif
	if (resultA != "6") return false;
	if (resultB != "16") return false;
	return true;
}

//naive recursion solution, seemed to work well enough
bool Puzzle19::IsDesignPossible(const std::string& design, const std::vector<std::set<std::string>>& patterns)
{
	if (design.size() == 0)
	{
		return true; //consumed the whole design.
	}
	bool isPossible = false;
	for (size_t i = 1; i < std::min(patterns.size(), design.size()+1); i++)
	{
		std::string leadingString = design.substr(0, i);
		if (patterns[i].contains(leadingString))
		{
			if (IsDesignPossible(design.substr(i), patterns))
				return true;
		}
	}
	return false;
}

//still recursive which i dislike but it makes memoization very simple.
int64_t Puzzle19::CountPossibleDesigns(const std::string& design, std::map<std::string, int64_t>& scoreCache, const std::vector<std::set<std::string>>& patterns)
{
	if (scoreCache.contains(design))
		return scoreCache[design];

	if (design.size() == 0)
	{
		return 1; //consumed the whole design.
	}
	int64_t possibleSubdesigns = 0;
	for (size_t i = 1; i < std::min(patterns.size(), design.size()+1); i++)
	{
		std::string leadingString = design.substr(0, i);
		if (patterns[i].contains(leadingString))
		{
			possibleSubdesigns += CountPossibleDesigns(design.substr(i), scoreCache, patterns);
		}
	}
	scoreCache[design] = possibleSubdesigns;
	return possibleSubdesigns;
}