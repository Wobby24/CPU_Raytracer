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

enum class SceneType {
	BouncingSpheres = 0,
	CheckeredSpheres = 1,
	Earth = 2,
	// Add more scene types as needed
};

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
	SceneType getCurrentScene() const { return currentScene; }
	void setCurrentScene(SceneType scene) { currentScene = scene; }
private:
	SceneType currentScene;
	void Init();
	void cameraInit();
//	void setupWorld(hittable_list& world); //no longer needed, as worlds are now managed by scene type and that calls a function to set up the world
	void bouncing_spheres(hittable_list& world);
	void checkered_spheres(hittable_list& world);
	void earth(hittable_list& world);

	camera cam;
	std::vector<hittable_list> worlds;
};

#endif // CPU_RAYTRACER_H
