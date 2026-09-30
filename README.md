CUDA Path Tracer
================

**University of Pennsylvania, CIS 565: GPU Programming and Architecture, Project 3**

* (TODO) YOUR NAME HERE
* Tested on: (TODO) Windows 22, i7-2222 @ 2.22GHz 22GB, GTX 222 222MB (Moore 2222 Lab)

### (TODO: Your README)

### Arbitrary Mesh Rendering

glTF triangle meshes can be added to a scene through an object entry. `FILE` is
resolved relative to the scene JSON file, and the object transform and material
are applied to the loaded mesh:

```json
{
	"TYPE": "gltf",
	"FILE": "scene.gltf",
	"MATERIAL": "diffuse_white",
	"TRANS": [0.0, 1.0, 0.0],
	"ROTAT": [0.0, 0.0, 0.0],
	"SCALE": [3.0, 3.0, 3.0]
}
```

The Path Tracer Analytics window contains the `Cull mesh triangles with AABB`
toggle, enabled by default. With culling enabled, each mesh's object-space
bounding box is tested before its triangles; disabling it tests every triangle
in the mesh. The displayed intersection GPU time is the running average, in
milliseconds, of the intersection kernels over all path depths for each render
iteration. Changing the toggle restarts accumulation and resets this average.

For a performance comparison, use the same mesh, scene, camera, resolution, and
render settings for both toggle states. Record the displayed average after it
stabilizes, along with the triangle count and GPU model. The measurement includes
intersection work only, not shading or the rest of the frame. No benchmark
values are included here because they depend on the model and GPU used.

*DO NOT* leave the README to the last minute! It is a crucial part of the
project, and we will not be able to grade you without a good README.

