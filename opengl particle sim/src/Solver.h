#pragma once
static class Solver
{
private:
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;
public:
	Solver(float width, float height);
	void wallCollision(float* quadAttribIdx, float* velocityIdx, float size);
	void particleCollision(float* quadAttribIdx, int p_count, int p_size, int quad_vacount);
	void updatePosition(float* quadAttribIdx, float* velocityIdx, float size);
};