#include "Puzzle23.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <queue>

//https://adventofcode.com/2024/day/23
//given a huge list of PC names and how they are connected in format kh-tc
//where kh and tc are separate PCs and the - indicates a two way connection
//we are given the description of a huge computer network.
//Given this network we are required to find how many sets of 3 PCs that each connect to eachother and return their sum.
//but only if a pc in that triplet starts with the letter 't'.
std::string Puzzle23::SolvePuzzleA(const char* fileName)
{
    std::map<std::string, LanComputer> pcs;

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            //pcs are in format "xx-yy" where xx and yy are unique 2 character identifiers
            std::string firstPC = line.substr(0, 2);
            std::string secondPC = line.substr(3, 2);

            if (!pcs.contains(firstPC))
                pcs[firstPC].name = firstPC;
            pcs[firstPC].connections.insert(secondPC);

            if (!pcs.contains(secondPC))
                pcs[secondPC].name = secondPC;
            pcs[secondPC].connections.insert(firstPC);
        }
    }

    int64_t searchDepth = 2;
    std::set<std::string> connectedPCs; //sets of connected pcs in format xx-yy-zz

    for (auto& keyValue : pcs)
    {
        for (const std::string& neighbour : keyValue.second.connections)
        {
            for (const std::string& secondNeighbour : pcs[neighbour].connections)
            {
                if (pcs[secondNeighbour].connections.contains(keyValue.first))
                {
                    std::vector<std::string> pcNames;
                    pcNames.push_back(keyValue.first);
                    pcNames.push_back(neighbour);
                    pcNames.push_back(secondNeighbour);
                    std::sort(pcNames.begin(), pcNames.end());
                    connectedPCs.insert(pcNames[0] + "," + pcNames[1] + "," + pcNames[2]);
                }
            }
        }
    }

    int64_t candidateCount = 0;
    for (std::string connectionTrio : connectedPCs)
    {
        //looking for pcs that start with 't'
        if (connectionTrio[0] == 't' || connectionTrio[3] == 't' || connectionTrio[6] == 't')
        {
            candidateCount++;
        }
    }

    return std::to_string(candidateCount);
}

//https://adventofcode.com/2024/day/23#part2
//As part one, but this time we are looking for the largest group of pcs that are all connected to eachother directly.
//then we are to return a comma separated list of their names in alphabetical order, e.g. co,de,ka,ta
std::string Puzzle23::SolvePuzzleB(const char* fileName)
{
    std::map<std::string, LanComputer> pcConnections;
    std::set<std::string> pcNames;

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            //pcs are in format "xx-yy" where xx and yy are unique 2 character identifiers
            std::string firstPC = line.substr(0, 2);
            std::string secondPC = line.substr(3, 2);

            if (!pcConnections.contains(firstPC))
                pcConnections[firstPC].name = firstPC;
            pcConnections[firstPC].connections.insert(secondPC);

            if (!pcConnections.contains(secondPC))
                pcConnections[secondPC].name = secondPC;
            pcConnections[secondPC].connections.insert(firstPC);

            pcNames.insert(firstPC);
            pcNames.insert(secondPC);
        }
    }

    int64_t searchDepth = 2;

    std::set<std::set<std::string>> networks; //network blobs we have detected

    BronKerbosch(std::set<std::string>(), pcNames, std::set<std::string>(), pcConnections, networks);

    std::vector<std::string> networkIdentifiers;
    for (const std::set<std::string>& network : networks)
    {
        std::vector<std::string> pcNames;
        for (const std::string& pc : network)
        {
            pcNames.push_back(pc);
        }
        std::sort(pcNames.begin(), pcNames.end());
        std::string concatenatedNames;
        for (int i = 0; i < pcNames.size(); i++)
        {
            concatenatedNames.append(pcNames[i]);
            if (i != pcNames.size() - 1)
                concatenatedNames.push_back(',');
        }
        networkIdentifiers.push_back(concatenatedNames);
    }

    std::sort(networkIdentifiers.begin(), networkIdentifiers.end(), [] (const std::string& first, const std::string& second) 
        {
            return first.size() > second.size();
        });

    std::string output = networkIdentifiers[0];

    return output;
}

bool Puzzle23::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle23/test.txt");
    std::string resultB = SolvePuzzleB("puzzle23/test.txt");
    if (resultA != "7") return false;
    if (resultB != "co,de,ka,ta") return false;
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "B: " << resultB << "\n";
#endif
    return true;
}

//https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
//exhaustive list of all possible sets
//seems like there is no good algorithm for finding the maximum asside from finding all "cliques" and getting the largest
//https://en.wikipedia.org/wiki/Clique_problem lists some alternative solutions but I imagine they are more complex for small speed ups and Bron-Kerbosch algorithm 
//seems straightforward to implement.
//note: maximal cliques are simple interconnected chunks of the set, the largest possible maximal clique is referred to as the "maximum" clique. Thanks mathematicians. Great naming as usual.

//inputs:
//R = empty set initially (holds maximal cliques)
//P = vertex set of the graph
//X = empty set initially
//U = union
//{ v } new set of vertex?
//N(v) = neighbours of v
//⋃ union
//⋂ intersection
// P \ {v} P complement of v

//algorithm BronKerbosch1(R, P, X) is
void Puzzle23::BronKerbosch(std::set<std::string> R, std::set<std::string> P, std::set<std::string> X, const std::map<std::string, LanComputer>& connections, std::set<std::set<std::string>>& output)
{
    //if P and X are both empty then
    if (P.size() == 0 && X.size() == 0)
    {
        //report R as a maximal clique
        output.insert(R);

        static int64_t count = 0;
        count++;
        if (count % 1000 == 0)
            std::cout << ". " << "\n";
    }
    //for each vertex v in P do
    while (P.size() > 0)
    {
        const std::string& v = *P.begin();
        //BronKerbosch1(R ⋃{ v }, P ⋂ N(v), X ⋂ N(v))
        std::set<std::string> RunionV = R;
        RunionV.insert(v);

        std::set<std::string> PintersectNv;
        std::set<std::string> XintersectNv;
        for (const std::string& node : connections.at(v).connections)
        {
            if (P.contains(node))
                PintersectNv.insert(node);

            if (X.contains(node))
                XintersectNv.insert(node);
        }

        BronKerbosch(RunionV, PintersectNv, XintersectNv, connections, output);

        //X: = X ⋃{ v } note: flipped with next line because v gets deleted
        X.insert(v);

        //P : = P \ {v}
        P.erase(v);
    }
}