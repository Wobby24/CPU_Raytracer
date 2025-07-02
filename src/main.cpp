#include <iostream>
#include <string>
#include <CPU_Raytracer/CPU_Raytracer.h>

//int main(int argc, char* argv[]) {
int main() {
 //   if (argc < 2) {
   //     std::cout << "Usage: raytracer <scene_name>\n";
   //     return 1;
  //  }

   // std::string sceneName = argv[1];
    CPURaytracer raytracer;

    // Map from string to SceneType enum
  //  SceneType sceneEnum;
  //  if (sceneName == "CheckeredSpheres") {
   //     sceneEnum = SceneType::CheckeredSpheres;
  //  }
  //  else if (sceneName == "BouncingSpheres") {
  //      sceneEnum = SceneType::BouncingSpheres;
 //   }
  //  else {
   //     std::cerr << "Unknown scene: " << sceneName << std::endl;
   //     return 1;
  //  }
	SceneType scene = SceneType::Earth; // Default scene
    raytracer.setCurrentScene(scene);

  //  raytracer.Render("res/image.ppm");
    raytracer.Render("res/output/image.png");

    return 0;
}
