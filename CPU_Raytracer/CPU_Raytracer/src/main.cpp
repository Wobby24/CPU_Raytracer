#include <iostream>
#include <CPU_Raytracer/CPU_Raytracer.h>

int main() {
	CPURaytracer raytracer;

	raytracer.Render("../CPU_Raytracer/res/image.ppm");
	raytracer.Render("../CPU_Raytracer/res/image.png");

	return 0;
}
