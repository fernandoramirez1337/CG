# CG - Computer Graphics Library

An OpenGL-based Computer Graphics library for creating and manipulating 2D/3D shapes with real-time rendering and interactive transformations.

## Features

- **Math Library**: Vector and matrix operations including:
  - 3D Vector operations (normalization, dot product, cross product)
  - 4x4 Matrix transformations (translation, rotation, scaling, projection)
  - Point representation with homogeneous coordinates

- **Shape Management**: Create and manipulate geometric shapes with:
  - Vertex and index buffer management
  - OpenGL VAO/VBO/EBO integration
  - Transform operations (translation, rotation, scaling)
  - Center-based transformations

- **Shader System**: Multiple shader programs with different colors:
  - Black, White, Red, Green, Blue fragment shaders
  - GLSL 330 core profile

- **Rendering Modes**: Switch between different rendering modes:
  - Triangles (filled)
  - Lines (wireframe)
  - Points

## Dependencies

- **OpenGL 3.3+**
- **GLFW 3.x** - Window management and input handling
- **GLAD** - OpenGL loader
- **CMake 3.10+** - Build system
- **C++17** compatible compiler

## Building

1. Clone the repository:
   ```bash
   git clone https://github.com/fernandoramirez1337/CG.git
   cd CG/mylib
   ```

2. Create a build directory and configure:
   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. Build the project:
   ```bash
   make
   ```

4. Run the executable:
   ```bash
   ./MyGLFWProject
   ```

### Note on GLAD

The project expects GLAD to be in the `mylib/glad/` directory. If not present, generate it from [GLAD Generator](https://glad.dav1d.de/) with:
- Language: C/C++
- Specification: OpenGL
- API: gl Version 3.3
- Profile: Core

## Controls

| Key | Action |
|-----|--------|
| `W` | Move shape up |
| `A` | Move shape left |
| `S` | Move shape down |
| `D` | Move shape right |
| `Q` | Scale down |
| `E` | Scale up |
| `Z` | Rotate counter-clockwise |
| `X` | Rotate clockwise |
| `1` | Triangles render mode (filled) |
| `2` | Lines render mode (wireframe) |
| `3` | Points render mode |
| `I` | Combined: move up + scale up + rotate |
| `K` | Combined: move left + scale down + rotate |
| `L` | Combined: move down + scale up + rotate |
| `O` | Combined: move right + scale down + rotate |
| `Esc` | Close window |

## Project Structure

```
CG/
├── README.md
├── .gitignore
└── mylib/
    ├── CMakeLists.txt
    ├── main.cpp              # Application entry point
    ├── include/
    │   ├── mylib.hpp         # Core math and shape classes
    │   ├── myshapes.hpp      # Predefined shapes and transformations
    │   └── shaders.hpp       # Shader program definitions
    └── src/
        ├── mylib.cpp         # Math and shape implementations
        └── shaders.cpp       # Shader compilation and linking
```

## API Overview

### Core Classes (namespace `ogl`)

- **`vector`**: 3D vector with operations like `length()`, `normalize()`, `dot()`, `cross()`
- **`point`**: 3D point representation
- **`matrix`**: 4x4 transformation matrix base class
- **`scale`**: Scaling transformation matrix
- **`rotate_x/y/z`**: Rotation matrices around each axis
- **`translation`**: Translation transformation matrix
- **`projection`**: Perspective projection matrix
- **`shape`**: Geometry container with OpenGL buffer management

### Shader Programs

Available color shaders accessed via `ShaderProgramID` enum:
- `BLACK`, `WHITE`, `RED`, `GREEN`, `BLUE`

## License

This project is for educational purposes in Computer Graphics.
