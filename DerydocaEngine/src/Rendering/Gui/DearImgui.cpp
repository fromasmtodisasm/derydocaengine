#include "EnginePch.h"
#include "DearImgui.h"

#include <vendor/imgui/imgui_impl_sdl.h>
#if (OPENGL == 1)
#include <vendor/imgui/imgui_impl_opengl3.h>
#endif

namespace DerydocaEngine::Rendering::Gui
{

	ImGuiIO DearImgui::init(SDL_Window* window, SDL_GLContext& context)
	{
		char* GLSL_VERSION = "#version 130";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto guiIO = ImGui::GetIO(); (void)guiIO;

		ImGui::StyleColorsDark();
		
		ImGui_ImplSDL2_InitForOpenGL(window, context);
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
#endif
		return guiIO;
	}

	void DearImgui::newFrame(SDL_Window* window)
	{
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_NewFrame();
#endif
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		static bool show_demo_w = true;
		if (show_demo_w)
		{
			ImGui::ShowDemoWindow(&show_demo_w);
		}
	}

	void DearImgui::render(SDL_Window* window, SDL_GLContext& context)
	{
		ImGui::Render();
		SDL_GL_MakeCurrent(window, context);
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	}

	void DearImgui::shutdown()
	{
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_Shutdown();
#endif
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	bool DearImgui::processEvent(SDL_Event& event)
	{
		return ImGui_ImplSDL2_ProcessEvent(&event);
	}

}