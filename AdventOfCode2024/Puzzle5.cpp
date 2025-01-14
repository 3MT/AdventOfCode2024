#include "Puzzle5.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <algorithm>

//https://adventofcode.com/2024/day/5
//Given 2 inputs in one file, a list of page rules and a second set of lists of pages.
//the way page rules work is they are in the form XX|YY where X and Y are two digit integers.
//within each page list, a list is valid if it obeys every page rule.
//to obey a page rule, if both integers in the rule appear in the list, the left digit must appear to the left of the right digit *at some point* in the list.
//if only one or zero integers in the rule appear, we can ignore the rule.
std::string Puzzle5::SolvePuzzleA(const char* fileName)
{
	std::vector<PageRule> rules;
	std::vector<std::vector<int64_t>> pageLines;
	std::ifstream myFile(fileName);

	//parse.
	if (myFile.is_open())
	{
		std::string line;
		bool parsingRules = true;
		while (getline(myFile, line))
		{
			if (parsingRules)
			{
				if (line.size() != 0)
				{
					try
					{
						rules.push_back(ParseRule(line));
					}
					catch (...) //catch failed stoll
					{
						std::cout << "Parse failed";
					}
				}
				else
				{
					//blank newline means we are now parsing page details
					parsingRules = false;
				}
			}
			else
			{
				if (line.size() != 0)
				{
					std::vector<int64_t> newLine = ParsePages(line);
					pageLines.push_back(newLine);
				}
			}
		}
	}

	//the output of this is the sum of the central digit in all lines that pass the rules check.
	int64_t sum = 0;
	//solve
	for (const std::vector<int64_t>& line : pageLines)
	{
		bool failedRule = false;
		for (const PageRule& rule : rules)
		{
			if (!rule.CheckRule(line))
			{
				failedRule = true;
				break;
			}
		}
		if (!failedRule)
		{
			sum += line[line.size() / 2]; //middle page
		}
	}

	return std::to_string(sum);
}
//https://adventofcode.com/2024/day/5#part2
//part b is to take the same rules as part a, but when a list fails the rules we have to change it so that it obeys the rules
//then we take the lists that failed the test and were fixed and output the sum of their middle pages, as in part a
std::string Puzzle5::SolvePuzzleB(const char* fileName)
{
	std::vector<PageRule> rules;
	std::vector<std::vector<int64_t>> pageLines;
	std::ifstream myFile(fileName);

	//parse.
	if (myFile.is_open())
	{
		std::string line;
		bool parsingRules = true;
		while (getline(myFile, line))
		{
			if (parsingRules)
			{
				if (line.size() != 0)
				{
					try
					{
						rules.push_back(ParseRule(line));
					}
					catch (...) //catch failed stoll
					{
						std::cout << "Parse failed";
					}
				}
				else
				{
					//blank newline means we are now parsing page details
					parsingRules = false;
				}
			}
			else
			{
				if (line.size() != 0)
				{
					std::vector<int64_t> newLine = ParsePages(line);
					pageLines.push_back(newLine);
				}
			}
		}
	}

	std::vector<std::vector<int64_t>> badLines;
	//find bad lines
	for (const std::vector<int64_t>& line : pageLines)
	{
		bool failedRule = false;
		for (const PageRule& rule : rules)
		{
			if (!rule.CheckRule(line))
			{
				failedRule = true;
				break;
			}
		}
		if (failedRule)
		{
			badLines.push_back(line);
		}
	}

	//fix bad lines
	for (std::vector<int64_t>& line : badLines)
	{
		std::sort(line.begin(), line.end(), [&](int64_t a, int64_t b)
		{
			for (const PageRule& rule : rules)
			{
				if (!rule.CheckRule(a, b))
					return false;
			}
			return true;
		});
	}

	//sum middle terms
	int64_t sum = 0;
	//solve
	for (const std::vector<int64_t>& line : badLines)
	{
		sum += line[line.size() / 2]; //middle page
	}
	return std::to_string(sum);
}

bool Puzzle5::RunTests()
{
	//test case answers provided by question
	std::string resultA = SolvePuzzleA("puzzle05/test.txt");
	std::string resultB = SolvePuzzleB("puzzle05/test.txt");
#if DEBUG_OUTPUT
	std::cout << "A: " << resultA << "\n";
	std::cout << "B: " << resultB << "\n";
#endif
	if (resultA != "143") return false;
	if (resultB != "123") return false;
	return true;
}

Puzzle5::PageRule Puzzle5::ParseRule(const std::string& line)
{
	// rules are always in the form XX|XX where X is ints
	int64_t left = std::stoll(line.substr(0, 2));
	int64_t right = std::stoll(line.substr(3, 2));
	return PageRule(left,right);
}

std::vector<int64_t> Puzzle5::ParsePages(const std::string& line)
{
	std::vector<int64_t> lineOutput;
	size_t parsePosition = 0;
	while (line.size() > 0)
	{
		try
		{
			size_t firstComma = line.find(',', parsePosition);
			int64_t value = 0;
			value = std::stoll(line.substr(parsePosition, firstComma));
			lineOutput.push_back(value);

			if (firstComma == std::string::npos) //we have validated the last number (no more values)
			{
				break;
			}
			//cull the part we parsed from the string
			parsePosition = firstComma + 1;
		}
		catch (...)
		{
			//parse failed.
			std::cout << "Parse Failed";
			break;
		}
	}
	return lineOutput;
}

bool Puzzle5::PageRule::CheckRule(const std::vector<int64_t>& input) const
{
	int64_t leftIndex = -1;
	int64_t rightIndex = -1;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == left)
			leftIndex = i;
		if (input[i] == right)
			rightIndex = i;
	}
	if (leftIndex == -1 || rightIndex == -1) //didn't find one of the inputs, pass by default
		return true;
	return (leftIndex < rightIndex);
}

bool Puzzle5::PageRule::CheckRule(int64_t a, int64_t b) const
{
	if (a == right && b == left)
		return false;
	return true;
}