#include "Puzzle16.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>

const char c_BLANK = '.';
const char c_WALL = '#';
const char c_START = 'S';
const char c_GOAL = 'E';

std::vector<Coord> c_Directions = { Coord(0,-1), Coord(0,1), Coord(-1,0), Coord(1,0) };
//small hack, my solution is very slow (2.5min) and I don't like this question so I don't feel like optimising it or working on it further.
//I can calculate part Bs answer in part A, so I'm choosing to just save it to a static and return that so I don't have to run the full 2.5 min calculation twice.
//this question was no fun whatsoever, fight me if you think this is cheating, but i'd rather not rework my entire existing question system just to accomodate this stupid question and I certainly never want to modify it again.
static int64_t bestTilesCount = 0; 

//https://adventofcode.com/2024/day/16
//For this puzzle we have a maze, walls are # floors are . and S and E represent start and end.
//given this maze reindeer will race through it.
//Reindeer pathfinding is different to what you'd expect, turning costs 1000, while moving costs 1.
//based on this metric we need to find the length of the shortest path for part a
std::string Puzzle16::SolvePuzzleA(const char* fileName)
{
    std::ifstream myFile(fileName);
    std::vector<std::vector<char>> map = ParseGrid(myFile);

    PathNode origin;
    PathNode end;
    for (int x = 0; x < map[0].size(); x++)
    {
        for (int y = 0; y < map.size(); y++)
        {
            if (map[y][x] == c_START)
            {
                origin.position = Coord(x, y);
            }
            if (map[y][x] == c_GOAL)
            {
                end.position = Coord(x, y);
            }
        }
    }
    origin.distance = 0;

    std::vector<PathNode> front;
    std::map<Coord, PathNode> nodes;
    nodes[origin.position] = origin;
    front.push_back(origin);

    //calculate distances
    while (front.size() > 0)
    {
        PathNode currentNode = *(front.end() - 1);
        front.pop_back();

        if (map[currentNode.position.y][currentNode.position.x] == c_GOAL)
        {
            if (currentNode.position == end.position)
            {
                continue; //this path has terminated.
            }
        }

        int validDirectionCount = 0;

        for (const Coord& direction : c_Directions)
        {
            Coord offset = currentNode.position + direction;
            if (map[offset.y][offset.x] == c_BLANK || map[offset.y][offset.x] == c_GOAL)
            {
                validDirectionCount++;
            }
        }

        for (const Coord& direction : c_Directions)
        {
            Coord offset = currentNode.position + direction;
            if (map[offset.y][offset.x] == c_BLANK || map[offset.y][offset.x] == c_GOAL)
            {
                PathNode newPath;
                newPath.distance = currentNode.distance + 1;
                if (validDirectionCount > 2)
                    newPath.distance += 1001;
                newPath.position = offset;

                if (nodes[offset].distance > newPath.distance)
                {
                    nodes[offset] = newPath;
                    front.push_back(newPath);
                }
            }
        }
    }

    std::vector<std::vector<PathNode>> allPaths;
    allPaths.push_back(std::vector<PathNode>());
    int64_t lowestScore = INT64_MAX;
    //BuildPaths(allPaths, nodes, nodes[origin.position], 0, 0, origin.position, end.position);
    int64_t bestScore = INT64_MAX;
    std::map<Coord, int64_t> bestScores;
    BuildGoodPaths(allPaths, std::vector<PathNode>(), origin.position, end.position, bestScore, 0, map, bestScores);

    std::vector<std::vector<PathNode>> goodPaths;
    for (const std::vector<PathNode> path : allPaths)
    {
        int64_t score = ScorePath(path, end.position);
        if (score < lowestScore)
        {
            lowestScore = score;
        }
        if (score != INT64_MAX)
        {
            goodPaths.push_back(path);
        }
    }

    std::vector<std::vector<PathNode>> bestPaths;
    for (const std::vector<PathNode> path : goodPaths)
    {
        int64_t score = ScorePath(path, end.position);
        if (score == lowestScore)
        {
            bestPaths.push_back(path);

#if DEBUG_OUTPUT
            std::cout << "\n";
            //debug vis
            for (int y = 0; y < map.size(); y++)
            {
                for (int x = 0; x < map[0].size(); x++)
                {
                    Coord pos(x, y);
                    bool found = false;
                    for (const PathNode& node : path)
                    {
                        if (node.position == pos)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        std::cout << "O";
                    else
                        std::cout << map[y][x];
                }
                std::cout << "\n";
            }
            std::cout << "\n";
#endif

        }
    }

    std::set<Coord> bestTiles;
    for (const std::vector<PathNode> path : bestPaths)
    {
        for (int i = 0; i < path.size(); i++)
        {
            bestTiles.insert(path[i].position);
        }
        ScorePath(path, end.position, true);
    }
    bestTilesCount = bestTiles.size();

    return std::to_string(lowestScore);
}

//https://adventofcode.com/2024/day/16#part2
//part b is to find all 'best' paths, and return the number of tiles in the maze that are in at least one of those paths.
//I'll be honest, I hated this part of the puzzle and I don't want to revisit it with a polish pass, so this is what it is.
std::string Puzzle16::SolvePuzzleB(const char* fileName)
{
    return std::to_string(bestTilesCount);
}

std::vector<std::vector<char>> Puzzle16::ParseGrid(std::ifstream& file)
{
    std::vector<std::vector<char>> tiles;
    int width = 0;
    int height = 0;
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            if (width == 0)
            {
                width = line.size();
            }
            else
            {
                if (width != line.size())
                {
                    //done parsing grid
                    return tiles;
                }
            }

            height++;
            std::vector<char> charLine(line.begin(), line.end());
            tiles.push_back(charLine);
        }
    }
}


void Puzzle16::PrintCosts(std::map<Coord, PathNode> nodes, const std::vector<std::vector<char>>& map)
{
    std::cout << "\n";
    //debug vis
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[0].size(); x++)
        {
            Coord pos(x, y);
            bool found = false;
            if (nodes.contains(pos))
            {
                if (nodes[pos].distance % 100 < 10)
                {
                    std::cout << " " << nodes[pos].distance % 100;
                }
                else
                {
                    std::cout << nodes[pos].distance % 100;
                }
            }
            else
            {
                std::cout << map[y][x] << map[y][x];
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int64_t Puzzle16::ScorePath(const std::vector<PathNode>& path, const Coord& end, bool requireEnding /*= true*/)
{
    Coord direction(1, 0);
    int64_t score = 0;
    if (path.size() == 0 || (requireEnding && (path.end() - 1)->position != end))
        return INT64_MAX; //didn't path to end.

    //also we ignore i = 0 intentionally as we don't turn for the final step.
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        Coord diff = path[i + 1].position - path[i].position;
        if (diff != direction)
        {
            direction = diff;
            score += 1000;
        }
        score += 1;
    }
    return score;
}


//recursively put every possible path into paths
void Puzzle16::BuildPaths(std::vector<std::vector<PathNode>>& outPaths, std::map<Coord, PathNode>& nodes, PathNode& node, int path, int index, const Coord& start, const Coord& end)
{
    if (node.position == end)
    {
        return;
    }

    bool samePath = true;
    size_t pathIndex = path;
    bool deadEnd = true;
    for (const Coord& direction : c_Directions)
    {
        Coord neighbour = node.position + direction;
        if (nodes.contains(neighbour))
        {
            PathNode neighbourNode = nodes[neighbour];
            //only move away from start
            bool isLoop = false;
            for (const PathNode& node : outPaths[path])
            {
                if (node.position == neighbour)
                {
                    isLoop = true;
                    break;
                }
            }
            if (!isLoop)
            {
                if (!samePath)
                {

                    //split the path into two
                    std::vector<PathNode> pathCopy(outPaths[path].begin(), outPaths[path].begin() + index);
                    outPaths.push_back(pathCopy);
                    pathIndex = outPaths.size() - 1;
                }
                outPaths[pathIndex].push_back(neighbourNode);
                BuildPaths(outPaths, nodes, neighbourNode, pathIndex, index + 1, start, end);
                samePath = false;
                deadEnd = false;

                //horrible pruning hack
                if ((outPaths[pathIndex].end() - 1)->position != end)
                {
                    outPaths[pathIndex].erase(outPaths[pathIndex].begin() + index, outPaths[pathIndex].end());
                }
            }
        }
    }
}

static int counter = 0;
bool Puzzle16::BuildGoodPaths(std::vector<std::vector<Puzzle16::PathNode>>& outPaths, std::vector<Puzzle16::PathNode> previousNodes, Coord& node, const Coord& end, int64_t& bestScore, int64_t runningScore, const std::vector<std::vector<char>>& map, std::map<Coord, int64_t>& bestScores)
{
    PathNode current;
    current.position = node;
    previousNodes.push_back(current);

    //if we are the end point, return the node in a new vectorvector
    if (node == end)
    {
        int64_t score = ScorePath(previousNodes, end);
        if (score > bestScore)
        {
            return false;
        }
        bestScore = score;

        outPaths.push_back(previousNodes);

        return true;
    }

    ////cull paths that can't beat our pb
    //if (bestScore != INT64_MAX && runningScore > bestScore)
    //{
    //    return false;
    //}

    ////cull nodes that can't beat our pb
    if (bestScores.contains(node))
    {
        if (runningScore - 1000 > bestScores[node])
        {
            return false;
        }
        else
        {
            if (runningScore < bestScores[node])
                bestScores[node] = runningScore;
        }
    }
    else
    {
        bestScores[node] = runningScore;
    }

    bool deadEnd = true;
    for (const Coord& direction : c_Directions)
    {
        Coord neighbour = node + direction;
        if (map[neighbour.y][neighbour.x] != '#')
        //if (nodes.contains(neighbour))
        {
            Coord lastDirection(1,0);
            if (previousNodes.size() > 1)
            {
                //avoid immediate backtracking
                if ((previousNodes.end() - 2)->position == neighbour)
                    continue;

                //determine direction we are enterring the next node from
                lastDirection = node - (previousNodes.end() - 2)->position;
            }

            bool isLoop = false;
            for (int i = 0; i < previousNodes.size(); i++)
            {
                if (previousNodes[i].position == neighbour)
                {
                    isLoop = true;
                }
            }

            if (isLoop)
            {
                continue;
            }
            deadEnd = false;
            {
                if (lastDirection == direction || neighbour == end)
                {
                    BuildGoodPaths(outPaths, previousNodes, neighbour, end, bestScore, runningScore + 1, map, bestScores);
                }
                else
                {
                    BuildGoodPaths(outPaths, previousNodes, neighbour, end, bestScore, runningScore + 1001, map, bestScores);
                }
            }
        }
    }
    return deadEnd;
}



bool Puzzle16::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle16/test.txt");
    std::string resultB = SolvePuzzleB("puzzle16/test.txt");
    std::string resultA2 = SolvePuzzleA("puzzle16/test2.txt");
    std::string resultB2 = SolvePuzzleB("puzzle16/test2.txt");
    std::string resultA3 = SolvePuzzleA("puzzle16/test3.txt");
    std::string resultB3 = SolvePuzzleB("puzzle16/test3.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
    std::cout << "A: " << resultA2 << "\n";
    std::cout << "A: " << resultA3 << "\n";
    std::cout << "B: " << resultB << "\n";
    std::cout << "B: " << resultB2 << "\n";
    std::cout << "B: " << resultB3 << "\n";
#endif
    if (resultA != "7036") return false;
    if (resultA2 != "11048") return false;
    if (resultB != "45") return false;
    if (resultB2 != "64") return false;
    if (resultB3 != "14") return false;
    return true;
}