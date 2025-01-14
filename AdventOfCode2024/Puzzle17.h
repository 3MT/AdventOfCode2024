#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

class Puzzle17
{
public:
	class VirtualTriBitMachine
	{
	public:
		enum TriCode
		{
			ADV = 0,
			BXL = 1,
			BST = 2,
			JNZ = 3,
			BXC = 4,
			OUT = 5,
			BDV = 6,
			CDV = 7,
		};
		VirtualTriBitMachine()
		{
			Reset();
		}

		void Reset()
		{
			registerA = 0;
			registerB = 0;
			registerC = 0;
			instructionPointer = 0;
			program.clear();
			virtualCOut.str("");
			firstWrite = true;
		}

		bool ProcessInstruction(); //returns false to indicate program has HALTed.

		void LoadProgram(std::string input);

		void SetRegister(int64_t index, int64_t value);

		int64_t GetComboValue(int64_t operand);

		std::stringstream& GetCOut() { return virtualCOut; };
		std::string DebugDumpProgram();

		bool RunTests();

	private:
		int64_t registerA;
		int64_t registerB;
		int64_t registerC;

		int64_t instructionPointer;
		
		std::vector<TriCode> program;

		std::stringstream virtualCOut;
		bool firstWrite;
	};
	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static std::string InstructionName(VirtualTriBitMachine::TriCode instruction);
};

