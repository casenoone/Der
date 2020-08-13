#include <cmath>
#include "tgaimage.h"
#include<iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//midpoint's Line Drawing Algorithm
void midpointLine(int x0,int y0,int x1,int y1, TGAImage &image, TGAColor color){

	bool k0 = false;//[0,1]
	bool k1 = false;//[-1,0]
	bool k2 = false;//[1,infinity]
	bool k3 = false;//[-1,-infinity]
	
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int delta_x = x0 - x1;
	int delta_y = y0 - y1;
	
	if (delta_x == 0) {
		k2 = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	else {

		float k = delta_y / (float)delta_x;

		if (k >= 0 && k <= 1) {
			k0 = true;
		}
		else if (k >= -1 && k < 0) {
			k1 = true;
			y0 = -y0;
			y1 = -y1;
		}
		else if (k > 1) {
			k2 = true;
			std::swap(x0, y0);
			std::swap(x1, y1);
		}
		else {
			
			k3 = true;
			x0 = -x0;
			x1 = -x1;
		
			std::swap(x0, y0);
			std::swap(x1, y1);
		}

	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	
	int x = x0, y = y0;
	delta_x = x1 - x0;
	delta_y = y0 - y1;
	double d = (delta_y)*(x0 + 1) + (delta_x)*(y0 + 0.5) + x0*y1 - x1*y0;

	for (x; x <= x1; ++x) {
		if (k0) {
			image.set(x, y, TGAColor(color));
		}
		if (k1) {
			image.set(x, -y, TGAColor(color));
		}
		if (k2) {
			image.set(y, x, TGAColor(color));
		}
		if (k3) {
			image.set(-y, x, TGAColor(color));
		}

		if (d < 0) {
			y += 1;
			d += delta_x + delta_y;
		}
		else{
			d += delta_y;
		}
	}
	

}

//Bresenham’s Line Drawing Algorithm
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

	bool steep = false;
	if (std::abs(x0 - x1)<std::abs(y0 - y1)) {

		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;

	}

	if (x0>x1) {

		std::swap(x0, x1);
		std::swap(y0, y1);

	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++) {

		if (steep) {

			image.set(y, x, TGAColor(color));

		}
		else {

			image.set(x, y, TGAColor(color));

		}

		error2 += derror2;

		if (error2>dx) {

			y += (y1>y0 ? 1 : -1);
			error2 -= dx * 2;

		}
	}
}


int main(int argc, char** argv) {

	TGAImage image(100, 100, TGAImage::RGB);
	for (int i = 0; i<1; i++) {

		//line(13, 13, 13, 1, image, white);
		/*line(20, 13, 40, 80, image, red);
		line(80, 40, 13, 20, image, red);*/
		
		midpointLine(1, 1, 30, 2, image, red);
		midpointLine(1, 1, 2, 30, image, red);
		midpointLine(1, 1, 100, 0, image, red);

	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;

}