#ifndef CPU_RAYTRACER_H
#define CPU_RAYTRACER_H

#include <iostream>
#include <CPU Raytracer/util/rtwk.h>
#include <CPU Raytracer/util/hittable.h>
#include <CPU Raytracer/util/hittable_list.h>
#include <CPU Raytracer/util/sphere.h>
#include <CPU Raytracer/util/camera.h>
#include <CPU Raytracer/util/material.h>
#include <CPU Raytracer/util/bvh.h>
#include <CPU Raytracer/util/texture.h>

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