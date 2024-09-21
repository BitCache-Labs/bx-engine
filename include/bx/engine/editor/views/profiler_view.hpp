#pragma once

#include "bx/editor/view.hpp"

class ProfilerView
{
public:
	static View GetView()
	{
		View view;
		view.Initialize = &Initialize;
		view.Shutdown = &Shutdown;
		view.OnReload = &OnReload;
		view.OnPlay = &OnPlay;
		view.OnStop = &OnStop;
		view.OnPause = &OnPause;
		view.OnToolbar = &OnToolbar;
		view.OnGui = &OnGui;
		view.isShown = false;
		return view;
	}

	static bool Initialize(bool& isShown);
	static void Shutdown(bool& isShown);

	static void OnReload();
	static void OnPlay();
	static void OnStop();
	static void OnPause(bool paused);

	static void OnToolbar(bool& isShown);
	static void OnGui(bool& isShown);
};