#include <vector>
#include <cstring>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "shader.h"
#include "texture.h"

#include "text2D.h"

unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;

void initText2D(OpenGLContext context, const char * texturePath){

	// Initialize texture
	Text2DTextureID = loadDDS(texturePath);

	// Initialize VBO
	context.extensions.glGenBuffers(1, &Text2DVertexBufferID);
	context.extensions.glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	Text2DShaderID = LoadShaders( "TextVertexShader.vertexshader", "TextVertexShader.fragmentshader" );

	// Initialize uniforms' IDs
	Text2DUniformID = context.extensions.glGetUniformLocation( Text2DShaderID, "myTextureSampler" );

}

void printText2D(OpenGLContext context, const char * text, int x, int y, int size){

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	context.extensions.glUseProgram(Text2DShaderID);

	// Bind texture
	context.extensions.glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	context.extensions.glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	context.extensions.glEnableVertexAttribArray(0);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	context.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	context.extensions.glEnableVertexAttribArray(1);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	context.extensions.glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glDisable(GL_BLEND);

	context.extensions.glDisableVertexAttribArray(0);
	context.extensions.glDisableVertexAttribArray(1);

}

void cleanupText2D(OpenGLContext context){

	// Delete buffers
	context.extensions.glDeleteBuffers(1, &Text2DVertexBufferID);
	context.extensions.glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	// Delete shader
	context.extensions.glDeleteProgram(Text2DShaderID);
}
