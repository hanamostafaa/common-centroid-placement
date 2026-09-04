#pragma once
#include "Placer.h"
#include<algorithm>
#include<memory>
#include<cmath>
#include<queue>
using namespace std;

Cell Placer::makeCell(const Device* d) {
	Cell c;
	c.symbol = d->symbol;
	c.device = d;
	c.isDummy = false;
	return c;
}

double Placer::distanceFromCenter(int i, int j, double centerRow, double centerCol) {
	double dr = i - centerRow;
	double dc = j - centerCol;
	return sqrt(dr * dr + dc * dc); // eucludean distance
}

Grid Placer::buildPlacement(const TestCase& tc) {
	priority_queue<PairSlot, vector<PairSlot>, FarthestFirst> pairQueue;
	int total_counts = 0;
	for (Device d : tc.devices) {
		total_counts += d.count;
		if (d.count % 2 == 1) total_counts++; // preserve symmetry with dummy
	}
	int cols = ceil(total_counts / (double)tc.rows);

	pair<double, double> centerCell = { (tc.rows - 1) / 2.0, (cols - 1) / 2.0 };

	vector<vector<bool>> visited(tc.rows, vector<bool>(cols, false));
	Grid grid;
	grid.rows = tc.rows;
	grid.cols = cols;
	grid.cells.assign(tc.rows, vector<Cell>(cols));
	for (int j = 0; j < cols; j++)
	{
	for (int i = 0; i < tc.rows; i++)
		 {
			grid.cells[i][j] = { '\0', false, nullptr, nullptr }; // empty cells
			int mirror_i = tc.rows - 1 - i;
			int mirror_j = cols - 1 - j;
			if (!visited[i][j] && !(i == mirror_i && j == mirror_j)) {
				// push (i,j) and (mirror_i, mirror_j) to pairs pq (based on distance from center)
				double d = distanceFromCenter(i, j, centerCell.first, centerCell.second);
				pairQueue.push({ {i,j}, {mirror_i,mirror_j}, d });
				visited[mirror_i][mirror_j] = true;
				visited[i][j] = true;
			}
		}
	}
	// sort devices by symbol
	vector<const Device*> devices;
	for (const auto& dev : tc.devices) {
		devices.push_back(&dev);
	}
	sort(devices.begin(), devices.end(), [](const Device* a, const Device* b) {
		return a->symbol < b->symbol;
		});
	int remaining = 0;

	for (const Device* d : devices) {
		remaining = d->count;
		while (remaining > 0) {
			PairSlot p = pairQueue.top(); pairQueue.pop();
			if (remaining >= 2) {
				grid.cells[p.a.row][p.a.col] = makeCell(d);
				grid.cells[p.b.row][p.b.col] = makeCell(d);
				remaining -= 2;
			}
			else { // balance odd leftover with dummy
				grid.cells[p.a.row][p.a.col] = makeCell(d);
				grid.cells[p.b.row][p.b.col].dummyRef = &tc.dummy;
				grid.cells[p.b.row][p.b.col].symbol = tc.dummy.symbol;
				grid.cells[p.b.row][p.b.col].isDummy = true;
				remaining -= 1;
			}
		}
	}
	for (int i = 0; i < grid.rows; ++i) {
		for (int j = 0; j < grid.cols; ++j) {
			if (grid.cells[i][j].symbol == '\0') {  
				grid.cells[i][j].symbol = tc.dummy.symbol;
				grid.cells[i][j].dummyRef = &tc.dummy;
				grid.cells[i][j].isDummy = true;
			}
		}
	}
	return grid;


}