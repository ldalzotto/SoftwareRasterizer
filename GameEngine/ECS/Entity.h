#pragma once

#include "DataStructures/VectorT.h"
#include <unordered_map>
#include <list>

#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	struct ECS;

	struct Entity
	{
		ECS* ECS;
		std::unordered_map<ComponentType, Component*> Components;
	};

	bool Entity_comparator(Entity** p_left, Entity** p_right);
	 
	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent);
	void Entity_freeComponent(Entity* p_entity, Component** p_component);

	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType);

#define GET_COMPONENT(ComponentTypeName, EntityPointer) (ComponentTypeName*) Entity_getComponent(EntityPointer, #ComponentTypeName)->Child

	struct EntityContainer
	{
		std::vector<Entity*> Entities;
	};

	Entity* EntityContainer_allocEntity(ECS* p_ecs);
	void EntityContainer_freeEntity(Entity** p_entity);
	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents);

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	struct EntityConfigurableContainer
	{
		_Core::VectorT<ComponentType> ListenedComponentTypes;
		_Core::VectorT<Entity*> FilteredEntities;
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is attached.
		*/
		_Utils::Subject OnComponentAttachedEventListener;

		/**
			Called when a @ref Component with type contained in @ref ListenedComponentTypes is detached.
		*/
		_Utils::Subject OnComponentDetachedEventListener;
	};

	struct EntityConfigurableContainerInitInfo
	{
		_Core::VectorT<ComponentType> ListenedComponentTypes;

		/** This callback is called when the components listed in ListenedComponentTypes are attached to the Entity.
			It is called after component initialization. */
		void(*OnEntityThatMatchesComponentTypesAdded)(Entity*);

		/** This callback is called when the components liste in the ListenedComponentTypes are no more attached to the Entity.
			It is called before the components are freed. */
		void(*OnEntityThatMatchesComponentTypesRemoved)(Entity*);

		ECS* ECS;
	};

	void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo);
	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
};