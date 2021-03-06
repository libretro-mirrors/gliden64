#include "GLideN64_mupenplus.h"
#include <stdio.h>

#include "../GLideN64.h"
#include "../OpenGL.h"
#include "../gDP.h"
#include "../Config.h"
#include "../Revision.h"

#ifndef OS_WINDOWS

void initGLFunctions()
{
}
#endif

class OGLVideoMupenPlus : public OGLVideo
{
public:
	OGLVideoMupenPlus() {}

private:
	void _setAttributes();

	virtual bool _start();
	virtual void _stop();
	virtual void _swapBuffers();
	virtual void _saveScreenshot();
	virtual bool _resizeWindow();
	virtual void _changeWindow();
};

OGLVideo & OGLVideo::get()
{
	static OGLVideoMupenPlus video;
	return video;
}

void OGLVideoMupenPlus::_setAttributes()
{
	CoreVideo_GL_SetAttribute(M64P_GL_DOUBLEBUFFER, 1);
	CoreVideo_GL_SetAttribute(M64P_GL_SWAP_CONTROL, config.video.verticalSync);
	CoreVideo_GL_SetAttribute(M64P_GL_BUFFER_SIZE, 32);
	CoreVideo_GL_SetAttribute(M64P_GL_DEPTH_SIZE, 16);
	if (config.video.multisampling > 0 && config.frameBufferEmulation.enable == 0) {
		CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLEBUFFERS, 1);
		if (config.video.multisampling <= 2)
			CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 2);
		else if (config.video.multisampling <= 4)
			CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 4);
		else if (config.video.multisampling <= 8)
			CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 8);
		else
			CoreVideo_GL_SetAttribute(M64P_GL_MULTISAMPLESAMPLES, 16);
	}
}

bool OGLVideoMupenPlus::_start()
{
	CoreVideo_Init();
	_setAttributes();

	m_bFullscreen = config.video.fullscreen > 0;
	m_screenWidth = config.video.windowedWidth;
	m_screenHeight = config.video.windowedHeight;
	_setBufferSize();

	printf("(II) Setting video mode %dx%d...\n", m_screenWidth, m_screenHeight);
	const m64p_video_flags flags = M64VIDEOFLAG_SUPPORT_RESIZING;
	if (CoreVideo_SetVideoMode(m_screenWidth, m_screenHeight, 0, m_bFullscreen ? M64VIDEO_FULLSCREEN : M64VIDEO_WINDOWED, flags) != M64ERR_SUCCESS) {
		printf("(EE) Error setting videomode %dx%d\n", m_screenWidth, m_screenHeight);
		CoreVideo_Quit();
		return false;
	}

	char caption[128];
# ifdef _DEBUG
	sprintf(caption, "%s debug. Revision %s", pluginName, PLUGIN_REVISION);
# else // _DEBUG
	sprintf(caption, "%s. Revision %s", pluginName, PLUGIN_REVISION);
# endif // _DEBUG
	CoreVideo_SetCaption(caption);

	return true;
}

void OGLVideoMupenPlus::_stop()
{
	CoreVideo_Quit();
}

void OGLVideoMupenPlus::_swapBuffers()
{
	// if emulator defined a render callback function, call it before buffer swap
	if (renderCallback != NULL) {
		glUseProgram(0);
		if (config.frameBufferEmulation.N64DepthCompare == 0) {
			glViewport(0, getHeightOffset(), getScreenWidth(), getScreenHeight());
			gSP.changed |= CHANGED_VIEWPORT;
		}
		gDP.changed |= CHANGED_COMBINE;
		(*renderCallback)((gSP.changed&CHANGED_CPU_FB_WRITE) == 0 ? 1 : 0);
	}
	CoreVideo_GL_SwapBuffers();
}

void OGLVideoMupenPlus::_saveScreenshot()
{
}

bool OGLVideoMupenPlus::_resizeWindow()
{
	_setAttributes();

	m_bFullscreen = false;
	m_width = m_screenWidth = m_resizeWidth;
	m_height = m_screenHeight = m_resizeHeight;
	if (CoreVideo_ResizeWindow(m_screenWidth, m_screenHeight) != M64ERR_SUCCESS) {
		printf("(EE) Error setting videomode %dx%d\n", m_screenWidth, m_screenHeight);
		m_width = m_screenWidth = config.video.windowedWidth;
		m_height = m_screenHeight = config.video.windowedHeight;
		CoreVideo_Quit();
		return false;
	}
	return true;
}

void OGLVideoMupenPlus::_changeWindow()
{
	CoreVideo_ToggleFullScreen();
}
