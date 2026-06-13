#pragma once
#include <stdio.h>

//-----------HazleCore----------------
#include "Application.h"
#include "Layer.h"
#include "Log.h"
#include "Input.h"
#include "core.h"
//-------------------------------------

//-------------IMGUI-------------------
#include "imgui.h"
#include "Hazle/ImGui/ImGuiLayer.h"
//-------------------------------------

//==============Renderer===================
#include "Renderer/Renderer.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/GraphicsContext.h"
//=========================================

//==============glm========================
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//=========================================

// To be used by Hazle applications

//=============Entry Point=================
#include "Hazle/EntryPoint.h"
//=========================================

//==============Platform OpenGL============
#include "Platform/OpenGL/OpenGLShader.h"
//=========================================
