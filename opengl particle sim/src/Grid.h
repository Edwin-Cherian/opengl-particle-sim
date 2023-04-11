#pragma once

#include <vector>

class Grid
{
private:
	static const int rows = 3;
	static const int cols = 3;
	std::vector<float*> grid [rows][cols];
public:
	Grid();
	~Grid();

	void AddObject(float* objPtr);
	void RemoveObject(float* objPtr);
	void FindNear(float* objPtr);
	void ReadData(float* objPtr);
};