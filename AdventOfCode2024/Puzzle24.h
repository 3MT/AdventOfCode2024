#pragma once
#include <cstdint>
#include <string>
#include <map>

class Puzzle24
{
public:
	struct LogicGate
	{
	public:
		enum WireState
		{
			False = 0,
			True = 1,
			Unresolved = -1,
		};
		enum GateType : char
		{
			XOR = '^',
			OR = '|',
			AND = '&',
			INPUT = '>'
		};
		std::string leftParent;
		std::string rightParent;
		WireState state;
		GateType type;
		std::string name;
		std::string ParseLogicGate(const std::string& parseLine);
		WireState ResolveLogicGate(std::map<std::string, LogicGate>& otherGates);
		int64_t CountParents(std::map<std::string, LogicGate>& otherGates);
		int64_t CountANDParents(std::map<std::string, LogicGate>& otherGates);
		int64_t CountXORParents(std::map<std::string, LogicGate>& otherGates);
		int64_t CountORParents(std::map<std::string, LogicGate>& otherGates);
		int64_t CountINPParents(std::map<std::string, LogicGate>& otherGates);
	};
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static void DebugPrintGateStates(std::map<std::string, LogicGate>& gates);
};

