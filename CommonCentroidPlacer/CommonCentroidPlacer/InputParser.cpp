# pragma once
#include "InputParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

void InputParser::ltrim(string& s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    if (start != string::npos) {
        s.erase(0, start);
    }
    else {
        s.clear();
    }
}

void InputParser::rtrim(string& s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    if (end != string::npos) {
        s.erase(end + 1);
    }
}

void InputParser::trim(string& s) {
    rtrim(s);
    ltrim(s);
}

bool InputParser::startsWith(const string& line, const string& prefix) {
    return line.compare(0, prefix.size(), prefix) == 0;
}

// #KEY=VALUE
pair<string, string> InputParser::splitHeader(const string& line) {
    string body = line.substr(1); // removing #
    auto eq = body.find('=');
	if (eq == string::npos) { // no = -> key with empty value
        trim(body);
        return { body, "" };
    };
    string key = body.substr(0, eq);
    trim(key);
    string value = body.substr(eq + 1);
    trim(value);
    return { key, value };
}

vector<string> InputParser::splitCsv(const string& line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, ','))
    {
        trim(field);
        fields.push_back(field);
    }
    return fields;
}

bool InputParser::parseBool(const string& s) {
    if (s == "true" || s == "1" || s == "True" || s == "TRUE") return true;
    if (s == "false" || s == "0" || s == "False" || s == "FALSE") return false;
    throw runtime_error("Invalid boolean value: " + s);
}
string InputParser::parseOutput(const string& s) {
    string lower;
    for (char c : s) lower += tolower(c);
    if (lower == "csv" || lower == "json" || lower == "xml") return lower;
    throw runtime_error("Invalid output format: " + s);
}

Device InputParser::parseDeviceRow(const vector<string>& f) {
    if (f.size() != 7)
        throw runtime_error("Device row must have 7 fields, got: " + to_string(f.size()));
    Device d;
    d.symbol = f[0][0];
    d.name = f[1];
    d.type = f[2];
    d.count = stoi(f[3]);
    d.width = stod(f[4]);
    d.length = stod(f[5]);
    d.fingers = stoi(f[6]);
    return d;
}

Dummy InputParser::parseDummyRow(const vector<string>& f) {
    if (f.size() != 3)
        throw runtime_error("Dummy row must have 3 fields, got: " + to_string(f.size()));
    Dummy d;
    d.symbol = f[0][0];
    d.name = f[1];
    d.dummyType = f[2];
    return d;
}

vector<TestCase> InputParser::parseInput(const string& filepath, bool& includeDetails, string& outputFormat) {
    ifstream file(filepath);
    if (!file.is_open())
        throw runtime_error("Could not open input file: " + filepath);

    vector<TestCase> cases;
    TestCase current{};
    bool haveOpenCase = false;

    Mode mode = Mode::None;
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        InputParser::trim(line);
        if (line.empty()) continue;

        if (InputParser::startsWith(line, "#")) {
            auto [key, value] = InputParser::splitHeader(line);

            if (key == "OUTPUT_FORMAT") {
                outputFormat = InputParser::parseOutput(value);
            }
            else if (key == "INCLUDE_DETAILS") {
                includeDetails = InputParser::parseBool(value);
            }
            else if (key == "CASE") {
                if (haveOpenCase)
                    cases.push_back(current);
                current = TestCase{};
                current.caseNumber = stoi(value);
                haveOpenCase = true;
                mode = Mode::None;
            }
            else if (key == "ROWS") {
                if (!haveOpenCase)
                    throw runtime_error("#ROWS before #CASE at line (no open case) " + to_string(lineNumber));
                current.rows = stoi(value);
            }
            else if (key == "DEVICES") {
                mode = Mode::Devices;
            }
            else if (key == "DUMMIES") {
                mode = Mode::Dummies;
            }
            continue;
        }

        if (!haveOpenCase)
            throw runtime_error("Data row outside any #CASE at line " + to_string(lineNumber));

        vector<string> fields = InputParser::splitCsv(line);
        switch (mode) {
        case Mode::Devices:
            current.devices.push_back(InputParser::parseDeviceRow(fields));
            break;
        case Mode::Dummies:
            current.dummy = InputParser::parseDummyRow(fields);
            break;
        case Mode::None:
            throw runtime_error("Data row with no (#DEVICES/#DUMMIES) at line " + to_string(lineNumber));
        }
    }

    if (haveOpenCase) cases.push_back(current);
    return cases;
}