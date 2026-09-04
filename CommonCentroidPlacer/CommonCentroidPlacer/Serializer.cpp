#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Serializer.h" 

string Serializer::serializeCase(const TestCase& tc, const Grid& grid, const string& format, bool includeDetails) {
    if (format == "csv") {
        return toCSV(tc, grid, includeDetails);
    }
    else if (format == "xml") {
        return toXML(tc, grid, includeDetails);
    }
    else if (format == "json") {
        return toJSON(tc, grid, includeDetails);
    }
    return "Invalid format";
}

string Serializer::cellToDetailString(const Cell& cell) {
    ostringstream ss;
    if (cell.isDummy) {
        if (cell.dummyRef) {
            ss << cell.symbol << ";" << cell.dummyRef->name << ";" << cell.dummyRef->dummyType;
        }
    }
    else {

        ss << cell.symbol << ";" << cell.device->name << ";" << cell.device->type << ";" << cell.device->width << ";" << cell.device->length << ";" << cell.device->fingers;
    }
    return ss.str();
}

    string Serializer::toCSV(const TestCase& tc, const Grid& grid, bool includeDetails) {
    ostringstream ss;
    ss << "CaseNumber," << tc.caseNumber << "\n";
    ss << "Rows," << grid.rows << "\n";
    ss << "Cols," << grid.cols << "\n";
    ss << "Grid\n";

    for (int i = 0; i < grid.rows; ++i) {
        for (int j = 0; j < grid.cols; ++j) {
            if (includeDetails) {
                ss << "\"" << cellToDetailString(grid.cells[i][j]) << "\"";
            }
            else {
                ss << grid.cells[i][j].symbol;
            }
            if (j < grid.cols - 1) ss << ",";
        }
        ss << "\n";
    }
    return ss.str();
}

string Serializer::toXML(const TestCase& tc, const Grid& grid, bool includeDetails) {
    ostringstream ss;
    ss << "<PlacementResult case=\"" << tc.caseNumber << "\" rows=\"" << grid.rows << "\" cols=\"" << grid.cols << "\">\n";
    ss << "  <Grid>\n";

    for (int i = 0; i < grid.rows; ++i) {
        ss << "    <Row>";
        if (!includeDetails) {
            for (int j = 0; j < grid.cols; ++j) {
                ss << grid.cells[i][j].symbol;
                if (j < grid.cols - 1) ss << ", ";
            }
        }
        else {
            ss << "\n";
            for (int j = 0; j < grid.cols; ++j) {
                const auto& cell = grid.cells[i][j];
                if (!cell.isDummy)
                    ss << "      <Cell symbol=\"" << cell.symbol << "\" name=\"" << cell.device->name << "\" type=\"" << cell.device->type << "\" width=\"" << cell.device->width << "\" length=\"" << cell.device->length << "\" fingers=\"" << cell.device->fingers << "\" />" << endl;
                else
                    ss << "      <Cell symbol=\"" << cell.symbol << "\" name=\"" << cell.dummyRef->name << "\" dummyType=\"" << cell.dummyRef->dummyType << "\" />" << endl;
            }
            ss << "    ";
        }
        ss << "</Row>\n";
    }

    ss << "  </Grid>\n";
    ss << "</PlacementResult>\n";
    return ss.str();
}

string Serializer::toJSON(const TestCase& tc, const Grid& grid, bool includeDetails) {
    ostringstream ss;
    ss << "{\n";
    ss << "  \"case\": " << tc.caseNumber << ",\n";
    ss << "  \"rows\": " << grid.rows << ",\n";
    ss << "  \"cols\": " << grid.cols << ",\n";
    ss << "  \"grid\": [\n";

    for (int i = 0; i < grid.rows; ++i) {
        ss << "    [";
        if (includeDetails) ss << "\n";

        for (int j = 0; j < grid.cols; ++j) {
            const auto& cell = grid.cells[i][j];
            if (includeDetails) {
                if (!cell.isDummy && cell.device) {
                    ss << "      {\"symbol\":\"" << cell.symbol
                        << "\",\"name\":\"" << cell.device->name
                        << "\",\"type\":\"" << cell.device->type
                        << "\",\"width\":" << fixed << setprecision(1) << cell.device->width
                        << ",\"length\":" << cell.device->length
                        << ",\"fingers\":" << cell.device->fingers << "}";
                }
                else {
                    string dName = cell.dummyRef ? cell.dummyRef->name : "";
                    string dType = cell.dummyRef ? cell.dummyRef->dummyType : "";
                    ss << "      {\"symbol\":\"" << cell.symbol
                        << "\",\"name\":\"" << dName
                        << "\",\"dummyType\":\"" << dType << "\"}";
                }
            }
            else {
                ss << "\"" << cell.symbol << "\"";
            }

            if (j < grid.cols - 1) ss << ",";
            if (includeDetails) ss << "\n";
        }

        if (includeDetails) ss << "    ]";
        else ss << "]";

        if (i < grid.rows - 1) ss << ",\n";
        else ss << "\n";
    }

    ss << "  ]\n";
    ss << "}\n";
    return ss.str();
}