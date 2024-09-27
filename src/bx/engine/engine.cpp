#include "bx/engine/engine.hpp"

#include "bx/engine/data.hpp"
#include "bx/engine/resource.hpp"
#include "bx/engine/script.hpp"

#include <bx/platform/window.hpp>

#ifdef BX_EDITOR_BUILD
#include <bx/editor/assets.hpp>
#include <bx/editor/toolbar.hpp>
#include <bx/editor/views/profiler_view.hpp>
#include <bx/editor/views/data_view.hpp>
#include <bx/editor/views/assets_view.hpp>
#include <bx/editor/views/settings_view.hpp>
#include <bx/editor/views/console_view.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <IconsFontAwesome5.h>

static void InitializeImGui()
{
	ImPlot::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	const float uiScale = 1.0f;
	const float fontSize = 14.0f;
	const float iconSize = 12.0f;

	ImFontConfig config;
	config.OversampleH = 8;
	config.OversampleV = 8;
	io.Fonts->AddFontFromFileTTF(FREE_FONTS_DROID_SANS, fontSize * uiScale, &config);

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 }; // will not be copied by AddFont* so keep in scope.
	config.MergeMode = true;
	config.OversampleH = 8;
	config.OversampleV = 8;
	io.Fonts->AddFontFromFileTTF(FONT_AWESOME_6_FREE_SOLID_900, iconSize * uiScale, &config, icons_ranges);
}

static void EngineMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Views"))
		{
			if (ImGui::MenuItem("Assets"))
			{
				//ViewManager::AddView(new AssetsView());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Profiler"))
			{
				ViewManager::AddView(new ProfilerView());
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::ShowDemoWindow();
}
#endif

bool Engine::Initialize()
{
	Data::Initialize();
	ResourceManager::Initialize();
	Script::Initialize();

#ifdef BX_EDITOR_BUILD
	InitializeImGui();

	AssetManager::Initialize();

	ViewManager::AddMenuBarCallback(&EngineMenuBar);
	//ViewManager::Initialize();

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
	//ViewManager::Shutdown();
	AssetManager::Shutdown();
#endif

	Script::Shutdown();
	ResourceManager::Shutdown();
	Data::Shutdown();
}

void Engine::Reload()
{
}