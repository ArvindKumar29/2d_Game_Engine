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

//================Scene====================
#include "Hazle/Scene/Scene.h"
#include "Hazle/Scene/Component.h"
#include "Hazle/Scene/Entity.h"
//=========================================

//==============Renderer===================
#include "Hazle/Renderer/Renderer.h"
#include "Hazle/Renderer/Renderer2D.h"
#include "Hazle/Renderer/Camera.h"
#include "Hazle/Renderer/OrthographicCamera.h"
#include "Hazle/Renderer/OrthographicCameraController.h"
#include "Hazle/Renderer/Shader.h"
#include "Hazle/Renderer/Buffer.h"
#include "Hazle/Renderer/VertexArray.h"
#include "Hazle/Renderer/Texture.h"
#include "Hazle/Renderer/SubTexture2D.h"
#include "Hazle/Renderer/RenderCommand.h"
#include "Hazle/Renderer/RendererAPI.h"
#include "Hazle/Renderer/GraphicsContext.h"
#include "Hazle/Renderer/FrameBuffer.h"
//=========================================

//==============glm========================
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//=========================================

//==============Platform OpenGL============
#include "Platform/OpenGL/OpenGLShader.h"
//=========================================
