#pragma once
#include "Models.h" 
using namespace std;

class Serializer {
public:
    static string serializeCase(const TestCase& tc, const Grid& grid, const string& format, bool includeDetails);
	static void outputToFile(const string& filename, const string& content);
private:
    static string cellToDetailString(const Cell& cell);

    static string toCSV(const TestCase& tc, const Grid& grid, bool includeDetails);

    static string toXML(const TestCase& tc, const Grid& grid, bool includeDetails);

    static string toJSON(const TestCase& tc, const Grid& grid, bool includeDetails);
};