#include "Coord.h"

Coord Coord::CardinalDirections[4] = {Coord(0,-1), Coord(0,1), Coord(-1,0), Coord(1,0)};

bool Coord::ParseCoord(const std::string input)
{
	try
	{
		//expects "x,y" format
		//undefined outside that.
		size_t index = 0;
		x = std::stoll(input, &index);
		index++; // ','
		y = std::stoll(input.substr(index));
	}
	catch (...)
	{
		//parse failed
		return false;
	}
	return true;
}
