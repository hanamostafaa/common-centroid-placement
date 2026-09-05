#pragma once
#include "Models.h"

using namespace std;
struct ClosestFirst {
	bool operator()(const PairSlot& x, const PairSlot& y) const { return x.dist > y.dist; }
};

class Placer {
	Cell makeCell(const Device* d);

	double distanceFromCenter(int i, int j, double centerRow, double centerCol);
	int planOddPairing(vector<const Device*> &devices, vector<bool>& pairPrev);

public:

	Grid buildPlacement(const TestCase& tc);
};