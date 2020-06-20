#include "Entity.h"

#include <stdexcept>

#include "ECS/ECS.h"
#include "Utils/Algorithm/Algorithm.h"
#include "MyLog/MyLog.h"

#include "DataStructures/ElementComparators.h"

namespace _GameEngine::_ECS
{

	Entity* entity_alloc(ECS* p_ecs);
	void entity_free(Entity** p_entity);

	bool Entity_comparator(Entity** p_left, Entity** p_right)
	{
		return *p_left == *p_right;
	};

	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent)
	{

#ifndef NDEBUG
		if (p_entity->Components.get(Component_comparator, &p_unlinkedComponent->ComponentType))
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Trying to add a component were it's type ( " + p_unlinkedComponent->ComponentType + " ) is aleady present as a component."));
		}
#endif

		p_entity->Components.push_back(&p_unlinkedComponent);
		p_unlinkedComponent->AttachedEntity = p_entity;
		ComponentEvents_onComponentAttached(&p_entity->ECS->ComponentEvents, p_unlinkedComponent);
	};

	void Entity_freeComponent(Entity* p_entity, Component** p_component)
	{
		ComponentEvents_onComponentDetached(&p_entity->ECS->ComponentEvents, (*p_component));
		p_entity->Components.erase(Component_comparator, &(*p_component)->ComponentType);
		Component_free(p_component);
	};

	Component* Entity_getComponent(Entity* p_entity, const ComponentType& p_componentType)
	{
		Component** l_foundComponent = p_entity->Components.get(Component_comparator, (ComponentType*)&p_componentType);
		if (l_foundComponent)
		{
			return *l_foundComponent;
		}
		return nullptr;
	};

	Entity* EntityContainer_allocEntity(ECS* p_ecs)
	{
		Entity* l_instanciatedEntity = entity_alloc(p_ecs);
		p_ecs->EntityContainer.Entities.push_back(&l_instanciatedEntity);
		return l_instanciatedEntity;
	};

	void EntityContainer_alloc(EntityContainer* p_entityContainer)
	{
		p_entityContainer->Entities.alloc();
	};

	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents)
	{
		// We copy entities vector because operations inside the loop writes to the initial array
		_Core::VectorT<Entity*> l_copiedEntitiesPointer;
		p_entityContainer->Entities.deepCopy(&l_copiedEntitiesPointer);

		for (size_t i = l_copiedEntitiesPointer.size(); i--;)
		{
			EntityContainer_freeEntity(l_copiedEntitiesPointer.at(i));
		}

		l_copiedEntitiesPointer.free();
		p_entityContainer->Entities.free();
	};

	void EntityContainer_freeEntity(Entity** p_entity)
	{
		(*p_entity)->ECS->EntityContainer.Entities.erase(Entity_comparator, p_entity);
		entity_free(p_entity);
	};

	Entity* entity_alloc(ECS* p_ecs)
	{
		Entity* l_instanciatedEntity = (Entity*)malloc(sizeof(Entity));
		l_instanciatedEntity->ECS = p_ecs;
		l_instanciatedEntity->Components.alloc(2);
		return l_instanciatedEntity;
	};

	void entity_free(Entity** p_entity)
	{
		{
			// We copy components vector because operations inside the loop writes to the initial array
			_Core::VectorT<Component*> l_copiedComponents{};

			(*p_entity)->Components.deepCopy(&l_copiedComponents);
			for (size_t i = 0; i < l_copiedComponents.size(); i++)
			{
				Entity_freeComponent((*p_entity), l_copiedComponents.at(i));
			}

			l_copiedComponents.free();
		}


		(*p_entity)->Components.free();

#ifndef NDEBUG
		EntityContainer* l_entityContainer = &(*p_entity)->ECS->EntityContainer;
		if (l_entityContainer->Entities.get(Entity_comparator, p_entity))
		{
			MYLOG_PUSH((*p_entity)->ECS->MyLog, _Log::WARN, "Potential wrong disposal of entity. When the Entity has been freed, is pointer is still present in the EntityContainer.");
		}
#endif

		free(*p_entity);
		*p_entity = nullptr;
	};

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component);
	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component);
	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
	void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity);
	void entityComponentListener_pushEntityToEntityIfElligible(EntityConfigurableContainer* l_entityComponentListener, Component* l_comparedComponent);
	void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity);

	/**
		When initializing the @ref EntityConfigurableContainer, we check the presence of the listened @ref ComponentType for all @ref Entity and push the
		Entity to the Listener if successfull.
		This is to be sure that all already present Entities are checked if the system initiation is done after the Entity creation.
	*/
	void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, _Core::VectorT<Entity*>* p_queriedEntities);


	void EntityConfigurableContainer_init(EntityConfigurableContainer* p_entityComponentListener, EntityConfigurableContainerInitInfo* p_entityComponentListenerInitInfo)
	{
		p_entityComponentListener->ListenedComponentTypes = p_entityComponentListenerInitInfo->ListenedComponentTypes;
		p_entityComponentListener->FilteredEntities.alloc(16);

		p_entityComponentListener->OnEntityThatMatchesComponentTypesAdded = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAdded;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesAddedUserdata = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAddedUserdata;

		p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemoved;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesRemovedUserData = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemovedUserData;

		p_entityComponentListener->OnComponentAttachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->OnComponentAttachedEventListener.Callback = entityComponentListener_onComponentAttachedCallback;

		p_entityComponentListener->OnComponentDetachedEventListener.Closure = p_entityComponentListener;
		p_entityComponentListener->OnComponentDetachedEventListener.Callback = entityComponentListener_onComponentDetachedCallback;

		entityComponentListener_pushAllElligibleEntities(p_entityComponentListener, &p_entityComponentListenerInitInfo->ECS->EntityContainer.Entities);
		entityComponentListener_registerEvents(p_entityComponentListener, p_entityComponentListenerInitInfo->ECS);
	};

	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.size(); i++)
		{
			ComponentType* l_componentType = p_entityComponentListener->ListenedComponentTypes.at(i);
			_Utils::Observer_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], &p_entityComponentListener->OnComponentAttachedEventListener);
			_Utils::Observer_unRegister(&l_componentEvents->ComponentDetachedEvents[*l_componentType], &p_entityComponentListener->OnComponentDetachedEventListener);
		}

		for (size_t i = 0; i < p_entityComponentListener->FilteredEntities.size(); i++)
		{
			Entity* l_entity = *p_entityComponentListener->FilteredEntities.at(i);
			entityComponentListener_removeEntity(p_entityComponentListener, l_entity);

		}

		p_entityComponentListener->ListenedComponentTypes.free();
		p_entityComponentListener->FilteredEntities.free();
	};

	void entityComponentListener_onComponentAttachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityConfigurableContainer* l_entityComponentListener = (EntityConfigurableContainer*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;
		entityComponentListener_pushEntityToEntityIfElligible(l_entityComponentListener, l_component);
	};

	void entityComponentListener_onComponentDetachedCallback(void* p_entityComponentListener, void* p_component)
	{
		EntityConfigurableContainer* l_entityComponentListener = (EntityConfigurableContainer*)p_entityComponentListener;
		Component* l_component = (Component*)p_component;


		ComponentType* l_foundComponentType = l_entityComponentListener->ListenedComponentTypes.get(_Core::Vector_equalsStringComparator, &l_component->ComponentType);
		if (l_foundComponentType)
		{
			entityComponentListener_removeEntity(l_entityComponentListener, l_component->AttachedEntity);
		}
	};

	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;


		for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.size(); i++)
		{
			ComponentType* l_componentType = p_entityComponentListener->ListenedComponentTypes.at(i);

			if (!l_componentEvents->ComponentAttachedEvents.contains(*l_componentType))
			{
				l_componentEvents->ComponentAttachedEvents[*l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&l_componentEvents->ComponentAttachedEvents[*l_componentType], &p_entityComponentListener->OnComponentAttachedEventListener);


			if (!l_componentEvents->ComponentDetachedEvents.contains(*l_componentType))
			{
				l_componentEvents->ComponentDetachedEvents[*l_componentType] = _Utils::Observer{};
			}
			_Utils::Observer_register(&l_componentEvents->ComponentDetachedEvents[*l_componentType], &p_entityComponentListener->OnComponentDetachedEventListener);
		}
	};

	void entityComponentListener_pushEntityToEntityIfElligible(_GameEngine::_ECS::EntityConfigurableContainer* l_entityComponentListener, _GameEngine::_ECS::Component* l_comparedComponent)
	{
		if (l_entityComponentListener->ListenedComponentTypes.get(_Core::Vector_equalsStringComparator, &l_comparedComponent->ComponentType))
		{
			bool l_addEntity = true;
			for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.size(); i++)
			{
				ComponentType* l_componentType = l_entityComponentListener->ListenedComponentTypes.at(i);
				bool l_filteredComponentTypeIsAnEntityComponent = false;

				for (size_t i = 0; i < l_comparedComponent->AttachedEntity->Components.size(); i++)
				{
					Component* l_component = *l_comparedComponent->AttachedEntity->Components.at(i);
					if (l_component->ComponentType == *l_componentType)
					{
						l_filteredComponentTypeIsAnEntityComponent = true;
						break;
					}
				}

				if (!l_filteredComponentTypeIsAnEntityComponent)
				{
					l_addEntity = false;
					break;
				}
			}


			if (l_addEntity)
			{
				entityComponentListener_pushEntity(l_entityComponentListener, l_comparedComponent->AttachedEntity);
			}
		}
	};

	void entityComponentListener_pushAllElligibleEntities(EntityConfigurableContainer* l_entityComponentListener, _Core::VectorT<Entity*>* p_queriedEntities)
	{
		for (size_t i = 0; i < p_queriedEntities->size(); i++)
		{
			Entity** p_entity = p_queriedEntities->at(i);
			for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.size(); i++)
			{
				ComponentType* l_askedComponentType = l_entityComponentListener->ListenedComponentTypes.at(i);
				bool l_componentTypeMatchFound = false;

				for (size_t i = 0; i < (*p_entity)->Components.size(); i++)
				{
					Component** l_component = (*p_entity)->Components.at(i);
					if (*l_askedComponentType == (*l_component)->ComponentType)
					{
						l_componentTypeMatchFound = true;
						break;
					}
				}

				if (!l_componentTypeMatchFound)
				{
					goto checkNextEntity;
				}
			}

			entityComponentListener_pushEntity(l_entityComponentListener, *p_entity);

		checkNextEntity:;
		}
	};

	void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity)
	{
		if (!l_entityComponentListener->FilteredEntities.get(Entity_comparator, &p_entity))
		{
			l_entityComponentListener->FilteredEntities.push_back(&p_entity);
		}

		if (l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded)
		{
			l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded(p_entity, l_entityComponentListener->OnEntityThatMatchesComponentTypesAddedUserdata);
		}
	};

	void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity)
	{
		size_t l_foundEntityIndex = p_entityComponentListener->FilteredEntities.getIndex(Entity_comparator, &p_entity);
		if (l_foundEntityIndex != std::numeric_limits<size_t>::max())
		{
			p_entityComponentListener->FilteredEntities.erase(l_foundEntityIndex);
			if (p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved)
			{
				p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved(p_entity, p_entityComponentListener->OnEntityThatMatchesComponentTypesRemovedUserData);
			}
		}
	};

};