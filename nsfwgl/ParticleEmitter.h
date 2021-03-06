#pragma once

namespace nsfw
{
struct ParticleEmitter
{
	ParticleEmitter();
	~ParticleEmitter();

	struct Particle
	{
		glm::vec3 position, velocity;
		float lifeTime, lifeSpan;
		Particle() : lifeTime(1), lifeSpan(0) {};
		void update();
	};

	struct ParticleVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};


	Particle* mParticles; 
	ParticleVertex* mVertexData;
	unsigned mMaxParticles;

	glm::vec3 mPosition;
	glm::vec4 mStartColor, mEndColor;

	float mLifeSpanMin, mLifeSpanMax;
	float mVelocityMin, mVelocityMax;
	float mStartSize, mEndSize;
	float time;

	unsigned mActiveBuffer;//, mUpdateShader, mDrawShader;
	unsigned int mVAO[2];
	unsigned int mVBO[2];
	unsigned int mDrawShader;
	unsigned int mUpdateShader;
	float mLastDrawTime;

	//nsfw::Asset<ASSET::SHADER> update;
	//nsfw::Asset<ASSET::SHADER> draw;
	//nsfw::Asset<ASSET::VAO> vao[2];
	//nsfw::Asset<ASSET::VBO> vbo[2];



	void Init(
		unsigned aMaxParticles,
		float aLifeSpanMin, float aLifeSpanMax,
		float aVelocityMin, float aVelocityMax,
		float aStartSize,   float aEndSize,
		glm::vec4 const& aStartColor, glm::vec4 const& aEndColor);

	void emit();
	void update();
	void draw(float time, const glm::mat4 & a_cameraTransform, glm::mat4 & a_projectionView);
	void draw();
	void CreateBuffers();
	void CreateUpdateShader();
	unsigned int LoadShader(unsigned int type, const char * path);
	void CreateDrawShader();
};

};