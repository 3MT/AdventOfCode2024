#pragma once
#include <string>
#include <vector>

//using classes as static namespaces since most puzzles will be procedural.
//if an object is needed, I will define them as a subclass to the puzzle namespace.
//I thought about inheritance but because the functions are mostly static it causes more problems than it solves.
//treating puzzle solvers as objects does have some pros, like reusing state between part a and b, but I wanted
//to practice writing more pure style functions for this AoC, so I went with this stylistic choice.

//Also, puzzle solutions will be returned as strings because a few later puzzles request string output.
//By convention we will use int64s everywhere as many of the questions end up reaching VERY large numbers.
class Puzzle1
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);

	//For early puzzles, tests are generally just running the solver on test data, however I will run them as a separate function
	//because for later puzzles, test cases can get complex, specific or only test specific components of the solution.
	static bool RunTests();

private:
	//generally each puzzle will parse data in a second function. Some questions can be solved as you parse but I prefer putting the input into
	//data structures, then operating on the data in memory rather than trying to calculate while parsing.
	//This generates slightly more verbose code, but is easier to work with and is generally safer as often part 1 can be solved during the parse but part 2 will work better in a data structure.
	//better to stick to parsing first then solving as a matter of consistency.
	static void ParseArrays(const char* fileName, std::vector<int64_t>& outLeftValues, std::vector<int64_t>& outRightValues);
	static bool ParseLine(const std::string& inputString, int64_t& out1, int64_t& out2);
};

