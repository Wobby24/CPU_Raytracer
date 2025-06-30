# CPU_Raytracer
A raytracer, soon to be pathtracer that is able to calculate a raytraced scene then display it to a PPM or PNG file


---

## Current Features

- Intermediate-level BVH (Bounding Volume Hierarchy) acceleration structure tweaking  
- Adjustable image parameters, camera position, and relevant settings  
- Motion blur support  
- Basic material support  
- Sphere rendering  

---

## Planned Features / To-Do

- Improved and more efficient BVH implementation  
- Completion of topics from:
  - *Ray Tracing: The Next Week*  
  - *Ray Tracing: The Rest of Your Life*  
  > **Authors:**  
  > Eric Haines and Peter Shirley  
  > (These books are part of the “Ray Tracing in One Weekend” series and provide advanced techniques for ray tracing.)  

- Support for triangle-based rendering instead of per-pixel calculation (enables more complex geometry)  
- Mesh loading via [tinygltf](https://github.com/syoyo/tinygltf)  
- Physically Based Rendering (PBR) support  
- Normal mapping, bump mapping, roughness, AO (ambient occulsion mapping) metallic mapping etc.
- SIMD batched ray tracing for improved performance, with runtime checks for instruction set support to maximize compatibility  

---

## Performance and Optimization Strategies

The following techniques are planned or partially implemented to build a high-performance path tracer:

### 1. Multithreading  
Utilize all CPU cores efficiently with thread pools, work stealing, and load balancing.  
- **Example CPU:** Intel i5-12600K (6 P-cores + 4 E-cores = 16 threads with Hyper-Threading)  

### 2. SIMD (AVX2 + FMA or older extensions like SSE)  
Process multiple rays or triangles simultaneously using wide SIMD registers:  
- AVX2 (256-bit) for floats and ints  
- FMA3 instructions for efficient fused multiply-add operations, heavily used in shading and geometry math
- SSE for older CPUs that don't support modern SIMD instructions like AVX (or SSE2 and later versions could be implemented for max compatiblity and performance)
- Scalar mode will still be utlized for CPUs that don't support "modern" SIMD instructions

### 3. Efficient Memory Layout  
Use Structure of Arrays (SOA), aligned memory, prefetching, and avoid indirection:  
- Example:  
  ```cpp
  struct RayPacket {
    float ox[8], oy[8], oz[8]; // origins
    float dx[8], dy[8], dz[8]; // directions
    // ...
  };
- BVH nodes and triangle data stored linearly and SIMD-ready  

- ### 4. Caching & Coherence  
- Sort rays by direction before BVH traversal  
- Use ray compaction after bounces  
- Spatially coherent batching (e.g., rendering in camera tiles)  
- Cache-align BVH nodes and triangle batches  

### 5. BVH Design  
- Use BVH8 (8 children per node) with SIMD traversal  
- Compress Axis-Aligned Bounding Boxes (AABBs) and store them SIMD-ready  

### 6. Control Flow & Loop Efficiency  
- Avoid shading rays that contribute no light (dead rays)  
- Use Russian roulette and early termination to minimize unnecessary bounces  
- Inline fast paths, minimize branching per ray  

### 7. Shading Math Optimizations  
- Use FMA for dot products, Lambertian, Fresnel calculations  
- Normalize vectors with fast reciprocal square root approximations (similar to Quake’s famous trick)  
- Avoid expensive trig functions where possible, use fast approximations  

### 8. Additional Techniques (Optional and Scene Dependent)  
- Batch texture fetches  
- LRU caching for procedural materials  
- SIMD-aware sampling methods (blue noise, improved hemisphere sampling)  

---

## Build Instructions

CMake is supported. Version 3.16 is required to build project, but ulitmatley I haven't run into any problems building it just quite yet.
Build instructions:

### Requirements

- CMake >= 3.16
- A C++ compiler (e.g., MSVC, GCC, Clang) WITH C++20 support
- Git (optional, if cloning the repo)
- Make/Ninja or a supported build system (e.g., Visual Studio)

Steps to Build:

1. Clone the repository:
  ```bash
  git clone https://github.com/Wobby24/CPU_Raytracer.git
  cd CPU_Raytracer
 ```
2. Create a build directory and relocate to it
  ```bash
  mkdir build
  cd build
  ```
3. Generate the build files
   ```bash
   cmake ..
   ```
3a. you can specifiy build system options if you want to build using a particular system, like ninja or visual studio by using -G
  ```bash
  cmake .. -G "Ninja" # OR "Visual Studio 17 2022" for Windows
  ```
4. Build the project
  ```bash
  cmake --build . #You can also use --config Release or --config Debug if using something like Visual Studio
  ```
The project should be able to run from there. The exectuable should be located in the bin folder, aswell as the output images being located in the "res" folder within the project source code
---

## Conclusion

If development proceeds well, this project aims to evolve into a powerful, efficient CPU-based path-tracer capable of handling complex scenes with advanced rendering techniques and hardware optimizations.

---

## References

- *Ray Tracing in One Weekend* series by Peter Shirley (https://raytracing.github.io/)  
- *Ray Tracing: The Next Week* by Peter Shirley  
- *Ray Tracing: The Rest of Your Life* by Eric Haines and Peter Shirley
