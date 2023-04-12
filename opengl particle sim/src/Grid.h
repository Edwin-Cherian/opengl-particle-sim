#pragma once

#include <vector>

class Grid
{
private:
	static const int rows = 175;
	static const int cols = 300;
	std::vector<float*> grid [rows][cols];
public:
	Grid();
	~Grid();

	void AddObject(float* objPtr);
	void RemoveObject(float* objPtr);
	std::vector<float*> FindNear(float* objPtr);
	void ReadData(float* objPtr);
};