#include <iostream>
#include <string>
#include <CPU_Raytracer/CPU_Raytracer.h>

int main(int argc, char* argv[]) {
    SceneType scene = SceneType::Earth; // Default scene

    if (argc >= 2) {
        try {
            int sceneIndex = std::stoi(argv[1]);

            // Map the integer index to your SceneType enum
            switch (sceneIndex) {
                case 0: scene = SceneType::CheckeredSpheres; break;
                case 1: scene = SceneType::BouncingSpheres; break;
                case 2: scene = SceneType::Earth; break;
                // Add more scenes here as needed
                default:
                    std::cerr << "Invalid scene index. Using default (Earth).\n";
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid input. Please enter an integer scene index.\n";
            return 1;
        }
    } else {
        std::cout << "No scene index provided. Using default (Earth).\n";
    }

    CPURaytracer raytracer;
    raytracer.setCurrentScene(scene);
    raytracer.Render("res/output/image.png");

    return 0;
}
