#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <fstream> 
#include <stbi_image/stb_image_write.h>
#include "CPU Raytracer/thread_pool.h"
#include <omp.h> //this will be for better performance, but later implement thread pools for more efficeny, and render to a framebuffer so that we can write
//to both ppm and png at once


//also add render to framebuffer function, so that we can render to a framebuffer and then write to both ppm and png at once to make it faster and so we dont have to render twice

class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width = 100;  // Rendered image width in pixel count
	int    samples_per_pixel = 10; // Number of samples per pixel for anti-aliasing
    int    max_depth = 10;   // Maximum number of ray bounces into scene

	double vfov = 90; // Vertical view angle (field of view)
	point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
	point3 lookat = point3(0, 0, -1); // Point camera is looking at
    vec3 vup = vec3(0, 1, 0); // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    void RenderToPPM(const hittable& world, const char* filename) {
        initialize();

        // Allocate framebuffer
        std::vector<color> framebuffer(image_width * image_height);

//#pragma omp parallel for schedule(dynamic) //commnet this out for less performance, but less cpu usage
        for (int j = 0; j < image_height; j++) {
            std::clog << "Scanlines remaining: " << image_height - j << '\n';
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; s++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                framebuffer[j * image_width + i] = pixel_samples_scale * pixel_color;
            }
        }

        // File write — single-threaded
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        outFile << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                write_color(outFile, framebuffer[j * image_width + i]);
            }
        }

        std::clog << "\rDone rendering to PPM file: " << filename << '\n';
    }


    void RenderToPNG(const hittable& world, const char* filename) {
        initialize();

        std::vector<color> framebuffer(image_width * image_height);

//#pragma omp parallel for schedule(dynamic)
        for (int j = 0; j < image_height; ++j) {
            std::clog << "Scanlines remaining: " << (image_height - j) << '\n';
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                framebuffer[j * image_width + i] = pixel_color / samples_per_pixel;
            }
        }

        // Convert framebuffer to image data
        unsigned char* image = new unsigned char[image_width * image_height * 3];

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                const color& pixel_color = framebuffer[j * image_width + i];

                auto r = std::sqrt(std::clamp(pixel_color.x(), 0.0, 0.999));
                auto g = std::sqrt(std::clamp(pixel_color.y(), 0.0, 0.999));
                auto b = std::sqrt(std::clamp(pixel_color.z(), 0.0, 0.999));

                int ir = static_cast<int>(256 * r);
                int ig = static_cast<int>(256 * g);
                int ib = static_cast<int>(256 * b);

                // Flip vertically
                int flipped_j = image_height - 1 - j;
                int pixel_index = (flipped_j * image_width + i) * 3;
                image[pixel_index + 0] = static_cast<unsigned char>(ir);
                image[pixel_index + 1] = static_cast<unsigned char>(ig);
                image[pixel_index + 2] = static_cast<unsigned char>(ib);
            }
        }


        stbi_flip_vertically_on_write(true);
        if (!stbi_write_png(filename, image_width, image_height, 3, image, image_width * 3)) {
            std::cerr << "Failed to write PNG file: " << filename << std::endl;
        }

        delete[] image;

        std::clog << "\rDone rendering to PNG file: " << filename << '\n';
    }


private:
    int    image_height;   // Rendered image height
    double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
	vec3 u, v, w; // Camera frame basis vectors
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
		auto ray_time = random_double(); // Random time for motion blur

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        //If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0) {
			return color(0, 0, 0);
		}

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif