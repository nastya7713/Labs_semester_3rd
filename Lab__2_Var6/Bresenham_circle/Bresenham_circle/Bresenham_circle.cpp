#include "Bresenham_circle.h"

void drawBresenhamCircle(HWND hwnd, int x0, int y0, int radius) {
	int x = 0;
	HDC hdc = GetDC(hwnd);
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {

		SetPixel(hdc, x0 + x, y0 + y, RGB(0, 0, 255));
		SetPixel(hdc, x0 + x, y0 - y, RGB(0, 0, 255));
		SetPixel(hdc, x0 - x, y0 + y, RGB(0, 0, 255));
		SetPixel(hdc, x0 - x, y0 - y, RGB(0, 0, 255));

		error = 2 * (delta + y) - 1;

		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
	ReleaseDC(hwnd, hdc);
}
