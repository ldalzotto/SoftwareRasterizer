#pragma once

#include <functional>

#include "Functional/Callback/ObserverT.hpp"

#include "Clock/Clock.hpp"
#include "Log/Log.hpp"

#include "GameLoop/GameLoop.h"
#include "EngineSequencers/EngineSequencers.h"

#include "Physics/Physics.h"
#include "Render/Render.h"
#include "ECS/ECS.h"
#include "Input/Input.h"

#include "GameEngineApplicationInterface.h"

namespace _GameEngine
{
	struct GameEngineApplication
	{
		GameEngineApplicationInterface GameEngineApplicationInterface;

		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> NewFrame;
		_Core::ObserverT<_GameEngine::GameEngineApplicationInterface> PreRender;
		UpdateSequencer UpdateSequencer;
		std::function<void(float)> SandboxUpdateHook;

		GameLoop GameLoop;
		::_Core::Log Log;
		::_Core::Clock Clock;
		_Physics::Physics Physics;
		_Render::Render Render;
		_Input::Input Input;
		_ECS::ECS ECS;
	};

	GameEngineApplication* app_alloc(const std::function<void(float)>& p_sandboxUpdateHook);
	void app_free(GameEngineApplication* p_app);
	void app_mainLoop(GameEngineApplication* p_app);
	void app_cleanup(GameEngineApplication* p_app);
	void app_run(GameEngineApplication* p_app);
} // namespace _GameEngine
