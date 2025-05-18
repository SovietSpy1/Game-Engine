#pragma once
namespace dx3d {
	class Point {
	public:
		Point(){};
		Point(int x, int y) : x(x), y(y) {};
		Point(const Point& other): x(other.x), y(other.y){}
		int x = 0, y = 0;
	};
}