#pragma once
#include <vector>
#include <string>

class Puzzle7
{
public:
	class Equation
	{
	public:
		bool IsCalculableAddMul() const;
		bool IsCalculableAddMulCon() const;
		void ParseLine(std::string line);
		uint64_t GetTarget() const { return target; }
		Equation()
		{
			target = 0;
		}
	private:
		std::vector<uint64_t> components;
		uint64_t target; //big numbers.
	};

	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();
};

