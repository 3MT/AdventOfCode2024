#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <unordered_set>

class Puzzle23
{
public:
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	struct LanComputer
	{
	public:
		std::string name;
		std::set<std::string> connections;
	};
	static void BronKerbosch(std::set<std::string> R, std::set<std::string> P, std::set<std::string> X, const std::map<std::string, LanComputer>& connections, std::set<std::set<std::string>>& output);
};

