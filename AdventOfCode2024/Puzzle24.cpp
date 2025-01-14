#include "Puzzle24.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <vector>

//https://adventofcode.com/2024/day/24
//we are given a series of logic gates in the format
//x00 AND y00 -> z00
//z00 would be our output, x00 and y00 are our inputs and the text between is the operator.
//our job is to evaluate the list of logic gates and see what the z gates all output, converted into an integer by writing each z gate into a binary position of an int.
//we are also given the initial values for the x and y prefixed gates.
std::string Puzzle24::SolvePuzzleA(const char* fileName)
{
    std::map<std::string, LogicGate> gates;
    std::vector<std::string> zGates; //these are the gates we query to answer the question.

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            if (line.size() > 0)
            {
                LogicGate gate;
                std::string name = gate.ParseLogicGate(line);
                gates[name] = gate;
                if (name[0] == 'z')
                {
                    zGates.push_back(name);
                }
            }
        }
    }
    
    std::sort(zGates.begin(), zGates.end());

    int64_t output = 0;
    for (int64_t i = 0; i < zGates.size(); i++)
    {
        const std::string& zGate = zGates[i];
        if (gates[zGate].ResolveLogicGate(gates) == LogicGate::True)
        {
            output += 1ll << i;
        }
    }

#if DEBUG_OUTPUT
    DebugPrintGateStates(gates);
#endif

    return std::to_string(output);
}

void Puzzle24::DebugPrintGateStates(std::map<std::string, LogicGate>& gates)
{
    for (auto& keyValue : gates)
    {
        LogicGate::WireState leftState = LogicGate::Unresolved;
        if (gates.contains(keyValue.second.leftParent))
            leftState = gates[keyValue.second.leftParent].state;
        LogicGate::WireState rightState = LogicGate::Unresolved;
        if (gates.contains(keyValue.second.leftParent))
        rightState = gates[keyValue.second.rightParent].state;
        std::string logicType = "";

        switch (keyValue.second.type)
        {
        case LogicGate::XOR:
            logicType = " XOR ";
            break;
        case LogicGate::OR:
            logicType = "  OR ";
            break;
        case LogicGate::AND:
            logicType = " AND ";
            break;
        default:
            logicType = " INP ";
            break;
        }
        std::cout << keyValue.first << ": " << keyValue.second.leftParent << leftState << logicType << keyValue.second.rightParent << rightState << " = " << keyValue.second.state << "\n";
    }

}

//https://adventofcode.com/2024/day/24#part2
//Part b is very interesting. I didn't write an algorithm to solve this one, instead I solved it by hand.
//It is revealed that part A was a corrupted binary addition circuit. Our job for this task is to find how it is corrupted.
//We know for a fact that 8 gates are wrong, there have been exactly 4 swaps, and it has broken the circuit.
//As I said I didn't find an algorithm to solve this, instead I had to de-obfuscate the in between gates, figure out how they worked
//and debug the issue. Some cases were simple, in a binary adder the output gates are always XOR, so any that didn't fit that case were easy to find.
//The last pair of gates were harder, it took a lot of debugging in a variety of ways, inspecting how the circuit resolved, drawing diagrams, looking up binary addition circuits etc.
//I can't really put it into a short comment but just wanted to give an idea of the strategies I was looking into. I have hardcoded the answer as a result, but I've left in place
//some of the debugging and fiddling code I wrote to try and diagnose the answer, I won't document it heavily since it was mostly dirty code to rename gates, move stuff around and otherwise
//fiddle, but I'm leaving it here for posterity anyway.

std::string Puzzle24::SolvePuzzleB(const char* fileName)
{
    return "cpm,ghp,gpr,krs,nks,z10,z21,z33";
    std::map<std::string, LogicGate> gates;
    std::vector<std::string> xGates; //these are the gates we query to answer the question.
    std::vector<std::string> yGates; //these are the gates we query to answer the question.
    std::vector<std::string> zGates; //these are the gates we query to answer the question.

    std::map<std::string, std::string> swaps;
    swaps["z10"] = "gpr";
    swaps["z21"] = "nks";
    swaps["z33"] = "ghp";

    swaps["gpr"] = "z10";
    swaps["nks"] = "z21";
    swaps["ghp"] = "z33";

    swaps["cpm"] = "krs";
    swaps["krs"] = "cpm";

    //renames to help understand the shape better
    std::map<std::string, std::string> renames;
    /*
    //swaps[""] = "a00"; //z00 special case!
    renames["cdh"] = "a01";
    renames["ctr"] = "a02";
    renames["bnn"] = "a03";
    renames["nwv"] = "a04";
    renames["bjh"] = "a05";
    renames["rqp"] = "a06";
    renames["jsg"] = "a07";
    renames["rtn"] = "a08";
    renames["gdp"] = "a09";
    renames["htv"] = "a10";
    renames["tdq"] = "a11";
    renames["kpf"] = "a12";
    renames["bcs"] = "a13";
    renames["dqw"] = "a14";
    renames["cpp"] = "a15";
    renames["fjt"] = "a16";
    renames["rcn"] = "a17";
    renames["tsj"] = "a18";
    renames["wpw"] = "a19";
    renames["tff"] = "a20";
    renames["ptd"] = "a21";
    renames["hrr"] = "a22";
    renames["sfc"] = "a23";
    renames["wfc"] = "a24";
    renames["sqt"] = "a25";
    renames["mrq"] = "a26";
    renames["bqv"] = "a27";
    renames["wmr"] = "a28";
    renames["dnb"] = "a29";
    renames["kwq"] = "a30";
    renames["ggv"] = "a31";
    renames["qnj"] = "a32";
    renames["tkq"] = "a33";
    renames["hbg"] = "a34";
    renames["wgw"] = "a35";
    renames["skt"] = "a36";
    renames["srf"] = "a37";
    renames["vmv"] = "a38";
    renames["cpm"] = "a39";
    renames["vvm"] = "a40";
    renames["cmn"] = "a41";
    renames["pcg"] = "a42";
    renames["ptf"] = "a43";
    renames["wdg"] = "a44";

    
    renames["hsv"] = "c01";
    renames["jhh"] = "c02";
    renames["qtp"] = "c03";
    renames["brh"] = "c04";
    renames["bwn"] = "c05";
    renames["vdc"] = "c06";
    renames["gmq"] = "c07";
    renames["hnp"] = "c08";
    renames["vjj"] = "c09";
    renames["fsf"] = "c10";
    renames["tjg"] = "c11";
    renames["qsh"] = "c12";
    renames["tfc"] = "c13";
    renames["vrc"] = "c14";
    renames["smr"] = "c15";
    renames["tvk"] = "c16";
    renames["dwh"] = "c17";
    renames["drt"] = "c18";
    renames["fgk"] = "c19";
    renames["gjs"] = "c20";
    renames["nks"] = "c21";
    renames["jvd"] = "c22";
    renames["rtc"] = "c23";
    renames["dpm"] = "c24";
    renames["brs"] = "c25";
    renames["nbs"] = "c26";
    renames["gnb"] = "c27";
    renames["mwr"] = "c28";
    renames["nkw"] = "c29";
    renames["fpk"] = "c30";
    renames["bws"] = "c31";
    renames["sdj"] = "c32";
    renames["trf"] = "c33";
    renames["khh"] = "c34";
    renames["nvr"] = "c35";
    renames["ppq"] = "c36";
    renames["qmp"] = "c37";
    renames["sqw"] = "c38";
    //["krb"]	{leftParent="mhr" rightParent="krs" // HMM
    //renames[""] = "c39";
    renames["gbn"] = "c40";
    renames["qkb"] = "c41";
    renames["qdc"] = "c42";
    renames["frb"] = "c43";
    renames["ndp"] = "c44";
        
    renames["bwd"] = "d00";
    renames["knd"] = "d01";
    renames["hsk"] = "d02";
    renames["ngv"] = "d03";
    renames["ktv"] = "d04";
    renames["tqr"] = "d05";
    renames["mnv"] = "d06";
    renames["wgj"] = "d07";
    renames["mbt"] = "d08";
    renames["tnd"] = "d09";
    renames["gpr"] = "d10";
    renames["gpg"] = "d11";
    renames["drb"] = "d12";
    renames["drw"] = "d13";
    renames["mgp"] = "d14";
    renames["fbc"] = "d15";
    renames["tqn"] = "d16";
    renames["dbf"] = "d17";
    renames["qqm"] = "d18";
    renames["dws"] = "d19";
    renames["dtg"] = "d20";
    renames["rhk"] = "d21";
    renames["bbr"] = "d22";
    renames["hhg"] = "d23";
    renames["hkn"] = "d24";
    renames["rpg"] = "d25";
    renames["ctw"] = "d26";
    renames["dpd"] = "d27";
    renames["ngk"] = "d28";
    renames["qrw"] = "d29";
    renames["kdb"] = "d30";
    renames["fhd"] = "d31";
    renames["fnw"] = "d32";
    renames["jtg"] = "d33";
    renames["hwr"] = "d34";
    renames["srq"] = "d35";
    renames["mkh"] = "d36";
    renames["bkw"] = "d37";
    renames["mph"] = "d38";
    renames["krs"] = "d39";
    renames["qcm"] = "d40";
    renames["sbj"] = "d41";
    renames["gnm"] = "d42";
    renames["wpg"] = "d43";
    renames["wtc"] = "d44";

    renames["pdq"] = "e01";
    renames["jrj"] = "e02";
    renames["sdf"] = "e03";
    renames["wcr"] = "e04";
    renames["vdv"] = "e05";
    renames["tfg"] = "e06";
    renames["twg"] = "e07";
    renames["bgn"] = "e08";
    renames["whd"] = "e09";
    renames["tdj"] = "e10";
    renames["jjs"] = "e11";
    renames["vjt"] = "e12";
    renames["jmk"] = "e13";
    renames["fkn"] = "e14";
    renames["nwk"] = "e15";
    renames["nsj"] = "e16";
    renames["fnb"] = "e17";
    renames["nwb"] = "e18";
    renames["qfd"] = "e19";
    renames["scj"] = "e20";
    renames["hhc"] = "e21";
    renames["nkp"] = "e22";
    renames["bvg"] = "e23";
    renames["hnd"] = "e24";
    renames["pcf"] = "e25";
    renames["kkb"] = "e26";
    renames["shk"] = "e27";
    renames["mgr"] = "e28";
    renames["vhs"] = "e29";
    renames["dfm"] = "e30";
    renames["nwt"] = "e31";
    renames["mvc"] = "e32";
    renames["ghp"] = "e33";
    renames["rtj"] = "e34";
    renames["prt"] = "e35";
    renames["wdr"] = "e36";
    renames["gft"] = "e37";
    renames["mhr"] = "e38";
    renames["jhd"] = "e39";
    renames["dcd"] = "e40";
    renames["sdk"] = "e41";
    renames["qgm"] = "e42";
    renames["ftb"] = "e43";
    //renames[""] = "e44";
    //special case ["z45"] {leftParent = "wtc" rightParent = "c44"
    */

    std::ifstream myFile(fileName);
    if (myFile.is_open())
    {
        std::string line;
        while (getline(myFile, line))
        {
            if (line.size() > 0)
            {
                LogicGate gate;
                std::string name = gate.ParseLogicGate(line);

                //swaps
                if (swaps.contains(name))
                {
                    name = swaps[name];
                    gate.name = name;
                }

                if (renames.contains(name))
                {
                    name = renames[name];
                    gate.name = name;
                }
                if (renames.contains(gate.leftParent))
                {
                    gate.leftParent = renames[gate.leftParent];
                }
                if (renames.contains(gate.rightParent))
                {
                    gate.rightParent = renames[gate.rightParent];
                }

                if (name[0] == 'z')
                {
                    zGates.push_back(name);
                }
                if (name[0] == 'x')
                {
                    xGates.push_back(name);
                    gate.state = LogicGate::True;
                    if (name[2] == '4' && name[1] == '4')
                    {
                        gate.state = LogicGate::False;
                    }
                }
                if (name[0] == 'y')
                {
                    yGates.push_back(name);
                }
                if (gate.type == LogicGate::INPUT)
                {

                }
                gates[name] = gate;
            }
        }
    }

    std::sort(zGates.begin(), zGates.end());

    /*
    for (int64_t i = 0; i < zGates.size(); i++)
    {
        const std::string& zGate = zGates[i];
        int64_t xorcount = gates[zGate].CountXORParents(gates);
        int64_t orcount = gates[zGate].CountORParents(gates);
        int64_t andcount = gates[zGate].CountANDParents(gates);
        int64_t inpcount = gates[zGate].CountINPParents(gates);
        std::cout << zGate << ": " << xorcount << ", " << orcount << ", " << andcount << ", " << inpcount << "\n";
        //output += count;
    }*/

    std::map<std::string, LogicGate> debug_aGates;
    std::map<std::string, LogicGate> debug_zGates;
    std::map<std::string, LogicGate> debug_j1Gates;
    std::map<std::string, LogicGate> debug_j2Gates;
    std::map<std::string, LogicGate> debug_eGates;
    std::map<std::string, LogicGate> debug_inputs;
    std::map<std::string, LogicGate> debug_oddInputs;
    for (auto& keyValue : gates)
    {
        switch (keyValue.second.type)
        {
            case LogicGate::XOR:
                if (keyValue.second.leftParent[0] == 'x' || keyValue.second.leftParent[0] == 'y')
                {
                    debug_aGates[keyValue.first] = keyValue.second;
                }
                else
                {
                    debug_zGates[keyValue.first] = keyValue.second;
                }
                break;
            case LogicGate::OR:
                debug_eGates[keyValue.first] = keyValue.second;
                break;
            case LogicGate::AND:
                if (keyValue.second.leftParent[0] == 'x' || keyValue.second.leftParent[0] == 'y')
                {
                    debug_j2Gates[keyValue.first] = keyValue.second;
                }
                else
                {
                    debug_j1Gates[keyValue.first] = keyValue.second;
                }
                break;
            case LogicGate::INPUT:
                debug_inputs[keyValue.first] = keyValue.second;
                break;
            default:
                debug_oddInputs[keyValue.first] = keyValue.second;
                break;
        }
    }

    int64_t output = 0;
    int64_t inputx = 0;
    int64_t inputy = 0;
    for (int64_t i = 0; i < zGates.size(); i++)
    {
        const std::string& zGate = zGates[i];
        if (gates[zGate].ResolveLogicGate(gates) == LogicGate::True)
        {
            output += 1ll << i;
        }

        LogicGate leftParent = gates[gates[zGate].leftParent];
        LogicGate rightParent = gates[gates[zGate].rightParent];

        std::cout << leftParent.type << " " << zGate << "\n";
        std::cout << rightParent.type << " " << zGate << "\n";
    }
    for (int64_t i = 0; i < xGates.size(); i++)
    {
        const std::string& xGate = xGates[i];
        if (gates[xGate].ResolveLogicGate(gates) == LogicGate::True)
        {
            inputx += 1ll << i;
        }
    }
    for (int64_t i = 0; i < yGates.size(); i++)
    {
        const std::string& yGate = yGates[i];
        if (gates[yGate].ResolveLogicGate(gates) == LogicGate::True)
        {
            inputy += 1ll << i;
        }
    }

    int64_t expectedOutput = inputx + inputy;
    int64_t error = expectedOutput ^ output;


    return std::to_string(output);
}

bool Puzzle24::RunTests()
{
    //test case answers provided by question
    std::string resultA = SolvePuzzleA("puzzle24/test.txt");
#if DEBUG_OUTPUT
    std::cout << "A: " << resultA << "\n";
#endif
    if (resultA != "2024") return false;
    return true;
}

std::string Puzzle24::LogicGate::ParseLogicGate(const std::string& parseLine)
{
    //Format:
    // "y01: 1"
    // OR
    // "kpf AND jjs -> qsh"
    size_t colonPosition = parseLine.find(':');
    size_t arrowPosition = parseLine.find("->");
    if (colonPosition < parseLine.size())
    {
        name = parseLine.substr(0, 3);
        if (parseLine[colonPosition + 2] == '1')
        {
            state = True;
        }
        else
        {
            state = False;
        }
        type = INPUT;
    }
    else if (arrowPosition < parseLine.size())
    {
        name = parseLine.substr(arrowPosition + 3, 3);
        state = Unresolved;
        leftParent = parseLine.substr(0, 3);
        rightParent = parseLine.substr(arrowPosition - 4, 3);

        if (parseLine.find("AND") < parseLine.size())
            type = AND;
        if (parseLine.find("OR") < parseLine.size())
            type = OR;
        if (parseLine.find("XOR") < parseLine.size())
            type = XOR;
    }
    else
    {
        assert(false); //bad parse.
    }
    return name;
}

Puzzle24::LogicGate::WireState Puzzle24::LogicGate::ResolveLogicGate(std::map<std::string, LogicGate>& otherGates)
{
    //if (state == Unresolved)
    {
        switch (type)
        {
            case XOR:
                state = (otherGates[leftParent].ResolveLogicGate(otherGates) == True) != (otherGates[rightParent].ResolveLogicGate(otherGates) == True) ? True : False;
                break;
			case OR:
                state = (otherGates[leftParent].ResolveLogicGate(otherGates) == True) || (otherGates[rightParent].ResolveLogicGate(otherGates) == True) ? True : False;
                break;
			case AND:
                state = (otherGates[leftParent].ResolveLogicGate(otherGates) == True) && (otherGates[rightParent].ResolveLogicGate(otherGates) == True) ? True : False;
                break;
            default:
                //assert(false); //input should always be resolved.
                break;
        }
    }
    return state;
}

int64_t Puzzle24::LogicGate::CountParents(std::map<std::string, LogicGate>& otherGates)
{
    if (type == INPUT)
    {
        return 0;
    }
    else
    {
        return 2 + otherGates[leftParent].CountParents(otherGates) + otherGates[rightParent].CountParents(otherGates);
    }
}
int64_t Puzzle24::LogicGate::CountXORParents(std::map<std::string, LogicGate>& otherGates)
{
    if (type == INPUT)
    {
        return 0;
    }
    else
    {
        int count = 0;
        if (type == XOR)
            count = 1;
        return count + otherGates[leftParent].CountXORParents(otherGates) + otherGates[rightParent].CountXORParents(otherGates);
    }
}
int64_t Puzzle24::LogicGate::CountORParents(std::map<std::string, LogicGate>& otherGates)
{
    if (type == INPUT)
    {
        return 0;
    }
    else
    {
        int count = 0;
        if (type == OR)
            count = 1;
        return count + otherGates[leftParent].CountORParents(otherGates) + otherGates[rightParent].CountORParents(otherGates);
    }
}
int64_t Puzzle24::LogicGate::CountANDParents(std::map<std::string, LogicGate>& otherGates)
{
    if (type == INPUT)
    {
        return 0;
    }
    else
    {
        int count = 0;
        if (type == AND)
            count = 1;
        return count + otherGates[leftParent].CountANDParents(otherGates) + otherGates[rightParent].CountANDParents(otherGates);
    }
}
int64_t Puzzle24::LogicGate::CountINPParents(std::map<std::string, LogicGate>& otherGates)
{
    if (type == INPUT)
    {
        return 1;
    }
    else
    {
        return otherGates[leftParent].CountINPParents(otherGates) + otherGates[rightParent].CountINPParents(otherGates);
    }
}