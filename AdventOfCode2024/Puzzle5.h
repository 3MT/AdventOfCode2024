#pragma once
#include <vector>
#include <string>

class Puzzle5
{
public:
	class PageRule
	{
	private:
		int left, right;
	public:
		PageRule(int64_t l, int64_t r)
		{
			left = l;
			right = r;
		}

		bool CheckRule(const std::vector<int64_t>& input) const;
		bool CheckRule(int64_t a, int64_t b) const;
	};

	static std::string SolvePuzzleA(const char* fileName);
	static std::string SolvePuzzleB(const char* fileName);
	static bool RunTests();

private:
	static PageRule ParseRule(const std::string& line);
	static std::vector<int64_t> ParsePages(const std::string& line);
};

