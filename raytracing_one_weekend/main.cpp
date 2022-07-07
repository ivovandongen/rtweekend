#include "common.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "camera.hpp"

#include <iostream>
#include <fstream>

color ray_color(const ray &r, const hittable_list &world, uint depth) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth == 0) {
        return {0, 0, 0};
    }

    static hit_record hit{};
    if (world.hit(r, 0.001, infinity, hit)) {
        ray scattered;
        color attenuation;
        if (hit.mat_ptr->scatter(r, hit, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return {0, 0, 0};
    }

    // Background gradient
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char **argv) {

    std::ofstream ofs{argc > 1 ? argv[1] : ""};
    if (argc > 1) {
        std::cout.rdbuf(ofs.rdbuf());
    }

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Camera
    camera cam;

    // World
    hittable_list world;
    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1);

    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::endl;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone." << std::endl;
}