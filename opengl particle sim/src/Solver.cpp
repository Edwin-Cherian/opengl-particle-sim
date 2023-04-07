#include "Solver.h"
#include <iostream>

Solver::Solver(float width, float height)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height) {
	std::cout << WINDOW_HEIGHT << " ; " << WINDOW_WIDTH;
};

void Solver::wallCollision(float* quadAttribIdx, float* velocityIdx, float size)
{
	if (*(quadAttribIdx + 0) < 0)
	{
		*(quadAttribIdx + 0) = 0;
		*(velocityIdx + 0) *= -1;
	}
	if (*(quadAttribIdx + 0) > WINDOW_WIDTH - size)
	{
		*(quadAttribIdx + 0) = WINDOW_WIDTH - size;
		*(velocityIdx + 0) *= -1;
	}

	if (*(quadAttribIdx + 1) < 0)
	{
		*(quadAttribIdx + 1) = 0;
		*(velocityIdx + 1) *= -1;
	}
	if (*(quadAttribIdx + 1) > WINDOW_HEIGHT - size)
	{
		*(quadAttribIdx + 1) = WINDOW_HEIGHT - size;
		*(velocityIdx + 1) *= -1;
	}
}

void Solver::updatePosition(float* quadAttribIdx, float* velocityIdx, float size)
{
	*(quadAttribIdx + 0)  = *(quadAttribIdx + 0) + *(velocityIdx + 0);
	*(quadAttribIdx + 1)  = *(quadAttribIdx + 1) + *(velocityIdx + 1);
	*(quadAttribIdx + 4)  = *(quadAttribIdx + 0) + *(velocityIdx + 0) + size;
	*(quadAttribIdx + 5)  = *(quadAttribIdx + 1) + *(velocityIdx + 1);     
	*(quadAttribIdx + 8)  = *(quadAttribIdx + 0) + *(velocityIdx + 0) + size;
	*(quadAttribIdx + 9)  = *(quadAttribIdx + 1) + *(velocityIdx + 1) + size;
	*(quadAttribIdx + 12) = *(quadAttribIdx + 0) + *(velocityIdx + 0);
	*(quadAttribIdx + 13) = *(quadAttribIdx + 1) + *(velocityIdx + 1) + size;
}
