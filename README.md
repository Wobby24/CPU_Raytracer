**# CPU_Raytracer
A raytracer that has all of its calculations performed on the CPU and its results are displayed to a PPM or a PNG file

**# CPU_Raytracer

A CPU-based raytracer that performs rendering calculations on the CPU and outputs images as either PPM or PNG files (user can choose the output format via API).

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
- Normal and bump mapping support, alongside specular maps (specular maps are useful in path tracers to model surface reflectivity)  
- SIMD batched ray tracing for improved performance, with runtime checks for instruction set support to maximize compatibility  

---

## Performance and Optimization Strategies

The following techniques are planned or partially implemented to build a high-performance path tracer:

### 1. Multithreading  
Utilize all CPU cores efficiently with thread pools, work stealing, and load balancing.  
- **Example CPU:** Intel i5-12600K (6 P-cores + 4 E-cores = 16 threads with Hyper-Threading)  
- **Expected Gains:** 8–12× speedup over single-threaded execution (scene-dependent)

### 2. SIMD (AVX2 + FMA)  
Process multiple rays or triangles simultaneously using wide SIMD registers:  
- AVX2 (256-bit) for floats and ints  
- FMA3 instructions for efficient fused multiply-add operations, heavily used in shading and geometry math  
- **Expected Gains:** 2–4× per-core speedup over scalar code

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
- **Expected Gains:** 1.5–3× over naive Array of Structures (AOS)

- ### 4. Caching & Coherence  
- Sort rays by direction before BVH traversal  
- Use ray compaction after bounces  
- Spatially coherent batching (e.g., rendering in camera tiles)  
- Cache-align BVH nodes and triangle batches  
- **Expected Gains:** 1.5–2.5× throughput improvements

### 5. BVH Design  
- Use BVH8 (8 children per node) with SIMD traversal  
- Compress Axis-Aligned Bounding Boxes (AABBs) and store them SIMD-ready  
- **Expected Gains:** 1.5–2× over BVH2/BVH4 designs

### 6. Control Flow & Loop Efficiency  
- Avoid shading rays that contribute no light (dead rays)  
- Use Russian roulette and early termination to minimize unnecessary bounces  
- Inline fast paths, minimize branching per ray  
- **Expected Gains:** 1.2–1.5×

### 7. Shading Math Optimizations  
- Use FMA for dot products, Lambertian, Fresnel calculations  
- Normalize vectors with fast reciprocal square root approximations (similar to Quake’s famous trick)  
- Avoid expensive trig functions where possible, use fast approximations  
- **Expected Gains:** 1.2–1.5×, minor but important in inner loops

### 8. Additional Techniques (Optional and Scene Dependent)  
- Batch texture fetches  
- LRU caching for procedural materials  
- SIMD-aware sampling methods (blue noise, improved hemisphere sampling)  
- **Expected Gains:** Variable, roughly 5–20% in some cases

---

## Build Instructions

Currently, this project can **only be built using Visual Studio 2022**.  
- No CMake or Makefile setup yet.  
- Future plans include adding cross-platform build support.

---

## Conclusion

If development proceeds well, this project aims to evolve into a powerful, efficient CPU-based raytracer capable of handling complex scenes with advanced rendering techniques and hardware optimizations.

---

## References

- *Ray Tracing in One Weekend* series by Peter Shirley (https://raytracing.github.io/)  
- *Ray Tracing: The Next Week* by Peter Shirley  
- *Ray Tracing: The Rest of Your Life* by Eric Haines and Peter Shirley
