#pragma once
static class Solver
{
private:
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;
public:
	Solver(float width, float height);
	void wallCollision(float* quadAttribIdx, float* velocityIdx, float size);
	void updatePosition(float* quadAttribIdx, float* velocityIdx, float size);
};