#include "ECS.h"

namespace _GameEngine::_ECS
{
	void EntityComponent_build(ECS* p_ecs, UpdateSequencer* p_updateSequencer, _Log::MyLog* p_myLog)
	{
		p_ecs->UpdateSequencer = p_updateSequencer;
		p_ecs->MyLog = p_myLog;
		SystemContainer_alloc(&p_ecs->SystemContainer);
		EntityContainer_alloc(&p_ecs->EntityContainer);
	};

	void EntityComponent_free(ECS* p_entityComponent)
	{
		EntityContainer_free(&(p_entityComponent)->EntityContainer, &(p_entityComponent)->ComponentEvents);
		SystemContainer_free(&(p_entityComponent)->SystemContainer);
	};
};