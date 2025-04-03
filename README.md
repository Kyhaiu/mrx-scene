# <h1 align="center">MRX Engine</h1>

> A graphics engine initially made for the final paper at Computer Graphics course at UNIOESTE. Made from scratch using C++ without using any engine to draw objects

## Dependencies

The code dependencies are:

- [OpenGL](https://opengl.org/) Used because of SDL2
- [ImGui](https://github.com/ocornut/imgui)
- [SDL2](https://www.libsdl.org/) Used for drawing pixels and only for that
- [GoogleTest](https://github.com/google/googletest)

The code is built using [`xmake`](https://xmake.io/) and the dependencies are installed automatically through. The only exception is the ImGui library, which is included as a submodule.
[`xrepo`](https://github.com/xmake-io/xrepo).

## TL;DR

### To run it.

On Linux:

```bash
xmake && xmake run
```

On Windows:

```bash
xmake; xmake run
```

### In case of errors during the installation of the dependencies:

Sometimes the dependencies are not installed correctly, due to a lot of reasons. When this happens, you can try to install manually the dependencies.

## Showcase

<details>
  <summary>
    Initial scene
  </summary>
  <p align="center">
    <br />
    <img src="https://raw.githubusercontent.com/Kyhaiu/mrx-scene/refs/heads/main/images/initial-scene.png" width=1200 />
  </p>
</details>

<details>
  <summary>
    Hierarchy
  </summary>

  <p>This is where the entities are listed and created.</p>
  <p align="center">
    <img src="https://raw.githubusercontent.com/Kyhaiu/mrx-scene/refs/heads/main/images/hierarchy.png" width=150 />
  </p>
</details>

<details>
  <summary>
    Inspector
  </summary>

  <br />
  <p>When you click in a element from the hierarchy it will display into the inspector it's information</p>

  <p align="center">
    <img src="https://raw.githubusercontent.com/Kyhaiu/mrx-scene/refs/heads/main/images/object%20inspector.png" width=300 />
  </p>

</details>

<details>
  <summary>
    Viewport
  </summary>

  <br />
  <p>The viewport is the main area of render, this is were the program draw frames</p>

  <p align="center">
    <img src="https://raw.githubusercontent.com/Kyhaiu/mrx-scene/refs/heads/main/images/viewport.png" width=1200 />
  </p>
</details>

## Features

### Pipelines

- [x] Left-handed coordinate system (Adair)
- [x] Right-handed coordinate system (Smith)

### Objects

You can add the following objects to the scene:

- [x] Sphere (IcoSphere)
- [x] Cube
- [x] Pyramid (tetrahedron)
- [x] Torus
- [x] Cone
- [x] Cylinder
- [x] Point light
- [ ] Directional light

### Shading

> ⚠️ The current version of the MRX engine only supports local shading.
> Amongst the shading models available, you can choose between:

- [x] Flat shading
- [x] Gouraud shading
- [x] Phong shading

> OBS: The shading model is chosen globally.

## Enhancements

- [ ] Scale window with the buttons +1 and -1.
- [ ] Base refactor
- [ ] Global shadows
- [ ] Ray casting from the camera
  - [ ] Select objects with the mouse
- [ ] Ray tracing support

```

```
