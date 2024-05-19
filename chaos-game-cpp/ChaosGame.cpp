#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>
#include <string>
#include <cstdio>

#include "raylib/raylib.h"

#include "ChaosGame.h"

ChaosGame::ChaosGame(int _vertices) {
	// option parameters
	vertices = _vertices;

	//fixed parameters
	running = false;
	can_modify = true;
	screen_border = 10;
	max_tickrate = 60;
	last_index = vertices;
	last_last_index = vertices;

	poly_points = initPolygon();
	screen_texture = LoadRenderTexture(GetScreenWidth() * super_res, 
		GetScreenHeight() * super_res);

	//BeginTextureMode(screen_texture);
	//DrawRectangle(0, 0, screen_texture.texture.width, screen_texture.texture.height, BLACK);
	//EndTextureMode();
}

std::vector<Point> ChaosGame::initPolygon() {
	double delta_angle = 2 * M_PI / vertices;
	double radius = (double)(GetScreenHeight() * super_res / 2.0 - screen_border * super_res);
	std::vector<Point> points;

	for (int i = 0; i < vertices; i++) {
		double angle = delta_angle * i + M_PI;
		Color colour = ColorFromHSV(angle * 180.0f / M_PI, 0.8, 1);
		Point point;
		point.x = (double)GetScreenWidth() * super_res / 2.0 + radius * std::sin(angle);
		point.y = (double)GetScreenHeight() * super_res / 2.0 + radius * std::cos(angle)
			+ (double)screen_border * super_res;
		point.colour = colour;
		points.push_back(point);
	}
	return points;
}

void ChaosGame::input() { // check_events()
	int key = 0;
	while ((key = GetKeyPressed())) {
		if (key == KEY_SPACE) {
			running = !running;
		}
		if (key == KEY_S) {
			Image img = LoadImageFromTexture(screen_texture.texture);
			ExportImage(img, "test.png");
			UnloadImage(img);
		}
	}
}

Point ChaosGame::findMidpoint(Point p1, Point p2) {
	Point p;
	p.x = (p1.x + p2.x) / 2.0;
	p.y = (p1.y + p2.y) / 2.0;
	p.colour = BLACK;
	return p;
}

int ChaosGame::getRandomPointIndex() {
	int index = -1;

	switch (random_type) {
	case PURE_RANDOM:
		index = GetRandomValue(0, vertices - 1);
		break;

	case NO_DOUBLE:
		index = GetRandomValue(0, vertices - 2);
		if (index >= last_index) index++;
		break;

	case NO_ANTICLOCKWISE:
		index = GetRandomValue(0, vertices - 2);
		if (index >= ((last_index + 1) % vertices)) {
			index = (index + 1) % vertices;
		}
		break;

	case NO_TWO_AWAY:
		index = GetRandomValue(0, vertices - 1);
		while ((index + 2) % vertices == last_index
			|| (index - 2) % vertices == last_index) {
			index = GetRandomValue(0, vertices - 1);
		}
		break;

	case NO_NEIGHBOUR:
		index = GetRandomValue(0, vertices - 1);
		if (last_index == last_last_index) {
			while ((index == (last_index + 1) % vertices) 
				|| (index == (last_index - 1) % vertices)) {
				index = GetRandomValue(0, vertices - 1);
			}
		}
		break;
	}

	last_last_index = last_index;
	last_index = index;
	return index;
}

void ChaosGame::drawPoint(Point point) {
	point.colour = { 1, 1, 1, 255 };
	DrawPixel(point.x, point.y, point.colour);
}

void ChaosGame::update() {
	BeginTextureMode(screen_texture);
	BeginBlendMode(BLEND_ADDITIVE);

	int i = 0;
	if (running) {
		for (i = 0; i < 50000; i++) {
			int index = getRandomPointIndex();
			current_point = findMidpoint(current_point, poly_points.at(index));
			drawPoint(current_point);
		}
	}

	iterations += i;

	EndBlendMode();
	EndTextureMode();

	std::string iters = "Iterations: " + std::to_string(iterations);
	std::string iterspersec = "Iterations/s: " + std::to_string((int)(i / GetFrameTime()));
	DrawText(iters.c_str(), 5, 25, 20, RAYWHITE);
	DrawText(iterspersec.c_str(), 5, 45, 20, RAYWHITE);
}

void ChaosGame::draw() {
	for (int i = 0; i < vertices; i++) {
		int x1 = (int)(poly_points.at(i).x / super_res);
		int y1 = (int)(poly_points.at(i).y / super_res);
		int x2 = (int)(poly_points.at((i + 1) % vertices).x / super_res);
		int y2 = (int)(poly_points.at((i + 1) % vertices).y / super_res);
		DrawLine(x1, y1, x2, y2, GRAY);
	}

	DrawTexturePro(screen_texture.texture, 
		{ 0.0f, 0.0f, (float)screen_texture.texture.width, -(float)screen_texture.texture.height},
		{ 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
		{ 0.0f, 0.0f }, 0.0f, WHITE);	
}
