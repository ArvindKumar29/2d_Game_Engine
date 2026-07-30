// ---------------------
//  - Hazle -
// Line Shader
// ---------------------

#type vertex
#version 450 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_VP;

out vec3     v_Position;
out vec4     v_Color;
out flat int v_EntityID;

void main()
{
	v_Position	= a_Position;
	v_Color			= a_Color;
	v_EntityID		= a_EntityID;
	gl_Position		= u_VP * vec4(a_Position, 1.0);
} 

#type fragment
#version 450 core
			
layout(location = 0) out vec4 f_Color;
layout(location = 1) out int f_EntityID;

in vec3		v_Position;
in vec4		v_Color;
in flat int v_EntityID;

uniform sampler2D u_Texture[32];

void main()
{
    f_Color = v_Color;
    f_EntityID = v_EntityID;
}