#ifndef CHAOS_GAME
#define CHAOS_GAME

#include "raylib/raylib.h"
#include <vector>
#include <random>
#include <cstdint>

enum RandomType {
	PURE_RANDOM,
	NO_DOUBLE,
	NO_ANTICLOCKWISE,
	NO_TWO_AWAY,
	NO_NEIGHBOUR
};

struct Point {
	double x;
	double y;
	Color colour;
};

class ChaosGame {
	public:
		ChaosGame(int _vertices);
		void input();
		void update();
		void draw();

	private:

		Point findMidpoint(Point p1, Point p2);
		std::vector<Point> initPolygon();
		int getRandomPointIndex();
		void drawPoint(Point point);
		void drawUI();

		// Polygon data
		std::vector<Point> poly_points;
		int vertices;
		int last_index;
		int last_last_index;

		Point current_point = { 0.0, 0.0, BLACK };

		float super_res = 1.0;

		// Interface details
		int screen_border;

		// Rendering
		RenderTexture2D screen_texture;
		Shader phase_shdr;

		// Program state
		bool running;
		bool can_modify;
		int iterations = 0;
	
		// Program behaviour
		int max_tickrate; //maximum number of iterations per second

		// Currently unused
		double start_time;
		double average_time;

		int point_size;
		int trace_point_size;
		int polygon_point_size;

		bool show_polygon_lines = true;
		bool show_polygon_points = true;	
	
		bool step = false;
		bool col_sys = false;
		RandomType random_type = PURE_RANDOM;
};

#endif

