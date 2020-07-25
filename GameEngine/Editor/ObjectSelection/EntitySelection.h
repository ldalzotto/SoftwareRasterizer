#pragma once

#include "Math/Box/Box.h"
#include "Math/Transform/Transform.h"

#include "Physics/World/Collider/BoxCollider.h"

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _Physics { struct PhysicsInterface; }
	namespace _ECS { struct ECS; struct Entity; struct TransformComponent; }
}

namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

namespace _GameEngineEditor
{
	struct TransformGizmoPlane
	{
		_GameEngine::_Math::Transform Transform;
		_GameEngine::_Math::Box Box;
		_GameEngine::_Physics::BoxCollider Collider;
	};

	struct TransformGizmo
	{
		_GameEngine::_ECS::TransformComponent* TransformGizoEntity;
		_GameEngine::_ECS::TransformComponent* RightArrow;
		_GameEngine::_ECS::TransformComponent* UpArrow;
		_GameEngine::_ECS::TransformComponent* ForwardArrow;
		TransformGizmoPlane TransformGizmoSelectedArrayPlane;
	};

	struct EntitySelection
	{
		_GameEngine::_ECS::ECS* ECS;
		_GameEngine::_Input::Input* Input;
		_GameEngine::_Render::RenderInterface* RenderInterface;
		_GameEngine::_Physics::PhysicsInterface* PhysicsInterface;
		TransformGizmo TransformGizmoV2;
		// _GameEngine::_ECS::Entity* TransformGizmo;
		_GameEngine::_ECS::Entity* SelectedEntity;
		_GameEngine::_ECS::TransformComponent* SelectedTransformArrow;
	};

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
	void EntitySelection_update(EntitySelection* p_entitySelection);
}