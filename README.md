# engine3d

A small C++/OpenGL learning project built to explore the basics of 3D rendering:

- Loading a 3D model from a Wavefront OBJ file.
- Compiling GLSL vertex and fragment shaders and linking them into a program.
- Loading a BMP image and applying it as a texture using UV coordinates.
- Rendering a scene with a movable camera.

This is an old experiment, not a general-purpose engine. No further development is planned; it is kept as a reference for those experiments.

## Build on macOS

The Makefile targets macOS and uses Apple's OpenGL framework. It resolves Homebrew dependency paths automatically on both Apple Silicon and Intel Macs.

### Requirements

- Apple's Xcode Command Line Tools (`clang++` and `make`). If they are not installed, run `xcode-select --install` and complete the installer.
- Homebrew.
- GLFW for window creation, the OpenGL context, and input.
- GLM for vector and matrix operations.

A separate Homebrew LLVM installation is not required.

From the project root:

```sh
brew install glfw glm
make
./build/game
```

`make` creates the `build/` directory automatically and produces `build/game`.

**Run from the project root**, not from inside `build/`: the application loads models, shaders, and textures using relative paths.

To remove the compiled objects and executable:

```sh
make clean
```

## Demo and controls

The demo loads a floor and a car model, then renders ten copies of the car. It loads the wood BMP texture and uses separate shader programs for the floor and cars. The window requests an OpenGL 3.2 core context.

| Input | Action |
| --- | --- |
| Mouse | Look around |
| W / S | Move forward / backward |
| A / D | Move left / right |
| Space / Left Shift | Move up / down relative to the camera |
| Escape | Close the application |

The mouse cursor is captured while the demo is running.

## Code layout

- `main.cpp`: window and input setup, texture loading, camera, and rendering loop.
- `Model3D.cpp` / `Model3D.h`: OBJ parsing, model transforms, and vertex-buffer uploads.
- `Shader.cpp` / `Shader.h`: shader compilation and program linking.
- `File.cpp` / `File.h`: reading files into memory.
- `models/`: sample OBJ models and related assets.
- `shaders/`: GLSL vertex and fragment shaders.
- `textures/`: the BMP texture used by the demo.

## Scope and limitations

The asset loaders are deliberately small and tailored to the bundled files. The OBJ loader expects triangulated faces with positive `vertex/texture/normal` indices; it is not a complete OBJ or material loader. Texture loading is a minimal BMP implementation, not a general image decoder.

The current build configuration is macOS-specific. OpenGL is deprecated on macOS; this project preserves the original rendering approach rather than migrating to a newer API.
