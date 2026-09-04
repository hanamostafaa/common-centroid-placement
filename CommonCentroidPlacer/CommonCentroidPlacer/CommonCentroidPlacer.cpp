#include <iostream>
#include "InputParser.h"
#include "Placer.h"
#include "Serializer.h"
using namespace std;
int main()
{
	InputParser parser;
	bool includeDetails = false;
	vector<TestCase> testCases;
	string outputFormat = "xml"; 
	try {
		testCases = InputParser::parseInput("input3.txt", includeDetails, outputFormat);
	}
	catch (const exception& e) {
		cerr << "Error parsing input: " << e.what() << endl;
		return 1;
	}
	for (TestCase& tc : testCases) {
		Placer placer;
		Grid grid = placer.buildPlacement(tc);
		string outputFileName = "output_case_" + to_string(tc.caseNumber) + "." + outputFormat;
		string serializedOutput = Serializer::serializeCase(tc, grid, outputFormat, includeDetails);
		try {
			Serializer::outputToFile(outputFileName, serializedOutput);
		}
		catch (const exception& e) {
			cerr << "error writing output to file: " << e.what() << endl;
			return 1;}
		cout << serializedOutput << endl;
	}
}
