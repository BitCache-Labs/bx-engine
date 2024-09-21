#pragma once

#include "bx/editor/view.hpp"

class Toolbar
{
public:
	static void AddView(const View& view);

	static void Initialize();
	static void Shutdown();

	static void Present();

	static void Reset();
	static bool IsPlaying();
	static bool IsPaused();
	static bool ConsumeNextFrame();
};