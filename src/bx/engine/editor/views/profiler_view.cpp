#include "bx/engine/editor/views/profiler_view.hpp"

#include "bx/engine/data.hpp"

#include <bx/math/math.hpp>
#include <bx/core/profiler.hpp>
#include <bx/core/time.hpp>
#include <bx/containers/list.hpp>

#include <imgui.h>
#include <implot.h>
#include <IconsFontAwesome5.h>

#include <algorithm>

static int s_frames = 0;
static float s_time = 0;
static int s_fps = 0;

static int s_frame = 0;

bool ProfilerView::Initialize(bool& isShown)
{
    isShown = Data::GetBool("Toolbar Show Profiler", false, DataTarget::EDITOR);
    return true;
}

void ProfilerView::Shutdown(bool& isShown)
{
    Data::SetBool("Toolbar Show Profiler", isShown, DataTarget::EDITOR);
}

void ProfilerView::OnReload()
{
}

void ProfilerView::OnPlay()
{
}

void ProfilerView::OnStop()
{
}

void ProfilerView::OnPause(bool paused)
{
}

void ProfilerView::OnToolbar(bool& isShown)
{
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CHART_PIE))
    {
        isShown = !isShown;
    }
    //Tooltip("Profiler");
}

void ProfilerView::OnGui(bool& isShown)
{
    ImGui::Begin(ICON_FA_CHART_PIE"  Profiler", &isShown, ImGuiWindowFlags_NoCollapse);

    // TODO: Fix moving frame number
    //s_frame++;
    s_frames++;
    s_time += Time::GetDeltaTime();
    if (s_time >= 1.f)
    {
        s_fps = (int)(ceil(s_frames / s_time));
        s_time = std::fmod(s_time, 1.f);
        s_frames = 0;
    }
    ImGui::Text("FPS: %i", s_fps);

    const auto& data = Profiler::GetData();
    float height = (float)Math::Max(200, (int)(20 * data.size()));
    if (ImPlot::BeginPlot("Profiler", ImVec2(-1, height)))
    {
        ImPlot::SetupAxes("Samples", "Milliseconds");
        ImPlot::SetupAxesLimits(s_frame, s_frame + 100, 0, 50, ImPlotCond_Always);
        ImPlot::SetupLegend(ImPlotLocation_NorthWest, ImPlotLegendFlags_Outside);
        for (const auto& itr : data)
        {
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            ImPlot::PlotShaded(itr.first.c_str(), itr.second.frames.data(), (int)itr.second.frames.size());
            ImPlot::PopStyleVar();
            ImPlot::PlotLine(itr.first.c_str(), itr.second.frames.data(), (int)itr.second.frames.size());
        }
        ImPlot::EndPlot();
    }
    
    for (auto& itr : data)
        ImGui::LabelText(itr.first.c_str(), "%f ms", itr.second.avg);

    ImGui::End();
}