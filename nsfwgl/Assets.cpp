#include <inc\gl_core_4_4.h>
#include "nsfw.h"
#include <iostream>
#include <fstream>

using namespace nsfw::ASSET;

const char *nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif


nsfw::GL_HANDLE nsfw::Assets::getVERIFIED(const AssetKey &key) const
{
#ifdef _DEBUG
			if (!handles.count(key))
			{
				std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
				return 0;
			}
#endif
			return handles.at(key);
}

bool nsfw::Assets::setINTERNAL(ASSET::GL_HANDLE_TYPE t,const char *name, GL_HANDLE handle)
{
	AssetKey key(t, name);
#ifdef _DEBUG
	if (handles.count(key))
	{
		std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
		return false;
	}
	else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
	handles[key] = handle;
	return true;
}


bool nsfw::Assets::makeVAO(const char * name, const struct Vertex *verts, unsigned vsize,  const unsigned * tris, unsigned tsize)
{
	ASSET_LOG(GL_HANDLE_TYPE::VBO);
	ASSET_LOG(GL_HANDLE_TYPE::IBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);

	unsigned int vbo, ibo, vao, size = tsize;
	

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned), tris, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1); //noraml
	glEnableVertexAttribArray(2); //tangent
	glEnableVertexAttribArray(3); // texCoord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,  sizeof(Vertex), (void*)Vertex::NORMAL);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,  sizeof(Vertex), (void*)Vertex::TANGENT);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::TEXCOORD);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	setINTERNAL(GL_HANDLE_TYPE::VBO,  name, vbo);
	setINTERNAL(GL_HANDLE_TYPE::IBO,  name, ibo);
	setINTERNAL(GL_HANDLE_TYPE::VAO,  name, vao);
	setINTERNAL(GL_HANDLE_TYPE::SIZE, name, size);

	TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");
	return false;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	int nColors = 0;

	for (int i = 0; i < nTextures; i++)
	{
		makeTexture(names[i], w, h, depths[i]);

		unsigned attachment;
		if      (depths[i] == GL_DEPTH_COMPONENT)    attachment = GL_DEPTH_ATTACHMENT;
		else if (depths[i] == GL_STENCIL_ATTACHMENT) attachment = GL_STENCIL_ATTACHMENT;
		else		/*is a color!*/					 attachment = GL_COLOR_ATTACHMENT0 + nColors++;


		glFramebufferTexture(GL_FRAMEBUFFER, attachment, get<TEXTURE>(names[i]), 0);
	}

	unsigned *colorAttachments = new unsigned[nColors];
	for (int i = 0; i < nColors; i++) colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(nColors, colorAttachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setINTERNAL(GL_HANDLE_TYPE::FBO, name, fbo);
	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
{
	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	setINTERNAL(GL_HANDLE_TYPE::TEXTURE, name, texture);
	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	return false;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	GLuint shader = glCreateProgram();

	std::ifstream vin(vpath);

	std::string vcontents = std::string(
		std::istreambuf_iterator<char>(vin),
		std::istreambuf_iterator<char>());

	std::ifstream fin(fpath);
	std::string fcontents = std::string(
		std::istreambuf_iterator<char>(fin),
		std::istreambuf_iterator<char>());
	

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, (const char**)vcontents.c_str(), 0);
	glShaderSource(fshader, 1, (const char**)fcontents.c_str(), 0);

	glCompileShader(vshader);
	glCompileShader(fshader);

	glAttachShader(shader, vshader);
	glAttachShader(shader, fshader);

	glLinkProgram(shader);
	
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	
	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);
	return true;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	//name/meshName
	//name/textureName
	TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");
	return false;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	return false;
}

void nsfw::Assets::init()
{
	setINTERNAL(FBO, "Screen", 0);
	
	makeVAO("Cube", CubeVerts,24, CubeTris,36);
	makeVAO("Quad", QuadVerts, 4, QuadTris, 6);
	
	char w[] = { 255, 0,255,255 };
	makeTexture("Magenta", 1, 1, GL_RGBA, w);

}

void nsfw::Assets::term()
{

	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		GLuint handle = k.second;

		switch (k.first.first)
		{
		case VBO:     glDeleteBuffers(1, &handle);		break;
		case IBO:     glDeleteBuffers(1, &handle);		break;
		case VAO:     glDeleteVertexArrays(1, &handle);	break;
		case SHADER:  glDeleteProgram(handle);		    break;
		case TEXTURE: glDeleteTextures(1, &handle);     break;
		case FBO:     glDeleteFramebuffers(1, &handle); break;
		}
	}
}


