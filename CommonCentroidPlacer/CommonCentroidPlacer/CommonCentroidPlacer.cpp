#include <iostream>
#include "InputParser.h"
#include "Placer.h"
#include "Serializer.h"
int main()
{
	InputParser parser;
	bool includeDetails = false;
	std::string outputFormat = "XML"; 

	vector<TestCase> testCases = InputParser::parseInput("input3.txt", includeDetails, outputFormat);
	for (TestCase& tc : testCases) {
		Placer placer;
		Grid grid = placer.buildPlacement(tc);
		std::string serializedOutput = Serializer::serializeCase(tc, grid, outputFormat, includeDetails);
		std::cout << serializedOutput << std::endl;
	}
}
