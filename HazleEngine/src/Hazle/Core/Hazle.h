#pragma once
#include <stdio.h>

//=======================================HAZLE CORE======================================================================
#include "Application.h"																								  
#include "core.h"																										  
#include "EntryPoint.h"																									  
#include "Input.h"																										  
#include "KeyCodes.h"																									  
#include "Layer.h"																										  
#include "LayerStack.h"																									  
#include "Log.h"																										  
#include "MouseCodes.h"																									  
#include "Timestep.h"																									  
#include "UUID.h"																										  
#include "Window.h"																										  
//=======================================================================================================================
																														  
//=======================================HAZLE DEBUG=====================================================================
#include "Debug/Instrumentor.h"																							  
//=======================================================================================================================
																														  
//=======================================HAZLE EVENT=====================================================================
#include "Event/ApplicationEvent.h"																						  
#include "Event/Event.h"																								  
#include "Event/KeyEvent.h"																								  
#include "Event/MouseEvent.h"																							  
//=======================================================================================================================
																														  
//=======================================HAZLE IMGUI=====================================================================
#include "imgui.h"																										  
#include "ImGui/ImGuiLayer.h"																							  
//=======================================================================================================================
// 																														  
//========================================HAZLE MATH=====================================================================
#include "Math/Math.h"																									  
//=======================================================================================================================
																														  
//=======================================HAZLE RENDERER==================================================================
#include "Renderer/Buffer.h"																							  
#include "Renderer/Camera.h"																							  
#include "Renderer/EditorCamera.h"																						  
#include "Renderer/FrameBuffer.h"																						  
#include "Renderer/GraphicsContext.h"																					  
#include "Renderer/OrthographicCamera.h"																				  
#include "Renderer/OrthographicCameraController.h"																		  
#include "Renderer/RenderCommand.h"																						  
#include "Renderer/Renderer.h"																							  
#include "Renderer/Renderer2D.h"																						  
#include "Renderer/RendererAPI.h"																						  
#include "Renderer/Shader.h"																							  
#include "Renderer/SubTexture2D.h"																						  
#include "Renderer/Texture.h"																							  
#include "Renderer/VertexArray.h"																						  
//=======================================================================================================================
																														  
																														  
//=========================================HAZLE Scene===================================================================
#include "Scene/Audio.h"																								  
#include "Scene/Component.h"																							  
#include "Scene/Entity.h"																								  
#include "Scene/Scene.h"																								  
#include "Scene/SceneCamera.h"																							  
#include "Scene/SceneSerializer.h"																						  
#include "Scene/ScriptableEntity.h"																						  
//=======================================================================================================================
																														  
																														  
//=======================================HAZLE UTILS=====================================================================
#include "Utils/PlatformUtils.h"																						  
//=======================================================================================================================
																														  
//==========================================GLM==========================================================================
#include <glm/glm.hpp>																									  
#include <glm/gtc/matrix_transform.hpp>																					  
#include <glm/gtc/type_ptr.hpp>																							  
//=======================================================================================================================
																														  
//==============PLATFORM OPENGL==========================================================================================
#include "Platform/OpenGL/OpenGLbuffer.h"																				  
#include "Platform/OpenGL/OpenGLContext.h"																				  
#include "Platform/OpenGL/OpenGLFrameBuffer.h"																			  
#include "Platform/OpenGL/OpenGLRendererAPI.h"																			  
#include "Platform/OpenGL/OpenGLShader.h"																				  
#include "Platform/OpenGL/OpenGLTexture.h"																				  
#include "Platform/OpenGL/OpenGLVertexArray.h"																			  
//=======================================================================================================================
																														  
//==============PLATFORM OPRNGL==========================================================================================
#include "Platform/Windows/WindowsWindow.h"																				  
//=======================================================================================================================