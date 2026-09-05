#include <iostream>
#include <vector>
#include <string>

#include "InputParser.h"
#include "Placer.h"
#include "Serializer.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
		cerr << "file path not provided . usage: " << argv[0] << " <input_file_path>" << endl;
        return 1;
    }

    string inputFileName = argv[1];
    bool includeDetails = false;
    string outputFormat = "xml";
    vector<TestCase> testCases;

    try
    {
        testCases = InputParser::parseInput(inputFileName, includeDetails, outputFormat);
    }
    catch (const exception& e)
    {
        cerr << "Err parsing input: " << e.what() << endl;
        return 1;
    }

    for (TestCase& tc : testCases)
    {
        try
        {
            Placer placer;
            Grid grid = placer.buildPlacement(tc);

            string outputFileName = "output_case_" + to_string(tc.caseNumber) + "." + outputFormat;
            string serializedOutput = Serializer::serializeCase(tc, grid, outputFormat, includeDetails);
            Serializer::outputToFile(outputFileName, serializedOutput);
            cout << serializedOutput << endl;
        }
        catch (const exception& e)
        {
            cerr << "Error processing case "<< tc.caseNumber << ": "<< e.what() << endl;
            return 1;
        }
    }

    return 0;
}