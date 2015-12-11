//#include "ParticleEmitter.h"
#include "nsfw.h"
#include <math.h>

nsfw::ParticleEmitter::ParticleEmitter() :
	mParticles(nullptr), mMaxParticles(0),
	mPosition(0,0,0), time(0), mActiveBuffer(0)
{
	mVAO[0] = 0;
	mVAO[1] = 0;
	mVBO[0] = 0;
	mVBO[1] = 0;
}

nsfw::ParticleEmitter::~ParticleEmitter()
{
	delete[] mParticles;
	//delete[] mVertexData;

}

void nsfw::ParticleEmitter::Init(
	unsigned aMaxParticles, 
	float aLifeSpanMin, float aLifeSpanMax, 
	float aVelocityMin, float aVelocityMax, 
	float aStartSize, float aEndSize, 
	glm::vec4 const & aStartColor, glm::vec4 const & aEndColor)
{
	mMaxParticles = aMaxParticles;
	mLifeSpanMin  = aLifeSpanMin;
	mLifeSpanMax  = aLifeSpanMax;
	mVelocityMin  = aVelocityMin;
	mVelocityMax  = aVelocityMax;
	mStartSize    = aStartSize;
	mEndSize      = aEndSize;
	mStartColor   = aStartColor;
	mEndColor     = aEndColor;

	mParticles = new Particle[mMaxParticles];

	CreateBuffers();

	CreateUpdateShader();

	CreateDrawShader();

	//mUpdateShader = nsfw::Assets::instance().get<nsfw::ASSET::SHADER>("ParticleUpdate");
	//mDrawShader   = nsfw::Assets::instance().get<nsfw::ASSET::SHADER>("ParticleDraw");
}

void nsfw::ParticleEmitter::Particle::update()
{

}

void nsfw::ParticleEmitter::emit()
{

}

void nsfw::ParticleEmitter::update()
{
	float dTime = nsfw::Window::instance().getTime();
	//mPosition.x = cos(dTime);
	time += dTime;
	emit();
}

void nsfw::ParticleEmitter::draw(float time, const glm::mat4 & a_cameraTransform, glm::mat4 & a_projectionView)
{
	glUseProgram(mUpdateShader);
	//debug need to implement delta time
	float dTime = .01f;
	//bind time info
	int location = glGetUniformLocation(mUpdateShader, "time");
	glUniform1f(location, time);
	float deltaTime = time - mLastDrawTime;
	mLastDrawTime = time;
	location = glGetUniformLocation(mUpdateShader, "deltaTime");
	glUniform1f(location, deltaTime);

	//bind emitters position
	location = glGetUniformLocation(mUpdateShader, "emitterPosition");
	glUniform3fv(location, 1, &mPosition[0]);


	/*
	setUniform("time", nsfw::UNIFORM::FLO1, time);

	setUniform("deltaTime", nsfw::UNIFORM::FLO1, &dTime);
	setUniform("lifeMin", nsfw::UNIFORM::FLO1, &pEmit->mLifeSpanMin);
	setUniform("lifeMax", nsfw::UNIFORM::FLO1, &pEmit->mLifeSpanMax);
	setUniform("sizeStart", nsfw::UNIFORM::FLO1, &pEmit->mStartSize);
	setUniform("sizeEnd", nsfw::UNIFORM::FLO1, &pEmit->mEndSize);
	setUniform("colorStart", nsfw::UNIFORM::FLO3, &pEmit->mStartColor);
	setUniform("colorEnd", nsfw::UNIFORM::FLO3, &pEmit->mEndColor);
	setUniform("emitterPosition", nsfw::UNIFORM::FLO3, &pEmit->mPosition);
	*/
	glEnable(GL_RASTERIZER_DISCARD);

	glBindVertexArray(mVAO[mActiveBuffer]);

	unsigned otherBuffer = (mActiveBuffer + 1) % 2;

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mVBO[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, mMaxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);


	glUseProgram(mDrawShader);

	location = glGetUniformLocation(mDrawShader, "projectionView");
	glUniformMatrix4fv(location, 1, false, &a_projectionView[0][0]);

	location = glGetUniformLocation(mDrawShader, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_cameraTransform[0][0]);

	/*
	setUniform("projectionView", nsfw::UNIFORM::MAT4, glm::value_ptr(c.getView()));
	setUniform("cameraTransform", nsfw::UNIFORM::MAT4, glm::value_ptr(c.m_transform));
	*/
	glBindVertexArray(mVAO[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, mMaxParticles);

	mActiveBuffer = otherBuffer;
}

void  nsfw::ParticleEmitter::CreateBuffers()
{
	glGenVertexArrays(2, mVAO);
	glGenBuffers(2, mVBO);

	glBindVertexArray(mVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(Particle), mParticles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//velocity
	glEnableVertexAttribArray(2);//lifetime
	glEnableVertexAttribArray(3);//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	glBindVertexArray(mVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(Particle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//velocity
	glEnableVertexAttribArray(2);//lifetime
	glEnableVertexAttribArray(3);//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void nsfw::ParticleEmitter::CreateUpdateShader()
{

	//create a shader
	unsigned int vertexShader = LoadShader(GL_VERTEX_SHADER, "../resources/shaders/gpuParticleUpdate.vert");
	mUpdateShader = glCreateProgram();
	glAttachShader(mUpdateShader, vertexShader);

	//specify the data will stream back
	const char* varyings[] = { "position", "velocity", "lifetime", "lifeSpan" };
	glTransformFeedbackVaryings(mUpdateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(mUpdateShader);

	//remove unneeded handles
	glDeleteShader(vertexShader);
	int success = GL_FALSE;
	glGetProgramiv(mUpdateShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(mUpdateShader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(mUpdateShader, length, 0, log);
		std::cout << "Error linking shader program.\n" << log << std::endl;
		delete[] log;
		assert(false);
	}

	//bind the shader so can set some uniforms that dont change per frame
	glUseProgram(mUpdateShader);

	//bind lifetime min / max
	int location = glGetUniformLocation(mUpdateShader, "lifeMin");
	glUniform1f(location, mLifeSpanMin);
	location = glGetUniformLocation(mUpdateShader, "lifeMax");
	glUniform1f(location, mLifeSpanMax);
}

unsigned int nsfw::ParticleEmitter::LoadShader(unsigned int type, const char * path)
{
	FILE* file = fopen(path, "rb");
	if (file == nullptr)
	{
		assert(false);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
		assert(false);
	}
	delete[] source;

	return shader;
}

void nsfw::ParticleEmitter::CreateDrawShader()
{
	/*
	"../resources/shaders/gpuParticle.vert",
	"../resources/shaders/gpuParticle.geom",
	"../resources/shaders/gpuParticle.frag");*/

	//a.loadShader("ParticleUpdate", "../resources/shaders/gpuParticleUpdate.vert"
	unsigned int vertexShader = LoadShader(GL_VERTEX_SHADER, "../resources/shaders/gpuParticle.vert");
	unsigned int geometryShader = LoadShader(GL_GEOMETRY_SHADER, "../resources/shaders/gpuParticle.geom");
	unsigned int fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "../resources/shaders/gpuParticle.frag");

	mDrawShader = glCreateProgram();
	glAttachShader(mDrawShader, vertexShader);
	glAttachShader(mDrawShader, fragmentShader);
	glAttachShader(mDrawShader, geometryShader);
	glLinkProgram(mDrawShader);

	int success = GL_FALSE;
	glGetProgramiv(mDrawShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(mDrawShader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(mDrawShader, length, 0, log);
		assert(false && "Error linking shader:" && log);
		delete[] log;
	}

	//remove unneeded handles
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//bind the shader so that we can set some uniforms that don't change per-frame
	glUseProgram(mDrawShader);

	//bind size info for lerp that wont change
	int location = glGetUniformLocation(mDrawShader, "sizeStart");
	glUniform1f(location, mStartSize);
	location = glGetUniformLocation(mDrawShader, "sizeEnd");
	glUniform1f(location, mEndSize);

	//bind colorinfo for lerp that wont change
	location = glGetUniformLocation(mDrawShader, "colorStart");
	glUniform4fv(location, 1, &mStartColor[0]);
	location = glGetUniformLocation(mDrawShader, "colorEnd");
	glUniform4fv(location, 1, &mEndColor[0]);
}
