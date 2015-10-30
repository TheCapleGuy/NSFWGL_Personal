#include <render.h>



class CompositePass : public nsfw::RenderPass
{
public:
	void prep();

	void draw();

	void post();
};