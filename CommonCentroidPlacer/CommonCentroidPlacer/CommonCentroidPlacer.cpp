// CommonCentroidPlacer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "InputParser.h"
#include "Placer.h"
#include "Serializer.h"

int main()
{
	InputParser parser;
	bool includeDetails = false;
	std::string outputFormat = "XML"; 
	vector<TestCase> testCases = InputParser::parseInput("input.txt", includeDetails, outputFormat);
	std::cout << includeDetails << std::endl;
	std::cout << outputFormat << std::endl;



	for (TestCase& tc : testCases) {
		Placer placer;
		Grid grid = placer.buildPlacement(tc);
		std::string serializedOutput = Serializer::serializeCase(tc, grid, outputFormat, includeDetails);
		std::cout << serializedOutput << std::endl;
	}
}
