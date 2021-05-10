#include "HorizonFrame.h"

int main()
{
	HorizonFrame* frame = HorizonFrame::getInstance();

	frame->FrameInit();
	frame->FrameLoop();
	frame->FrameFinalize();
}

