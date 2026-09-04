#pragma once
#include <string>
#include <vector>
using namespace std;

struct Device {
    char symbol;
    string name;
    string type;
    int count;
    double width, length;
    int fingers;
};

struct Dummy {
    char symbol;
    string name;
    string dummyType;
};

struct TestCase {
    int caseNumber;
    int rows;
    vector<Device> devices;
    Dummy dummy;
};

struct Cell {
    char symbol; // alphabet or *
    bool isDummy;
    const Device* device = nullptr; // could use variant later
    const Dummy* dummyRef = nullptr; 
};

struct Grid {
    int rows, cols;
    vector<vector<Cell>> cells;
};
struct CellCoordinates {
	int row, col;
	CellCoordinates(int r, int c) : row(r), col(c) {}
};

struct PairSlot {
    CellCoordinates a, b;
    double dist;
};