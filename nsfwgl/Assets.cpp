#include <ogl\gl_core_4_4.h>
#include "nsfw.h"
#include "ParticleEmitter.h"
#include <iostream>
#include <fstream>
#include <fbx\FBXFile.h>
#include <stb\stb_image.h>

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

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // normal
	glEnableVertexAttribArray(2); // tangent
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

	return true;
}

bool nsfw::Assets::makeVAO(const char * name, struct ParticleEmitter *partemit)
{
	ASSET_LOG(GL_HANDLE_TYPE::VBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);

	unsigned vao[2], vbo[2];

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	//    first buffer
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, partemit->mMaxParticles * sizeof(ParticleEmitter::Particle), partemit->mParticles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), 0);               //position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 12); //velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 24); //lifetime
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 28); //lifespan
	//    first buffer

	//    second buffer
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, partemit->mMaxParticles * sizeof(ParticleEmitter::Particle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), 0);               //position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 12); //velocity
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 24); //lifetime
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleEmitter::Particle), ((char*)0) + 28); //lifespan
	//   second buffer

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//first buffer
	std::string name1 = name; name1 += '0';
	std::string name2 = name; name2 += '1';

	setINTERNAL(GL_HANDLE_TYPE::VBO, name1.c_str(), vbo[0]);
	setINTERNAL(GL_HANDLE_TYPE::VAO, name1.c_str(), vao[0]);
	//second buffer
	setINTERNAL(GL_HANDLE_TYPE::VBO, name2.c_str(), vbo[1]);
	setINTERNAL(GL_HANDLE_TYPE::VAO, name2.c_str(), vao[1]);

	partemit->vao[0] = name1.c_str();
	partemit->vbo[0] = name1.c_str();

	partemit->vao[1] = name2.c_str();
	partemit->vbo[1] = name2.c_str();
	

	return true;
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

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const unsigned char *pixels)
{
	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	setINTERNAL(GL_HANDLE_TYPE::TEXTURE, name, texture);
	return true;
}

unsigned int nsfw::Assets::loadTexture(const char * name, const char * path)
{
	int w, h, d;
	unsigned char *p;

	p = stbi_load(path, &w, &h, &d, STBI_default);

	switch (d)
	{
	case 1: d = GL_RED;   break;
	case 2: d = GL_RG;    break;
	case 3: d = GL_RGB;	  break;
	case 4: d = GL_RGBA;  break;
	}

	makeTexture(name, w,h,d,p);
	stbi_image_free(p);

	return true;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath)
{
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	GLuint shader = glCreateProgram();

	std::ifstream vin(vpath);

	std::string vcontents = std::string(
		std::istreambuf_iterator<char>(vin),
		std::istreambuf_iterator<char>());

	const char *vs = vcontents.c_str();

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vshader, 1, &vs, 0);

	glCompileShader(vshader);

	glAttachShader(shader, vshader);

	glLinkProgram(shader);

	glDeleteShader(vshader);

	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);
	return true;
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

	const char *vs = vcontents.c_str();
	const char *fs = fcontents.c_str();

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, &vs, 0);
	glShaderSource(fshader, 1, &fs, 0);

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

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * gpath, const char * fpath)
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

	std::ifstream gin(gpath);

	std::string gcontents = std::string(
		std::istreambuf_iterator<char>(gin),
		std::istreambuf_iterator<char>());

	const char *vs = vcontents.c_str();
	const char *fs = fcontents.c_str();
	const char *gs = gcontents.c_str();

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gshader = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(vshader, 1, &vs, 0);
	glShaderSource(fshader, 1, &fs, 0);
	glShaderSource(gshader, 1, &gs, 0);

	glCompileShader(vshader);
	glCompileShader(fshader);
	glCompileShader(gshader);

	glAttachShader(shader, vshader);
	glAttachShader(shader, fshader);
	glAttachShader(shader, gshader);

	glLinkProgram(shader);

	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteShader(gshader);

	setINTERNAL(GL_HANDLE_TYPE::SHADER, name, shader);

	return true;
}

bool nsfw::Assets::loadUpdateShader(const char * name, const char *vpath, const char * VarNames[], const int numVars)
{
	unsigned vs = loadShader(name, vpath);
	//create program?? do that here? or before calling function?
	//function doesnt need to know about the particle emitter. 
	//function meant to update the shader before calling draw

	glTransformFeedbackVaryings(get(SHADER,name), numVars, VarNames, GL_INTERLEAVED_ATTRIBS);
	
	return false;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	FBXFile file;
	
	file.load(path, FBXFile::UNITS_METER);

	for (int i = 0; i < file.getMeshCount(); i++)
	{
		unsigned tsize, vsize;
		unsigned *tris;
		Vertex *verts;
		auto m = file.getMeshByIndex(i);

		verts = new Vertex[vsize = m->m_vertices.size()];
		tris = new unsigned[tsize = m->m_indices.size()];

		for (int i = 0; i < tsize; i++)tris[i] = m->m_indices[i];
		
		for (int i = 0; i < vsize; i++)
		{
			verts[i].position = m->m_vertices[i].position;
			verts[i].normal = m->m_vertices[i].normal;
			verts[i].tangent = m->m_vertices[i].tangent;
			verts[i].texCoord = m->m_vertices[i].texCoord1;
		}
		
		std::string n = name; n += m->m_name;
		makeVAO(n.c_str(), verts, vsize, tris, tsize);

		delete[]verts;
		delete[]tris;
	}


	for (int i = 0; i < file.getTextureCount(); i++)
	{
		auto t = file.getTextureByIndex(i);

		std::string n = name; n += t->name;
		loadTexture(n.c_str(), t->path.c_str());
	}

	file.unload();
	return true;
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

	unsigned char w[] = { 100, 0, 100,255 };
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


  