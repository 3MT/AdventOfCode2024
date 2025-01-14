#include "Puzzle17.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>

//https://adventofcode.com/2024/day/17
//for this question we have to create a simulated CPU with a set of instructions.
//normally I would describe it in this comment, but the CPU does have a lot going on, so I refer you to the link above for details on the CPU spec.
//part A's task is to set up a CPU with the instructions, and feed it a program passed in.
//we then run the program until it halts (thankfully it's guaranteed to halt) and output the string of integers, comma separated that it output.
std::string Puzzle17::SolvePuzzleA(const char* fileName)
{
    VirtualTriBitMachine virtualMachine;

    std::fstream file(fileName);
    std::string output;
    if (file.is_open())
    {
        std::string line;
        int lineCount = 0;
        while (getline(file, line))
        {
            //expected file structure:
            //Register A: 729
            //Register B: 0
            //Register C: 0

            //Program: 0, 1, 5, 4, 3, 0
            std::string registerName = "Register A:";
            std::string programName = "Program:";

            switch (lineCount)
            {
            case 0:
                virtualMachine.SetRegister(0, std::stoll(line.substr(registerName.size())));
                break;
            case 1:
                virtualMachine.SetRegister(1, std::stoll(line.substr(registerName.size())));
                break;
            case 2:
                virtualMachine.SetRegister(2, std::stoll(line.substr(registerName.size())));
                break;
            case 4:
                virtualMachine.LoadProgram(line.substr(programName.size()));
            }
            lineCount++;
        }
    }

    while (virtualMachine.ProcessInstruction())
    {
        //...
    }

#if DEBUG_OUTPUT
    std::cout << virtualMachine.GetCOut().str();
#endif

	return virtualMachine.GetCOut().str();
}

std::string Puzzle17::SolvePuzzleB(const char* fileName)
{
    int64_t registerA = 0;
    int64_t registerB = 0;
    int64_t registerC = 0;
    std::fstream file(fileName);
    std::string output;
    std::string program;
    if (file.is_open())
    {
        std::string line;
        int lineCount = 0;
        while (getline(file, line))
        {
            //expected file structure:
            //Register A: 729
            //Register B: 0
            //Register C: 0

            //Program: 0, 1, 5, 4, 3, 0
            std::string registerName = "Register A: ";
            std::string programName = "Program: ";

            switch (lineCount)
            {
            case 0:
                //virtualMachine.SetRegister(0, std::stoll(line.substr(registerName.size())));
                break;
            case 1:
                registerB =std::stoll(line.substr(registerName.size()));
                break;
            case 2:
                registerC = std::stoll(line.substr(registerName.size()));
                break;
            case 4:
                program = line.substr(programName.size());
            }
            lineCount++;
        }
    }

    bool foundMatch = false;
    //registerA = pow(8, 16); //rough number of digits.

    
    /* this code was an attempt to reverse engineer the program.
    * I ended up understanding what it did, but unfortunately couldn't come up with a simple way to reverse it's behaviour.
    * It is essentially taking the registers A B and C and repeatedly mutating them, similar to the monkey banana secret question.
    * The while loop below is roughly what the program is doing, but like I said I was sadly unable to devise a way to do the operations backwards.
    {
        std::vector<int64_t> crack;
        for (size_t i = 0; i < program.size(); i++)
        {
            if (program[i] >= '0' && program[i] <= '9')
            {
                crack.push_back((program[i] - '0')); //char to int conversion, then convert to 'tricode'
            }
        }

        std::reverse(crack.begin(), crack.end());

        //BXL : 5 = 5
        //CDV : 5 = A / B
        //BXL : 6
        //ADV : 3
        //BXC : 0
        //OUT : 5
        //JNZ : 0

        int64_t a = 1;
        int64_t b = 0;
        int64_t c = 0;
        int64_t remainder;
        while (crack.size() != 0)
        {
            int64_t b = crack[0];
            int64_t next = (crack.size() > 1 ? crack[1] : 0);

            crack.pop_back();
            //BXC : 0
            b = b ^ c;

            //ADV : 3
            a = (a * 8) + (a % 8);
            
            //BXL : 6
            b = b ^ 6;

            //CDV : 5 = A / B
            c = a * b;

            //BXL : 5 = 5
            int64_t expectedB = next ^ 5;
            remainder = expectedB;
            a += remainder;
        }
        std::cout << a;
    }
    */
    
    registerA = 0;

    while (!foundMatch)
    {
        VirtualTriBitMachine machine;


        machine.SetRegister(0, registerA);
        machine.SetRegister(1, registerB);
        machine.SetRegister(2, registerC);
        machine.LoadProgram(program);

        int timeout = 0;
        while (machine.ProcessInstruction())
        {
            timeout++;
            if (timeout > 1000)
                break;
            //...
        }

        std::string output = machine.GetCOut().str();

#if DEBUG_OUTPUT
        std::cout << output << "\n";
#endif
        //essentially an exhaustive search, the output is not necessarily linear, but similar outputs are grouped together
        //so I figured the best way to do it was to search for more and more matching digits, then make our search slower and more granular as we approach the correct answer.
        if (output.size() == program.size())
        {
            int sharedDigits = 0;
            for (int i = output.size() - 1; i >= 0; i--)
            {
                if (output[i] != ',')
                {
                    if (output[i] == program[i])
                    {
                        sharedDigits++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            sharedDigits = sharedDigits;
            if (sharedDigits == 16)
            {
                break;
            }
            registerA += std::max((int64_t)pow(10, std::max(11 - sharedDigits, 0)),1ll); //this portion is our searches level of detail, as we get more digits correct we slow down until eventually we are checking every integer.
        }
        else
        {
            registerA += 100000000000;
        }
    }

	return std::to_string(registerA);
}


bool Puzzle17::RunTests()
{
    //my test cases for the emulator.
    VirtualTriBitMachine testMachine;
    if (!testMachine.RunTests()) return false;

    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle17/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "4,6,3,5,6,3,5,2,1,0") return false;
    return true;
}

bool Puzzle17::VirtualTriBitMachine::ProcessInstruction()
{
    if (instructionPointer >= program.size())
    {
        //std::cout << "Program Complete\n";
        return false;
    }

    TriCode instruction = program[instructionPointer];
    int64_t operand = program[instructionPointer+1];

    bool debugOutput = false;
    if (debugOutput)
    {
        std::cout << InstructionName(instruction) << ":" << instruction << "\n  L[" << operand << "]\n\n";
    }
    switch (instruction)
    {
        case ADV: 
        {
            int64_t numerator = registerA;
            int64_t comboOperand = GetComboValue(operand);
            if (comboOperand == -1)
            {
                return false;
            }
            int64_t denominator = pow(2, comboOperand);

            registerA = numerator / denominator; //int truncation intentional.
            instructionPointer += 2;
            break; 
        }
        case BXL: 
        { 
            registerB = registerB ^ operand;
            instructionPointer += 2;
            break; 
        }
        case BST: 
        {
            int64_t comboOperand = GetComboValue(operand);
            if (comboOperand == -1)
            {
                return false;
            }
            registerB = comboOperand % 8;
            instructionPointer += 2;
            break; 
        }
        case JNZ: 
        {
            if (registerA != 0)
            {
                instructionPointer = operand;
            }
            else
            {
                instructionPointer += 2;
            }
            break; 
        }
        case BXC: 
        {
            registerB = registerB ^ registerC;
            //should 'read' operand, if reading an operand ever gets a side effect?
            operand;
            instructionPointer += 2;
            break; 
        }
        case OUT: 
        { 
            if (!firstWrite)
                virtualCOut << ',';

            firstWrite = false;
            int64_t comboOperand = GetComboValue(operand);
            if (comboOperand == -1)
            {
                return false;
            }
            virtualCOut << (comboOperand % 8);
            instructionPointer += 2;
            break; 
        }
        case BDV: 
        {             
            int64_t numerator = registerA;
            int64_t comboOperand = GetComboValue(operand);
            if (comboOperand == -1)
            {
                return false;
            }
            int64_t denominator = pow(2, comboOperand);
            registerB = numerator / denominator; //int truncation intentional.
            instructionPointer += 2;
            break; 
        }
        case CDV:
        {
            int64_t numerator = registerA;
            int64_t comboOperand = GetComboValue(operand);
            if (comboOperand == -1)
            {
                return false;
            }
            int64_t denominator = pow(2, comboOperand);
            registerC = numerator / denominator; //int truncation intentional.
            instructionPointer += 2;
            break;
        }
        default: { break; }
    }
	return true;
}

int64_t Puzzle17::VirtualTriBitMachine::GetComboValue(int64_t operand)
{
    int64_t value = 0;
    switch (operand)
    {
    case 0: { value = 0; break; }
    case 1: { value = 1; break; }
    case 2: { value = 2; break; }
    case 3: { value = 3; break; }
    case 4: { value = registerA; break; }
    case 5: { value = registerB; break; }
    case 6: { value = registerC; break; }
    case 7: { std::cout << "Reserved Operand Error\n"; return -1; break; }
    default: { std::cout << "Invalid Operand Error\n"; return -1; break; }
    }
    return value;
}

std::string Puzzle17::VirtualTriBitMachine::DebugDumpProgram()
{
    std::string output;
    for (int i = 0; i < program.size(); i+=2)
    {
        std::cout << InstructionName(program[i]) << " : " << program[i + 1] << "\n";
    }
    return output;
}

bool Puzzle17::VirtualTriBitMachine::RunTests()
{
    Reset();
    {
        //If register C contains 9, the program 2,6 would set register B to 1.
        registerC = 9;
        LoadProgram("2,6");
        while (ProcessInstruction()) { }
        assert(registerB == 1);
    }
    Reset();
    {
        //If register A contains 10, the program 5,0,5,1,5,4 would output 0,1,2.
        registerA = 10;
        LoadProgram("5,0,5,1,5,4");
        while (ProcessInstruction()) {}
        assert(virtualCOut.str()._Equal("0,1,2"));
    }
    Reset();
    {
        //If register A contains 2024, the program 0,1,5,4,3,0 would output 4,2,5,6,7,7,7,7,3,1,0 and leave 0 in register A.
        registerA = 2024;
        LoadProgram("0,1,5,4,3,0");
        while (ProcessInstruction()) {}
        assert(virtualCOut.str()._Equal("4,2,5,6,7,7,7,7,3,1,0"));
        assert(registerA == 0);
    }
    Reset();
    {
        //If register B contains 29, the program 1,7 would set register B to 26.
        registerB = 29;
        LoadProgram("1,7");
        while (ProcessInstruction()) {}
        assert(registerB == 26);
    }
    Reset();
    {
        //If register B contains 2024 and register C contains 43690, the program 4,0 would set register B to 44354.
        registerB = 2024;
        registerC = 43690;
        LoadProgram("4,0");
        while (ProcessInstruction()) {}
        assert(registerB == 44354);
    }
    Reset();
    {
        //If register B contains 16 and register C contains 32 and register A contains 2, the program 6,1,7,2 would set register B to 16 and register C to 8.
        registerA = 32;
        LoadProgram("6,1,7,2");
        while (ProcessInstruction()) {}
        assert(registerB == 16);
        assert(registerC == 8);
    }
    return true;
}

std::string Puzzle17::InstructionName(VirtualTriBitMachine::TriCode instruction)
{
    switch (instruction)
    {
    case VirtualTriBitMachine::TriCode::ADV: { return "ADV"; }
    case VirtualTriBitMachine::TriCode::BXL: { return "BXL"; }
    case VirtualTriBitMachine::TriCode::BST: { return "BST"; }
    case VirtualTriBitMachine::TriCode::JNZ: { return "JNZ"; }
    case VirtualTriBitMachine::TriCode::BXC: { return "BXC"; }
    case VirtualTriBitMachine::TriCode::OUT: { return "OUT"; }
    case VirtualTriBitMachine::TriCode::BDV: { return "BFV"; }
    case VirtualTriBitMachine::TriCode::CDV: { return "CDV"; }
    default: { return "ERR"; }
    }
}

void Puzzle17::VirtualTriBitMachine::LoadProgram(std::string input)
{
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] >= '0' && input[i] <= '9')
        {
            program.push_back((TriCode)(input[i] - '0')); //char to int conversion, then convert to 'tricode'
        }
    }
}

void Puzzle17::VirtualTriBitMachine::SetRegister(int64_t index, int64_t value)
{
    switch (index)
    {
    case 0:
        registerA = value;
        break;
    case 1:
        registerB = value;
        break;
    case 2:
        registerC = value;
        break;
    default:
        break;
    }
}
