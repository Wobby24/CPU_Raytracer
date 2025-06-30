#include <iostream>
#include <CPU_Raytracer/CPU_Raytracer.h>

int main() {
	CPURaytracer raytracer;

	raytracer.Render("res/image.ppm");
//	raytracer.Render("res/image.png");

	return 0;
}