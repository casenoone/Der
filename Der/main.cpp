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

void swap_vec3(point3 &A,point3 &B) {//vec3里实现一个复制构造函数
	point3 temp;
	temp = A;
	A = B;
	B = temp;
}

void trianle_box(point3 &A, point3 &B, point3 &C) {

}

//三角形包围盒顶点坐标：（x取最小的x，y取最小的y），（x取最大的x，y取最大的y）
//扫描填充法：首先求三角形包围盒，然后遍历包围盒覆盖的每一个像素，利用叉积判断该像素是否在三角形内
//具体实现不写了
void scan_triangle(point3 &A,point3 &B,point3 &C, TGAImage &image, TGAColor color) {
	
}

//重心坐标法填充三角形
//同样需要计算包围盒
/*bgra[0] = B;
bgra[1] = G;
bgra[2] = R;
bgra[3] = A;*/

void barycentric_triangle(point3 &A, point3 &B, point3 &C, TGAImage &image, vec3 colorA,vec3 colorB,vec3 colorC) {

	vec3 tempcolor;
	point3 minP = A;
	point3 maxP = A;
	if (minP.x() > B.x()) {
		minP.e[0] = B.x();
	}
	if (minP.x() > C.x()) {
		minP.e[0] = C.x();
	}

	if (maxP.x() < B.x()) {
		maxP.e[0] = B.x();
	}
	if (maxP.x() < C.x()) {
		maxP.e[0] = C.x();
	}

	if (minP.y() > B.y()) {
		minP.e[1] = B.y();
	}
	if (minP.y() > C.y()) {
		minP.e[1] = C.y();
	}

	if (maxP.y() < B.y()) {
		maxP.e[1] = B.y();
	}
	if (maxP.y() < C.y()) {
		maxP.e[1] = C.y();
	}
	
	double x_min = floor(minP.x());
	double x_max = ceil(maxP.x());
	double y_min = floor(minP.y());
	double y_max = ceil(maxP.y());



	double f_alpha = (B.y() - C.y())*A.x() + (C.x() - B.x())*A.y() + B.x()*C.y() - C.x()*B.y();
	//double f_beta = (C.y() - A.y())*B.x() + (A.x() - C.x())*B.y() + C.x()*A.y() - A.x()*C.y();
	double f_beta = (A.y() - C.y())*B.x() + (C.x() - A.x())*B.y() + A.x()*C.y() - C.x()*A.y();
	double f_gamma = (A.y() - B.y())*C.x() + (B.x() - A.x())*C.y() + A.x()*B.y() - B.x()*A.y();


	double f12= (B.y() - C.y())*(-1) + (C.x() - B.x())*(-1) + B.x()*C.y() - C.x()*B.y();
	//double f20= (C.y() - A.y())*(-1) + (A.x() - C.x())*(-1) + C.x()*A.y() - A.x()*C.y();
	double f20 = (A.y() - C.y())*(-1) + (C.x() - A.x())*(-1) + A.x()*C.y() - C.x()*A.y();
	double f01= (A.y() - B.y())*(-1) + (B.x() - A.x())*(-1) + A.x()*B.y() - B.x()*A.y();

	for (double y = y_min; y <= y_max; y++) {
		
		for (double x = x_min; x <= x_max; x++) {
			double alpha= (B.y() - C.y())*x + (C.x() - B.x())*y + B.x()*C.y() - C.x()*B.y();
			//double beta = (C.y() - A.y())*x + (A.x() - C.x())*y + C.x()*A.y() - A.x()*C.y();
			double beta = (A.y() - C.y())*x + (C.x() - A.x())*y + A.x()*C.y() - C.x()*A.y();

			double gamma= (A.y() - B.y())*x + (B.x() - A.x())*y + A.x()*B.y() - B.x()*A.y();


			if (alpha >= 0 && beta >= 0 && gamma >= 0) {
				std::cout << 1 << std::endl;

				if ((alpha>0||f_alpha*f12>0)&&(beta>0||f_beta*f20>0)&&(gamma>0||f_gamma*f01>0)) {
					//计算颜色插值

					tempcolor = alpha*colorA + beta*colorB + gamma*colorC;
					TGAColor lastcolor = TGAColor(tempcolor.x(), tempcolor.y(), tempcolor.z(), 255);
					image.set(x, y, lastcolor);

				}
			}
		}
	}
}

int main(int argc, char** argv) {

	TGAImage image(1000, 1000, TGAImage::RGB);
	
	midpointLine(0, 0, 1000, 1000, image, red);
	barycentric_triangle(point3(0, 0, 0), point3(10, 10, 0), point3(20, 0, 0), image, vec3(255, 0, 0), vec3(255, 0, 0), vec3(255, 0, 0));

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	return 0;

}