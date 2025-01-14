#pragma once

#include <functional>
#include <string>
#include <string>

//moved this into it's own class as multiple questions use the concept of a 2d coordinate. was originally just in puzzle 8

struct Coord
{
public:
	static Coord CardinalDirections[];

	int64_t x = 0;
	int64_t y = 0;
	
	Coord() {}

	Coord(int64_t nx, int64_t ny )
		: x(nx)
		, y(ny)

	{
	}

	Coord operator+(Coord const& c1) const
	{
		return Coord(x + c1.x, y + c1.y);
	}
	Coord operator-(Coord const& c1) const
	{
		return Coord(x - c1.x, y - c1.y);
	}
	Coord operator*(int64_t const& c1) const
	{
		return Coord(x * c1, y * c1);
	}

	// needed for std::set
	bool operator< (const Coord& e) const {
		if (x == e.x)
			return y < e.y;
		return x < e.x;
	}
	bool operator== (const Coord& e) const {
		return x == e.x && y == e.y;
	}

	bool ParseCoord(const std::string input);
};

//needed for std::unordered_map
class CoordHash {
public:

	// Use sum of lengths of first and last names
	// as hash function.
	size_t operator()(const Coord& k) const
	{
		//really really crappy hash i'm just confident this wont cause issues in the data sets we're using (usually <1000)
		return ((std::hash<int64_t>()(k.x)
			^ (std::hash<int64_t>()(k.y << 16))));
	}
};