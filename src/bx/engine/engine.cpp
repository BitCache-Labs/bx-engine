#include "bx/engine/engine.hpp"

#include "bx/engine/data.hpp"
#include "bx/engine/resource.hpp"
#include "bx/engine/script.hpp"

#include <bx/core/plugin.hpp>
#include <bx/core/memory.hpp>
#include <bx/core/log.hpp>
#include <bx/core/time.hpp>
#include <bx/core/profiler.hpp>
#include <bx/core/runtime.hpp>

#include <bx/platform/file.hpp>
#include <bx/platform/input.hpp>
#include <bx/platform/window.hpp>
#include <bx/platform/graphics.hpp>
#include <bx/platform/audio.hpp>
#include <bx/platform/imgui.hpp>

#ifdef BX_EDITOR_BUILD
#include <bx/engine/editor/assets.hpp>
#include <bx/engine/editor/toolbar.hpp>
#include <bx/engine/editor/views/profiler_view.hpp>
#include <bx/engine/editor/views/data_view.hpp>
#include <bx/engine/editor/views/assets_view.hpp>
#include <bx/engine/editor/views/settings_view.hpp>
#include <bx/engine/editor/views/console_view.hpp>
#endif

bool Engine::Initialize()
{
	// Initialze core
#ifdef MEMORY_CUSTOM_CONTAINERS
	Memory::Initialize();
#endif

	Time::Initialize();
	File::Initialize();
	Data::Initialize();
	ResourceManager::Initialize();
	Script::Initialize();
	Window::Initialize();
	Input::Initialize();
	Graphics::Initialize();
	ImGuiImpl::Initialize();
	Audio::Initialize();

#ifdef BX_EDITOR_BUILD
	AssetManager::Initialize();

	Toolbar::AddView(ProfilerView::GetView());
	Toolbar::Initialize();

	String scenePath = Data::GetString("Current Scene", "", DataTarget::EDITOR);
	if (!scenePath.empty())
	{
		scenePath = Data::GetString("Main Scene", "[assets]/main.scene", DataTarget::GAME);
		Data::SetString("Current Scene", scenePath, DataTarget::EDITOR);
	}

	Reload();
	//Scene::Load(scenePath);
#else
	Window::SetCursorMode(CursorMode::DISABLED);

	const String& mainScene = Data::GetString("Main Scene", "[assets]/main.scene", DataTarget::GAME);
	//Scene::Load(mainScene);
#endif

	return true;
}

void Engine::Shutdown()
{
#ifdef BX_EDITOR_BUILD
	Toolbar::Shutdown();
	AssetManager::Shutdown();
#endif

	Audio::Shutdown();
	ImGuiImpl::Shutdown();
	Graphics::Shutdown();
	Input::Shutdown();
	Window::Shutdown();
	Script::Shutdown();

	ResourceManager::Shutdown();
	Data::Shutdown();
	//File::Shutdown();
	//Time::Shutdown();

#ifdef MEMORY_CUSTOM_CONTAINERS
	Memory::Shutdown();
#endif
}

void Engine::Reload()
{
#ifdef BX_EDITOR_BUILD
	Toolbar::Reset();
#endif
}

void Engine::Tick()
{
	//PROFILE_SECTION("Application");

	Profiler::Update();
	Script::CollectGarbage();
	Time::Update();
	Window::PollEvents();
	Input::Poll();

#ifdef BX_EDITOR_BUILD
	if (Toolbar::IsPlaying() && (Toolbar::ConsumeNextFrame() || !Toolbar::IsPaused()))
	{
		SystemManager::Update();
		//Scene::GetCurrent().Update();
		Script::Update();
	}
#else
	SystemManager::Update();
	//Scene::GetCurrent().Update();
	Script::Update();
#endif

	Graphics::NewFrame();
	ImGuiImpl::NewFrame();

#ifdef BX_EDITOR_BUILD
	if (Toolbar::IsPlaying() && !Toolbar::IsPaused())
	{
		SystemManager::Render();
		Script::Render();
	}
	Toolbar::Present();
#else
	SystemManager::Render();
	Script::Render();
#endif

	ImGuiImpl::EndFrame();
	Graphics::EndFrame();

	Window::Display();

#ifdef BX_EDITOR_BUILD
	AssetManager::Refresh();
#endif

	if (!Window::IsOpen())
	{
		Runtime::Close();
	}
}