<h1 align="center">Hazle Engine ⚙️</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue.svg" alt="C++17">
  <img src="https://img.shields.io/badge/OpenGL-3.3+-green.svg" alt="OpenGL">
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey.svg" alt="Windows">
  <img src="https://img.shields.io/badge/Build-Premake5-orange.svg" alt="Premake">
</p>

> A high-performance 2D Game Engine built from scratch using C++ and OpenGL.

## 📸 Screenshots:
#2D: Orthographic Camera
<img width="1442" height="952" alt="Orthographic Camera 2D view" src="https://github.com/user-attachments/assets/d2837a58-7626-4ab3-a2d0-cb8476d99304" />
#3D: Perspective Camera
<img width="1442" height="952" alt="Perspective Camera 3D view" src="https://github.com/user-attachments/assets/602417ae-777e-49bd-93a2-ce775a12bf5a" />
check other media files in the actual repo

## ✨ Current Features
* ***Core Architecture:*** Custom application layer and window abstraction.
* ***Rendering Pipeline:*** OpenGL context setup, Vertex Array Objects (VAOs), Vertex Buffer Objects (VBOs), and Index Buffers (IBOs).
* ***Shaders and Textures:*** Custom shader compilation and linking system (GLSL) with custom texture integration.
* ***UI Integration:*** Fully integrated **ImGui** for live debugging and developer tools.
* ***Event System:*** Custom event dispatcher for handling window, mouse, and keyboard inputs.
* ***Performance profiling:*** It provides custom profiling tools for developers.
* ***Fast 2D Renderer:*** A 2D renderer API for fast frame generation.
* ***OpenGl API:*** 2D renderer completely written in OpenGL/Glad to support multiple platforms.
* ***HazleNut:*** The level editor of hazle to create and modify different games .
* ***ImGui Docking:*** Docking system to dock and customize the viewport, file explorer, menu, settings etc. tabs to hazle editor.
* ***ECS:*** It has support of modern ECS architecture and uses EnTT library for that.

## 🚀 Getting Started

### Prerequisites
* Visual Studio 2026 (with Desktop development with C++)
* Git

### Building the Engine
We use **Premake5** to generate the project files. 

1. Clone the repository:
   ```bash
   git clone [https://github.com/ArvindKumar29/2d_Game_Engine.git]


   cd 2d_Game_Engine
