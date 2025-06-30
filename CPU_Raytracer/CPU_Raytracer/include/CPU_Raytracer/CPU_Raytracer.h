#ifndef CPU_RAYTRACER_H
#define CPU_RAYTRACER_H

#include <iostream>
#include <CPU_Raytracer/util/rtwk.h>
#include <CPU_Raytracer/util/hittable.h>
#include <CPU_Raytracer/util/hittable_list.h>
#include <CPU_Raytracer/util/sphere.h>
#include <CPU_Raytracer/util/camera.h>
#include <CPU_Raytracer/util/material.h>
#include <CPU_Raytracer/util/bvh.h>
#include <CPU_Raytracer/util/texture.h>

class CPURaytracer {
public:
	CPURaytracer();
	~CPURaytracer();

	void Render(const char* filename);
	std::vector<hittable_list>& getWorldVector() { return worlds; }
	const std::vector<hittable_list>& getWorldVector() const { return worlds; }
	void addWorld(hittable_list& world) { worlds.push_back(world); }
	void clearWorldVector() { worlds.clear(); }
	void setCamera(camera& c) { cam = c; }
	camera getCamera() const { return cam; }
private:
	void Init();
	void cameraInit();
	void setupWorld(hittable_list& world);

	camera cam;
	std::vector<hittable_list> worlds;
};

#endif // CPU_RAYTRACER_H