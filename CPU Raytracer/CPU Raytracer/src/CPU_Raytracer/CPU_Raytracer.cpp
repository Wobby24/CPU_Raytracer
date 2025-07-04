#include <CPU_Raytracer/CPU_Raytracer.h>

CPURaytracer::CPURaytracer() {
	Init();
	/*	
	for (int i = 0; i < 10; i++) {
		hittable_list world;
		setupWorld(world);
		addWorld(world);
	}
	*/
}

void CPURaytracer::Render(const char* filename) {

	if (!filename) {
		std::cerr << "Filename cannot be null." << std::endl;
		return;
	}
	switch (currentScene) {
		case SceneType::CheckeredSpheres: checkered_spheres(worlds[0]); break;
		case SceneType::BouncingSpheres: bouncing_spheres(worlds[0]); break;
	        case SceneType::Earth: earth(worlds[0]); break;
	}
	//if you wanted to render multiple worlds, you could loop through the worlds vector here
	if (worlds.empty()) {
		std::cerr << "No worlds available to render. Scene with no worlds was selected OR no worlds were added" << std::endl;
		return;
	}

	if (strcmp(getFileExtension(filename), ".ppm") == 0)
		cam.RenderToPPM(worlds[0], filename);

	else if (strcmp(getFileExtension(filename), ".png") == 0)
		cam.RenderToPNG(worlds[0], filename);
	else {
		std::cerr << "Unsupported file format: " << getFileExtension(filename) << std::endl;
		return;
	}
}

CPURaytracer::~CPURaytracer() {
	// Destructor implementation (if needed)
}

void CPURaytracer::Init() {
	SceneType scene = SceneType::CheckeredSpheres; // Default scene type
	// Initialize camera
	cameraInit();
	// Initialize world
	hittable_list world;
	//add it to the list of worlds
	addWorld(world);
}

void CPURaytracer::cameraInit() {
	// Initialize camera parameters
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.vfov = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;
}

void CPURaytracer::checkered_spheres(hittable_list& world) {
	world.clear();

	auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

	world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	world = hittable_list(make_shared<bvh_node>(world));

	//change the camera defocus angle for the checkered spheres scene;
	cam.defocus_angle = 0;
}

void CPURaytracer::bouncing_spheres(hittable_list& world) {
	// Initialize the world with some spheres
	world.clear();
	//init materials
	auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, .5), 0);
					world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	//bvh optimization, do not comment this out unless you want to disable BVH for testing purposes, or want to have a slower render time.
	//BVH is a bounding volume hierarchy that speeds up ray tracing by reducing the number of intersection tests.
	//It is a tree structure that partitions the space into bounding volumes, allowing the ray tracer to quickly eliminate large areas of space that do not contain any objects.
	//This is especially useful for scenes with a large number of objects, as it can significantly reduce the number of intersection tests required.
	//Ultimately, it can lead to faster rendering times and more efficient ray tracing.
	//If you want to disable BVH, just comment out the line below and uncomment the line after it.
	world = hittable_list(make_shared<bvh_node>(world));
}

void CPURaytracer::earth(hittable_list& world) {
	//clear world
	world.clear();
	//
	auto earth_texture = make_shared<image_texture>("res/images/earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

	//add the globe
	world.add(globe);

	cam.lookfrom = point3(0, 0, 12);

	cam.defocus_angle = 0;
}

//no longer needed, but kept for reference. its just the boucning spheres scene setup but in a seperate function
/*void CPURaytracer::setupWorld(hittable_list& world) {
	// Initialize the world with some spheres
	world.clear();
	
	//init materials
	auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + vec3(0, random_double(0, .5), 0);
					world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	//bvh optimization, do not comment this out unless you want to disable BVH for testing purposes, or want to have a slower render time.
	//BVH is a bounding volume hierarchy that speeds up ray tracing by reducing the number of intersection tests.
	//It is a tree structure that partitions the space into bounding volumes, allowing the ray tracer to quickly eliminate large areas of space that do not contain any objects.
	//This is especially useful for scenes with a large number of objects, as it can significantly reduce the number of intersection tests required.
	//Ultimately, it can lead to faster rendering times and more efficient ray tracing.
	//If you want to disable BVH, just comment out the line below and uncomment the line after it.
	world = hittable_list(make_shared<bvh_node>(world));
}
*/
