#pragma once
#include "Models.h"

using namespace std;
struct FarthestFirst {
	bool operator()(const PairSlot& x, const PairSlot& y) const { return x.dist > y.dist; }
};

class Placer {
	Cell makeCell(const Device* d);

	double distanceFromCenter(int i, int j, double centerRow, double centerCol);

public:

	Grid buildPlacement(const TestCase& tc);
};