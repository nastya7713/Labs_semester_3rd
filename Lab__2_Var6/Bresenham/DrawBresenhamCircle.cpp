#include "DrawBresenhamCircle.h"

void drawBresenhamCircle(HWND hwnd, int x0, int y0, int radius) {
	HDC hdc = GetDC(hwnd);
	//starting point x = 0, y equals radius
	int x = 0;
	int y = radius;
	
	//delta identifies the position of diagonal point:
	//1)delta > 0 diagonal point inside the circle
	//2)delta < 0 diagonal point outside the circle
	//3)delta = 0 diagonal point on the circle

	int delta = 1 - 2 * radius;
	int error = 0;//selecting at each step for the entry of the pixel point, the closest to the true circle, so that the error is minimal.

	//builts an arc for the first quadrant, and the coordinates of points for the other quadrants of the circle are symmetric
	while (y >= 0) {
		
		SetPixel(hdc, x0 + x, y0 + y, RGB(0, 0, 255));
		SetPixel(hdc, x0 + x, y0 - y, RGB(0, 0, 255));
		SetPixel(hdc, x0 - x, y0 + y, RGB(0, 0, 255));
		SetPixel(hdc, x0 - x, y0 - y, RGB(0, 0, 255));

		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;//moving horizontally
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;//vertical movement
			continue;
		}
		++x;
		delta += 2 * (x - y);//moving diagonally
		--y;
	}
	ReleaseDC(hwnd, hdc);
}