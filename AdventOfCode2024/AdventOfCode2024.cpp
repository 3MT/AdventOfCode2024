#include <iostream>

#include "Puzzle1.h"
#include "Puzzle2.h"
#include "Puzzle3.h"
#include "Puzzle4.h"
#include "Puzzle5.h"
#include "Puzzle6.h"
#include "Puzzle7.h"
#include "Puzzle8.h"
#include "Puzzle9.h"
#include "Puzzle10.h"
#include "Puzzle11.h"
#include "Puzzle12.h"
#include "Puzzle13.h"
#include "Puzzle14.h"
#include "Puzzle15.h"
#include "Puzzle16.h"
#include "Puzzle17.h"
#include "Puzzle18.h"
#include "Puzzle19.h"
#include "Puzzle20.h"
#include "Puzzle21.h"
#include "Puzzle22.h"
#include "Puzzle23.h"
#include "Puzzle24.h"
#include "Puzzle25.h"

#include <chrono>
#include <format>

//#define DEBUG_OUTPUT 1
//#define RUN_TESTS 1
//#define WAIT_AFTER_PUZZLE 1

//see individual puzzle classes for problem breakdown etc.
//main is just here to display results and set up input etc.

//each question in aoc supplies a test data with a known solution and input data with a large, unknown solution for you to solve.
//these are stored in the project directory with the convention puzzleXX/test.txt and puzzleXX/input.txt
//data format differs by puzzle

using time_point = std::chrono::high_resolution_clock::time_point;

template <class T> void RunPuzzle(int64_t puzzleID, std::string expectedAResult, std::string expectedBResult)
{
    std::cout << "Puzzle " << puzzleID << "\n";
    time_point start, end;

#ifdef RUN_TESTS
    {
        if (T::RunTests())
        {
            std::cout << " Tests passed!\n";
        }
        else
        {
            std::cout << " Tests failed!\n";
        }
    }
#endif

    std::cout << " Part A\n";
    std::string realFile = std::format("puzzle{:02}/input.txt", puzzleID);
    start = std::chrono::high_resolution_clock::now();
    std::string puzzleResultA = T::SolvePuzzleA(realFile.c_str());
    end = std::chrono::high_resolution_clock::now();
    std::cout << "   answer:   " << puzzleResultA << "\n";
    std::cout << "   expected: " << expectedAResult << "\n";
    std::cout << "   Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";

    std::cout << " Part B\n";
    start = std::chrono::high_resolution_clock::now();
    std::string puzzleResultB = T::SolvePuzzleB(realFile.c_str());
    end = std::chrono::high_resolution_clock::now();
    std::cout << "   answer:   " << puzzleResultB << "\n";
    std::cout << "   expected: " << expectedBResult << "\n";
    std::cout << "   Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
    std::cout << std::endl;

#if WAIT_AFTER_PUZZLE
    system("pause");
    system("cls");
#endif
}

int main()
{
    time_point start, end;
    start = std::chrono::high_resolution_clock::now();
    //note: the expected result arguments were determined by solving the puzzle initially, I have hardcoded them after solving the puzzle
    //so I can make sure any changes I make to solutions don't break the solution later ;)
    RunPuzzle<Puzzle1>(1, "1319616", "27267728");
    RunPuzzle<Puzzle2>(2, "463", "514");
    RunPuzzle<Puzzle3>(3, "161085926", "82045421");
    RunPuzzle<Puzzle4>(4, "2569", "1998");
    RunPuzzle<Puzzle5>(5, "5087", "4971");
    RunPuzzle<Puzzle6>(6, "4967", "1789");
    RunPuzzle<Puzzle7>(7, "465126289353", "70597497486371");
    RunPuzzle<Puzzle8>(8, "423", "1287");
    RunPuzzle<Puzzle9>(9, "6201130364722", "6221662795602");
    RunPuzzle<Puzzle10>(10, "811", "1794");
    RunPuzzle<Puzzle11>(11, "197157", "234430066982597");
    RunPuzzle<Puzzle12>(12, "1456082", "872382");
    RunPuzzle<Puzzle13>(13, "36954", "79352015273424");
    RunPuzzle<Puzzle14>(14, "229421808", "6577");
    RunPuzzle<Puzzle15>(15, "1490942", "1519202");
    RunPuzzle<Puzzle16>(16, "123540", "665");
    RunPuzzle<Puzzle17>(17, "7,3,1,3,6,3,6,0,2", "105843716614554");
    RunPuzzle<Puzzle18>(18, "324", "46,23");
    RunPuzzle<Puzzle19>(19, "276", "681226908011510");
    RunPuzzle<Puzzle20>(20, "1518", "1032257");
    RunPuzzle<Puzzle21>(21, "171596", "209268004868246");
    RunPuzzle<Puzzle22>(22, "15303617151", "1727"); 
    RunPuzzle<Puzzle23>(23, "1248", "aa,cf,cj,cv,dr,gj,iu,jh,oy,qr,xr,xy,zb");
    RunPuzzle<Puzzle24>(24, "51107420031718", "cpm,ghp,gpr,krs,nks,z10,z21,z33");
    RunPuzzle<Puzzle25>(25, "3439", "Merry Christmas!");
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms TOTAL!\n";
    system("pause");
    return 0;
}

