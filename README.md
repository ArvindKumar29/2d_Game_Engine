# HazleEngine (2D Game Engine & HazleNut Editor)

![C++](https://img.shields.io/badge/Language-C++20-00599C?style=for-the-badge&logo=c%2B%2B)
![OpenGL](https://img.shields.io/badge/Renderer-OpenGL%204.5-5586A4?style=for-the-badge&logo=opengl)
![Platform](https://img.shields.io/badge/Platform-Windows%20(x64)-0078D6?style=for-the-badge&logo=windows)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**HazleEngine** is a fast, modular 2D game engine and level editor written from scratch in modern C++20. Built on an interactive viewport and Entity Component System (ECS) architecture, it features a custom OpenGL batch renderer, live asset browsing, and a dedicated level editor (**HazleNut**).

---

# ![HazleNut Level Editor Workspace and demo video 720p]
https://github.com/user-attachments/assets/ff4b478c-142d-413d-bf4b-95f2631a7d32

---

## ✨ Current Features

### 🛠️ HazleNut Editor & UI
* **Interactive Viewport & Docking:** Multi-window docking and UI customization powered by **Dear ImGui**.
* **Content Browser Panel:** Visual asset hierarchy with thumbnail rendering, folder navigation, and double-click directory traversal.
* **Drag-and-Drop Workflow:** Seamless drag-and-drop support for textures and scenes directly into the editor viewport and inspector panels.
* **Editor Camera & Gizmos:** Smooth orthographic camera controls combined with **ImGuizmo** for live viewport entity manipulation (Translate, Rotate, Scale).

### 🎨 Rendering & Graphics Pipeline
* **Fast 2D Batch Renderer:** High-performance OpenGL batch renderer for sprites, textures, and primitives to minimize draw calls.
* **Core Rendering Abstractions:** Clean abstraction over OpenGL context setup, Vertex Array Objects (VAOs), Vertex Buffer Objects (VBOs), and Index Buffers (IBOs).
* **Shaders & Textures:** Custom GLSL shader compilation and linking system integrated with dynamic texture binding.
* **Mouse Picking & Raycasting:** Pixel-accurate viewport entity selection using framebuffer color-ID encoding.

### 🧠 Core Architecture & Systems
* **Entity Component System (ECS):** Fast, cache-friendly entity and component management powered by **EnTT**.
* **Scene Serialization:** Complete level saving and loading to human-readable `.yaml` files using **yaml-cpp**.
* **Play-In-Editor (PIE) Mode:** Instant in-memory scene duplication allowing seamless switching between `Editor` (design) and `Runtime` (simulation) states without data loss.
* **Event System:** Custom event dispatcher for low-latency window, mouse, and keyboard input handling.
* **Performance Profiling:** Integrated custom profiling tools for real-time frame generation and debugging metrics.
*  **UUID / GUID Tracking:** 64-bit Universally Unique Identifiers assigned to all active entities and resources to eliminate filepath dependency conflicts.
*  **2D Physics Simulation:** Integrating **Box2D** for real-time rigidbody dynamics, gravity, friction, and collision triggers within the runtime loop.
*  **Audio Engine:** Audio integration for sound effects and ambient soundtracks using FMOD.

---

## 🗺️ Planned Features
- [ ] **SPIR-V Shader Overhaul:** Transitioning from text-based OpenGL shaders to precompiled binary bytecode using `shaderc` and `SPIRV-Cross` for automated reflection.

---

## 📦 Architecture & Vendor Libraries

* **[GLFW](https://www.glfw.org/):** Window creation, context switching, and input handling.
* **[Glad](https://glad.dav1d.de/):** OpenGL specification loading.
* **[Dear ImGui](https://github.com/ocornut/imgui) & [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo):** Immediate-mode editor UI and viewport transformation gizmos.
* **[EnTT](https://github.com/skypjack/entt):** Fast Entity Component System.
* **[yaml-cpp](https://github.com/jbeder/yaml-cpp):** YAML data serialization and deserialization.
* **[GLM](https://github.com/g-truc/glm):** Mathematics library for graphics software based on GLSL specifications.
* **[stb_image](https://github.com/nothings/stb):** Single-header image decoding and texture loading.
* **[Box2D](https://github.com/thecherno/box2d/):** 2D Physics simulation engine.
* **[FMOD](https://www.fmod.com/):** Audio Integration to use audio files in engine and games.

---

## 🚀 Getting Started

### Prerequisites
* **Windows 10 / 11 (64-bit)**
* **Visual Studio 2022 or later** (with *Desktop development with C++* workload installed)
* **Git**

### Cloning & Building

1. **Clone the repository along with its submodules:**
   ```bash
   git clone --recursive https://github.com/ArvindKumar29/2d_Game_Engine.git
   cd 2d_Game_Engine
