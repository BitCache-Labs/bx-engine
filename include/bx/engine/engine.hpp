#pragma once

#include <bx/core/plugin.hpp>

class Engine
{
public:
	static Plugin GetPlugin()
	{
		Plugin plugin;
		plugin.Initialize = &Initialize;
		plugin.Shutdown = &Shutdown;
		plugin.Reload = &Reload;
		plugin.Tick = &Tick;
		return plugin;
	}

private:
	static bool Initialize();
	static void Shutdown();
	static void Reload();
	static void Tick();
};