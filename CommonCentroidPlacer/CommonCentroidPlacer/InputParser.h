# pragma once
#include "Models.h"

using namespace std;
enum class Mode { None, Devices, Dummies }; // current parsing mode


const string WHITESPACE = " \n\r\t\f\v";

class InputParser {

    static void ltrim(string& s);

    static void rtrim(string& s);

    static void trim(string& s);

    static bool startsWith(const string& line, const string& prefix);

    static pair<string, string> splitHeader(const string& line);

    static vector<string> splitCsv(const string& line);

    static bool parseBool(const string& s);

    static string parseOutput(const string& s);

    static Device parseDeviceRow(const vector<string>& f);

    static Dummy parseDummyRow(const vector<string>& f);

    public:

    static vector<TestCase> parseInput(const string& filepath, bool& includeDetails, string& outputFormat);
};