#pragma once

#include "IMGuiRender/IMGuiRender.h"
#include "Utils/Observer/Observer.h"
#include "DebugConsole/DebugConsole.h"
#include "IMGuiRender/DrawableWindow.h"
#include "Gizmo/GizmoDrawStep.h"

using namespace _GameEngine;

namespace _GameEngine
{
	struct GameEngineApplication;
}

namespace _GameEngineEditor
{
	struct GameEngineEditor
	{
		GameEngineApplication* GameEngineApplication;
		_Utils::Subject OnPreRender;
		IMGuiRender IMGuiRender;
		DebugConsole DebugConsole;
		DrawableWindows DrawableWindows;
		GizmoDrawStep GizmoDrawStep;
	};

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplication* p_gameEngineApplication);
	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor, GameEngineApplication* p_gameEngineApplication);
}