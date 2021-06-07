#define GLM_FORCE_RADIANS 
#include "HorizonFrame.h"

int main()
{
	HorizonFrame* frame = HorizonFrame::getInstance();

	frame->FrameInit();
	frame->FrameLoop();
	frame->FrameFinalize();
}

