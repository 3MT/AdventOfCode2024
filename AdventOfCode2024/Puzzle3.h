#pragma once

#include <vector>
#include <string>

class Puzzle3
{
public:
	struct Command
	{
	public:
		enum CommandType { DO, DONT, MUL };
		CommandType type;
		int64_t argument1;
		int64_t argument2;

		int64_t Evaluate() const
		{
			return argument1 * argument2;
		}
	};

	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::vector<Command> ParseCommands(const char* fileName);
};

