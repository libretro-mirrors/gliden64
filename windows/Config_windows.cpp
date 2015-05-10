#include "GLideN64_Windows.h"
#include "../N64.h"
#include "../Config.h"
#include "../RSP.h"
#include "../PluginAPI.h"
#include "../OpenGL.h"
#include "../GLideNUI/GLideNUI.h"

Config config;

void Config_DoConfig(/*HWND hParent*/)
{
	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strIniFolderPath);

	ÑonfigOpen = true;
	const bool bRestart = RunConfig(strIniFolderPath);
	if (config.generalEmulation.enableCustomSettings != 0)
		LoadCustomRomSettings(strIniFolderPath, RSP.romname);
	if (bRestart)
		video().restart();
	ÑonfigOpen = false;
}

void Config_LoadConfig()
{
	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strIniFolderPath);
	LoadConfig(strIniFolderPath);
	if (config.generalEmulation.enableCustomSettings != 0)
		LoadCustomRomSettings(strIniFolderPath, RSP.romname);
}
