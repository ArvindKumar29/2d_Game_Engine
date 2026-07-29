// Texure Shader code
#type vertex
#version 450 core
			
layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec2  a_TexCoord;
layout(location = 2) in vec4  a_Color;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int   a_EntityID;

uniform mat4 u_VP;

out vec3     v_Position;
out vec2	 v_TexCoord;
out vec4     v_Color;
out float	 v_TexIndex;
out float	 v_TilingFactor;
out flat int v_EntityID;

void main()
{
	v_Position		= a_Position;
	v_TexCoord		= a_TexCoord;
	v_Color			= a_Color;
	v_TexIndex		= a_TexIndex;
	v_TilingFactor  = a_TilingFactor;
	v_EntityID		= a_EntityID;
	gl_Position		= u_VP * vec4(a_Position, 1.0);
} 

#type fragment
#version 450 core
			
layout(location = 0) out vec4 f_Color;
layout(location = 1) out int f_Color2;

in vec3		v_Position;
in vec2		v_TexCoord;
in vec4		v_Color;
in float	v_TexIndex;
in float	v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Texture[32];

void main()
{
	f_Color		= texture(u_Texture[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
	f_Color2	= v_EntityID; // Placeholder for our entity ID
}