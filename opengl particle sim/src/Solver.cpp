#include "Solver.h"
#include <iostream>
#include <chrono>>
Solver::Solver(float width, float height)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height) {
	std::cout << WINDOW_HEIGHT << " ; " << WINDOW_WIDTH;
};

void Solver::wallCollision(float* quadAttribIdx, float* velocityIdx, float p_size)
{
	if (*(quadAttribIdx + 0) < 0)
	{
		*(quadAttribIdx + 0) = 0;
		*(velocityIdx + 0) *= -1;
	}
	if (*(quadAttribIdx + 0) > WINDOW_WIDTH - p_size)
	{
		*(quadAttribIdx + 0) = WINDOW_WIDTH - p_size;
		*(velocityIdx + 0) *= -1;
	}

	if (*(quadAttribIdx + 1) < 0)
	{
		*(quadAttribIdx + 1) = 0;
		*(velocityIdx + 1) *= -1;
	}
	if (*(quadAttribIdx + 1) > WINDOW_HEIGHT - p_size)
	{
		*(quadAttribIdx + 1) = WINDOW_HEIGHT - p_size;
		*(velocityIdx + 1) *= -1;
	}
}

void Solver::particleCollision(float* quadAttribIdx, int p_count, int p_size, int stride)
{
	//auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < p_count; i++)
	{
		int count = 0;
		//srand(time(NULL));
		float t_offset = (float)round(rand() % 2);
		for (int j = 0; j < p_count; j++)
		{
			if (pow(*(quadAttribIdx + stride * i + 0) - *(quadAttribIdx + stride * j + 0), 2) +
				pow(*(quadAttribIdx + stride * i + 1) - *(quadAttribIdx + stride * j + 1), 2) < pow(p_size, 2))
			{
				count++;
			}
		}
		if (count > 1)
		{
			*(quadAttribIdx + stride * i + 4)  = 1.0f;
			*(quadAttribIdx + stride * i + 9)  = 1.0f;
			*(quadAttribIdx + stride * i + 14) = 1.0f;
			*(quadAttribIdx + stride * i + 19) = 1.0f;
		}					  
		else				  
		{					  
			*(quadAttribIdx + stride * i + 4)  = 0.0f;
			*(quadAttribIdx + stride * i + 9)  = 0.0f;
			*(quadAttribIdx + stride * i + 14) = 0.0f;
			*(quadAttribIdx + stride * i + 19) = 0.0f;
		}
	}


	/*auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << duration.count() << std::endl;*/
}

void Solver::updatePosition(float* quadAttribIdx, float* velocityIdx, float p_size)
{
	*(quadAttribIdx + 0)  = *(quadAttribIdx + 0) + *(velocityIdx + 0);
	*(quadAttribIdx + 1)  = *(quadAttribIdx + 1) + *(velocityIdx + 1);
	*(quadAttribIdx + 5)  = *(quadAttribIdx + 0) + *(velocityIdx + 0) + p_size;
	*(quadAttribIdx + 6)  = *(quadAttribIdx + 1) + *(velocityIdx + 1);     
	*(quadAttribIdx + 10) = *(quadAttribIdx + 0) + *(velocityIdx + 0) + p_size;
	*(quadAttribIdx + 11) = *(quadAttribIdx + 1) + *(velocityIdx + 1) + p_size;
	*(quadAttribIdx + 15) = *(quadAttribIdx + 0) + *(velocityIdx + 0);
	*(quadAttribIdx + 16) = *(quadAttribIdx + 1) + *(velocityIdx + 1) + p_size;
}
