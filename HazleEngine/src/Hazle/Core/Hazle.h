#pragma once
#include <stdio.h>

//=======================================HAZLE CORE======================================================================
#include "Application.h"																								  
#include "core.h"																										  
#include "Input.h"																										  
#include "KeyCodes.h"																									  
#include "Layer.h"																										  
#include "LayerStack.h"																									  
#include "Log.h"																										  
#include "MouseCodes.h"																									  
#include "Timestep.h"																									  
#include "UUID.h"																										  
#include "Window.h"																										  //=======================================================================================================================
																														  
//=======================================HAZLE DEBUG=====================================================================
#include "Hazle/Debug/Instrumentor.h"																							  
//=======================================================================================================================
																														  
//=======================================HAZLE EVENT=====================================================================
#include "Hazle/Event/ApplicationEvent.h"																						  
#include "Hazle/Event/Event.h"																								  
#include "Hazle/Event/KeyEvent.h"																								  
#include "Hazle/Event/MouseEvent.h"																							  
//=======================================================================================================================
																														  
//=======================================HAZLE IMGUI=====================================================================
#include "imgui.h"																										  
#include "Hazle/ImGui/ImGuiLayer.h"																							  
//=======================================================================================================================
// 																														  
//========================================HAZLE MATH=====================================================================
#include "Hazle/Math/Math.h"																									  
//=======================================================================================================================
																														  
//=======================================HAZLE RENDERER==================================================================
#include "Hazle/Renderer/Buffer.h"																							  
#include "Hazle/Renderer/Camera.h"																							  
#include "Hazle/Renderer/EditorCamera.h"																						  
#include "Hazle/Renderer/FrameBuffer.h"																						  
#include "Hazle/Renderer/GraphicsContext.h"																					  
#include "Hazle/Renderer/OrthographicCamera.h"																				  
#include "Hazle/Renderer/OrthographicCameraController.h"																		  
#include "Hazle/Renderer/RenderCommand.h"																						  
#include "Hazle/Renderer/Renderer.h"																							  
#include "Hazle/Renderer/Renderer2D.h"																						  
#include "Hazle/Renderer/RendererAPI.h"																						  
#include "Hazle/Renderer/Shader.h"																							
#include "Hazle/Renderer/SubTexture2D.h"																						  
#include "Hazle/Renderer/Texture.h"																							  
#include "Hazle/Renderer/VertexArray.h"																						  
//=======================================================================================================================
																														  
																														  
//=========================================HAZLE Scene===================================================================
#include "Hazle/Scene/Audio.h"																								  
#include "Hazle/Scene/Component.h"																							  
#include "Hazle/Scene/Entity.h"																								  
#include "Hazle/Scene/Scene.h"																								  
#include "Hazle/Scene/SceneCamera.h"																							  
#include "Hazle/Scene/SceneSerializer.h"																						  
#include "Hazle/Scene/ScriptableEntity.h"																						  
//=======================================================================================================================
																														  
																														  
//=======================================HAZLE UTILS=====================================================================
#include "Hazle/Utils/PlatformUtils.h"																						  
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