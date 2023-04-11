#include "Grid.h"
#include <iostream>

Grid::Grid()
{

}

Grid::~Grid()
{
    std::cout << "make a grid destructor pls";
}

void Grid::AddObject(float* objPtr)
{
	const int cell_row = static_cast<int>(objPtr[0] / ((int) 1200.0f/rows));
	const int cell_col = static_cast<int>(objPtr[1] / ((int) 700.0f/cols));
    //std::cout << "Adding obj: " << cell_row << " " << cell_col << std::endl;
	grid[cell_row][cell_col].push_back(objPtr);
}

/* does not remove given object ptr from vector in a grid cell. Instead, it
removes the last element in the vector. This is ok in the given context because every
element will be removed from the grid before being added.
Could be sped up by just clearing vector. consider what to do when other particles
try to clear the same vector.*/
void Grid::RemoveObject(float* objPtr)
{
    const int cell_row = static_cast<int>(objPtr[0] / ((int)1200.0f / rows));
    const int cell_col = static_cast<int>(objPtr[1] / ((int)700.0f / cols));
    //std::cout << "Deleting obj: " << cell_row << " " << cell_col << std::endl;
    grid[cell_row][cell_col].pop_back();
}

void Grid::FindNear(float* objPtr)
{
    const int cell_row = static_cast<int>(objPtr[0] / ((int)1200.0f / rows));
    const int cell_col = static_cast<int>(objPtr[1] / ((int)700.0f / cols));
    //std::cout << "Finding near: " << cell_row << " " << cell_col << std::endl;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (cell_row + i < 0 || cell_row + i >= rows || cell_col + j < 0 || cell_col + j >= cols)
                continue;
            std::cout << cell_row + i << " " << cell_col + j << " " << std::endl;
            for (auto v : grid[cell_row + i][cell_col + j])
            {
                std::cout << cell_row + i << " " << cell_col + j << " " << *v << std::endl;
            }
        }
    }
}

void Grid::ReadData(float* objPtr)
{
	/*const int cell_row = static_cast<int>(objPtr[0] / rows);
	const int cell_col = static_cast<int>(objPtr[1] / cols);
	grid[rows][cols].push_back(objPtr);*/
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (auto v : grid[i][j])
            {
                std::cout << i << " " << j << " " << *v << ", " << *(v+1) << std::endl;
            }
        }
    }
}
