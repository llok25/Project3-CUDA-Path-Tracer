CUDA Path Tracer
================

**University of Pennsylvania, CIS 565: GPU Programming and Architecture, Project 3**

* Sau Lok Li
* Tested on: Windows, AMD Ryzen 9 270 w/ Radeon 780M Graphics, NVIDIA GeForce RTX 5070 Laptop GPU (8151 MiB), driver 596.13

![Cover render](img/cover1.png)

A GPU path tracer written in CUDA that renders arbitrary glTF models inside a fully path-traced environment. The cover scene is a desk lit by a ceiling panel and a glowing lamp bulb, with a red gem, a gold and a chrome sphere on the desktop, a full-length arched mirror, and a wire waste basket.

## Features

* CUDA path tracing with diffuse, emissive and specular (mirror) materials
* glTF mesh loading with [tinygltf v3](https://github.com/syoyo/tinygltf), with scene-level placement (translate / rotate / scale) driven from JSON
* Per-mesh and per-triangle AABB culling, togglable at runtime from the ImGui panel so the speedup can be measured live
* ImGui analytics panel: trace depth, intersection kernel time, frame time and FPS
*  material sorting, BVH, refraction

## Gallery

| Glass | Refraction |
|---|---|
| ![](img/glass1.png) | ![](img/refraction.png) |
| Stochastic antialiasing (off) | Stochastic antialiasing (on) |
|---|---|
| ![](img/SAA_off_crop.png) | ![](img/SAA_on_crop.png) |


## Loading glTF models

Meshes are loaded through `loadGLTF`, which flattens every triangle primitive into one triangle array and records the start index, count and bounding box on the owning `Geom`. The mesh `Geom` then carries its own transform from the scene JSON, so one model file can be instanced at any position, rotation and scale.

Models exported from Sketchfab store their orientation and scale in nested node matrices (for example, a -90° rotation to convert from Z-up to Y-up, and 0.059 and 0.01 scale factors on the lamp and mirror). The renderer reads raw vertex data, so these transforms are baked into the vertex positions and normals in an offline step. After baking, each model is a single node with world-space geometry, which means the scene JSON can be written in real units:

| Model | Triangles | Size |
|---|---|---|
| Desk | 144 | 4.8 × 3.0 × 2.6 |
| Lamp | 31,282 | 1.9 × 2.6 × 0.9 |
| Mirror | 1,230 | 0.6 × 1.7 × 0.04 |
| Basket | 45,684 | 0.34 × 0.29 × 0.34 |
| Gem | 30 | 41.6 × 33.5 × 41.6 (placed at scale 0.014) |

The cover scene totals about 78,000 triangles.

## Scene description

Objects are placed from JSON; each mesh entry picks a model file, a material and a transform:

```json
{
    "TYPE": "gltf",
    "FILE": "scenes/lamp.gltf",
    "MATERIAL": "diffuse_black",
    "TRANS": [-1.3, 2.69, -0.1],
    "ROTAT": [0, 0, 0],
    "SCALE": [0.8, 0.8, 0.8]
}
```

## Performance analysis

<!-- TODO: fill every table below with measured numbers. Do not leave estimates. -->

### AABB culling

Intersection kernel time, averaged over TODO iterations at 800×800, depth 8:

| Scene | Culling off (ms) | Culling on (ms) | Speedup |
|---|---|---|---|
| Cornell box + gem only | TODO | TODO | TODO |
| Desk scene (~78k triangles) | TODO | TODO | TODO |

Culling only helps rays that miss a mesh's bounding box. Rays that do hit a box still test every triangle in that mesh, so the basket and the lamp (45k and 31k triangles) dominate the remaining cost. TODO: explain what you observe, and whether a BVH is the next step.

### Per-kernel breakdown

Use stacked bars: one bar per configuration, one segment per kernel (generate rays, intersect, shade, compaction/sort, final gather), with timings from Nsight Systems or Nsight Compute.

![Kernel breakdown](img/kernel_breakdown.png)

## Build and run

1. Place the five models (`desk`, `lamp`, `gem`, `mirror`, `basket`, each a `.gltf` plus a `.bin`) in `scenes/`.
2. Configure and build with CMake (Visual Studio 2022 or later works well on Windows).
3. Run with the scene path as the first argument: `cis565_path_tracer scenes/custom_scene.json`

## Credits and references

* [tinygltf v3](https://github.com/syoyo/tinygltf) for glTF parsing
* [Dear ImGui](https://github.com/ocornut/imgui) for the analytics panel
* [nlohmann/json](https://github.com/nlohmann/json) for scene parsing
* [GLM](https://github.com/g-truc/glm) for math
* 3D models (all from Sketchfab):
  * This work is based on ["Computer Desk"](https://sketchfab.com/3d-models/computer-desk-05353724b7884bfb81211c7033a57fd4) by [felixawani](https://sketchfab.com/felixawani), licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)
  * This work is based on ["Desk lamp"](https://sketchfab.com/3d-models/desk-lamp-7377ec591df04445a1aae370017aaa13) by [KaramellGlass](https://sketchfab.com/KaramellGlass), licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)
  * This work is based on ["Red Gem"](https://sketchfab.com/3d-models/red-gem-3dcfeb2d4d9c4f37bf0c1cd57bad5a29) by [bang_m](https://sketchfab.com/bang_m), licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)
  * This work is based on ["Mirror B"](https://sketchfab.com/3d-models/mirror-b-cc2cc732368a44e8b3ad17d0ae0d86d0) by [DudleyLong](https://sketchfab.com/DudleyLong), licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)
  * ["Wastebasket"](https://sketchfab.com/3d-models/wastebasket-a29b85c6a0fc4d5ba8bdc0f39fdc6384) by [mariocanfly](https://sketchfab.com/mariocanfly), used under the [Sketchfab Standard license](https://sketchfab.com/licenses)
  * The models were modified: node transforms were baked into the vertices and texture and material data was removed.
* [Tips for writing an awesome README](https://github.com/pjcozzi/Articles/blob/master/CIS565/GitHubRepo/README.md)