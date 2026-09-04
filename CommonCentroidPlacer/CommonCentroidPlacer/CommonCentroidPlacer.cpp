#include <iostream>
#include "InputParser.h"
#include "Placer.h"
#include "Serializer.h"
int main()
{
	InputParser parser;
	bool includeDetails = false;
	vector<TestCase> testCases;
	std::string outputFormat = "XML"; 
	try {
		testCases = InputParser::parseInput("input3.txt", includeDetails, outputFormat);
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing input: " << e.what() << std::endl;
		return 1;
	}
	for (TestCase& tc : testCases) {
		Placer placer;
		Grid grid = placer.buildPlacement(tc);
		string outputFileName = "output_case_" + std::to_string(tc.caseNumber) + "." + outputFormat;
		std::string serializedOutput = Serializer::serializeCase(tc, grid, outputFormat, includeDetails);
		Serializer::outputToFile(outputFileName, serializedOutput);
		std::cout << serializedOutput << std::endl;
	}
}
