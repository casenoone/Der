#include <cmath>
#include "tgaimage.h"
#include<iostream>
#include"vec3.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//扫描法光栅化三角形，写失败了。。。。
void drawTriangle(point3 A,point3 B,point3 C, TGAImage &image, TGAColor color) {//B在左，A在右

	point3 temp;
	if (A.y() <= B.y()) {
		temp = A;
		A = B;
		B = temp;
	}

	if (A.y() <= C.y()) {
		temp = A;
		A = C;
		C = temp;
	}

	point3 midP((B.x() + C.x()) / 2, (B.y() + C.y()) / 2, 0);
	

	if (B.x() - midP.x() > 0) {//如果B点在中点右侧
		temp = B;
		B = C;
		C = temp;
	}
	
	bool k0 = false;//[0,1]
	bool k1 = false;//[-1,0]
	bool k2 = false;//[1,infinity]
	bool k3 = false;//[-1,-infinity]

	int x1 = midP.x();
	int x0 = A.x();
	int y1 = midP.y();
	int y0 = A.y();

	int tri_x0 = x0;
	int tri_x1 = x1;
	int tri_y0 = y0;
	int tri_y1 = y1;



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
	int temp_x = 0;
	int temp_y = 0;


	point3 left_A(A.x(), A.y(), A.z());
	point3 left_B(B.x(), B.y(), B.z());
	point3 right_A(A.x(), A.y(), A.z());
	point3 right_C(C.x(), C.y(), C.z());

	if (left_B.x() > left_A.x()) {
		temp = left_B;
		left_B = left_A;
		left_A = temp;
	}

	if (right_C.x() > right_A.x()) {
		temp = right_C;
		right_C = right_A;
		right_A = temp;
	}

	int left_flage = 0;
	int right_flage = 0;


	/*int tri_x0 = x0;
	int tri_x1 = x1;
	int tri_y0 = y0;
	int tri_y1 = y1;*/
	
	double left_equation; 
	double right_equation; 

	double left_equations = (left_B.y() - left_A.y())*midP.x() + (left_A.x() - left_B.x())*midP.y() + left_B.x()*left_A.y() - left_A.x()*left_B.y();
	double right_equations = (right_C.y() - right_A.y())*midP.x() + (right_A.x() - right_C.x())*midP.y() + right_C.x()*right_A.y() - right_A.x()*right_C.y();

	if (left_equations >= 0) {
		left_flage = 1;
	}
	else {
		left_flage = -1;
	}


	if (right_equations >= 0) {
		right_flage = 1;
	}
	else {
		right_flage = -1;
	}
	

	for (x; x <= x1; ++x) {
		
		if (k0) {
			temp_x = x;
			temp_y = y;

		}
		if (k1) {
			temp_x = x;
			temp_y = -y;
		}
		if (k2) {
			temp_x = y;
			temp_y = x;
		}
		if (k3) {
			temp_x = -y;
			temp_y = x;
		}

		image.set(temp_x, temp_y, TGAColor(color));

		left_equation = (left_B.y() - left_A.y())*temp_x + (left_A.x() - left_B.x())*temp_y + left_B.x()*left_A.y() - left_A.x()*left_B.y();
		right_equation = (right_C.y() - right_A.y())*temp_x + (right_A.x() - right_C.x())*temp_y + right_C.x()*right_A.y() - right_A.x()*right_C.y();
		int temp_x1 = temp_x;
		int temp_x2 = temp_x;
		int count = 0;

		while ((left_equation >= 0 && left_flage >= 0) || (left_equation < 0 && left_flage < 0)) {
			left_equation -= (left_B.y() - left_A.y());
			--temp_x1;
			image.set(temp_x1, temp_y, TGAColor(color));
			//std::cout << left_equation << std::endl;
			image.set(temp_x1, temp_y, TGAColor(color));
			//std::cout << temp_x1 << std::endl;

			count++;
		}
		std::cout << "--------"<<std::endl;
		while ((right_equation >= 0 && right_flage >= 0) || (right_equation < 0 && right_flage < 0)) {
			right_equation += (right_C.y() - right_A.y());
			++temp_x2;
			image.set(temp_x2, temp_y, TGAColor(color));
			//++count;
			//std::cout << temp_x2 << std::endl;
			
		}
		//std::cout << count<<std::endl;
		if (d < 0) {
			y += 1;
			d += delta_x + delta_y;
		}
		else {
			d += delta_y;
		}
	}
}

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
		
		/*midpointLine(1, 1, 30, 2, image, red);
		midpointLine(1, 1, 2, 30, image, red);
		midpointLine(1, 1, 100, 0, image, red);*/
		/*drawTriangle(point3(12, 9, 0), point3(24, 15, 0), point3(6, 21, 0), image, red);
		drawTriangle(point3(12, 9, 0), point3(24, 15, 0), point3(15, 6, 0), image, white);*/
		drawTriangle(point3(10, 10, 0), point3(0, 0, 0), point3(10, 0, 0), image, red);
		drawTriangle(point3(10, 10, 0), point3(10, 0, 0), point3(20, 0, 0), image, white);

	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;

}