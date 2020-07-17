#include "Entity.h"

#include <stdexcept>

#include "Log/Log.hpp"
#include "ECS/ECS.h"
#include "Algorithm/Compare/CompareAlgorithmT.hpp"

namespace _GameEngine::_ECS
{

	bool ComponentType_comparator(ComponentType* p_left, ComponentType* p_right, void*)
	{
		return *p_left == *p_right;
	};

	void entity_free(Entity** p_entity);

	bool Entity_comparator(Entity** p_left, Entity** p_right, void*)
	{
		return *p_left == *p_right;
	};

	void Entity_addComponentDeferred(Entity* p_entity, Component* p_unlinkedComponent, ECS* p_ecs)
	{
		auto l_addComponentMessage = _ECS::ECSEventMessage_AddComponent_alloc(&p_entity, &p_unlinkedComponent);
		_ECS::ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_addComponentMessage);
	};

	void Entity_addComponent(Entity* p_entity, Component* p_unlinkedComponent)
	{

#ifndef NDEBUG
		if (_Core::CompareT_contains(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<Component*, ComponentType, void>{Component_comparator, p_unlinkedComponent->ComponentType}))
		{
			::_Core::String l_errorMessage; ::_Core::String_alloc(&l_errorMessage, 100);
			::_Core::String_append(&l_errorMessage, "Trying to add a component were it's type ( ");
			::_Core::String_append(&l_errorMessage, (char*)p_unlinkedComponent->ComponentType->c_str());
			::_Core::String_append(&l_errorMessage, " ) is aleady present as a component.");
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
		}
#endif
		_Core::VectorT_pushBack(&p_entity->Components, &p_unlinkedComponent);
		p_unlinkedComponent->AttachedEntity = p_entity;
		ComponentEvents_onComponentAttached(&p_entity->ECS->ComponentEvents, p_unlinkedComponent);
	};

	void Entity_freeComponent(Entity* p_entity, Component** p_component)
	{
		ComponentEvents_onComponentDetached(&p_entity->ECS->ComponentEvents, (*p_component));
		_Core::VectorT_eraseCompare(&p_entity->Components, _Core::ComparatorT<Component*, ComponentType, void>{ Component_comparator, (*p_component)->ComponentType });
		Component_free(p_component);
	};

	Component* Entity_getComponent(Entity* p_entity, ComponentType* p_componentType)
	{
		Component** l_foundComponent =
			_Core::CompareT_find(_Core::VectorT_buildIterator(&p_entity->Components), _Core::ComparatorT<Component*, ComponentType, void>{ Component_comparator, p_componentType }).Current;
		if (l_foundComponent)
		{
			return *l_foundComponent;
		}
		return nullptr;
	};

	void EntityContainer_alloc(EntityContainer* p_entityContainer)
	{
		_Core::VectorT_alloc(&p_entityContainer->Entities, 10);
	};

	void EntityContainer_free(EntityContainer* p_entityContainer, ComponentEvents* p_componentEvents)
	{
		_Core::VectorT_free(&p_entityContainer->Entities);
	};

	void EntityContainer_sendEventToDeleteAllEntities(EntityContainer* p_entityContainer, ECS* p_ecs)
	{
		for (size_t i = 0; i < p_entityContainer->Entities.Size; i++)
		{
			auto l_message = ECSEventMessage_removeEntity_alloc(_Core::VectorT_at(&p_entityContainer->Entities, i));
			ECSEventQueue_pushMessage(&p_ecs->EventQueue, &l_message);
		}
	};

	void EntityContainer_freeEntity(Entity** p_entity)
	{
		_Core::VectorT_eraseCompare(&(*p_entity)->ECS->EntityContainer.Entities, _Core::ComparatorT<Entity*, Entity*, void>{Entity_comparator, p_entity});
		entity_free(p_entity);
	};

	Entity* Entity_alloc(ECS* p_ecs)
	{
		Entity* l_instanciatedEntity = (Entity*)malloc(sizeof(Entity));
		l_instanciatedEntity->ECS = p_ecs;
		_Core::VectorT_alloc(&l_instanciatedEntity->Components, 2);
		return l_instanciatedEntity;
	};

	void entity_free(Entity** p_entity)
	{
		{
			// We copy components vector because operations inside the loop writes to the initial array
			_Core::VectorT<Component*> l_copiedComponents = _Core::VectorT_deepCopy(&(*p_entity)->Components);
			for (size_t i = 0; i < l_copiedComponents.Size; i++)
			{
				Entity_freeComponent((*p_entity), _Core::VectorT_at(&l_copiedComponents, i));
			}

			_Core::VectorT_free(&l_copiedComponents);
		}
		_Core::VectorT_free(&(*p_entity)->Components);

#ifndef NDEBUG
		EntityContainer* l_entityContainer = &(*p_entity)->ECS->EntityContainer;
		if (_Core::CompareT_contains(_Core::VectorT_buildIterator(&l_entityContainer->Entities), _Core::ComparatorT<Entity*, Entity*, void>{ Entity_comparator, p_entity }))
		{
			MYLOG_PUSH((*p_entity)->ECS->MyLog, ::_Core::LogLevel::WARN, "Potential wrong disposal of entity. When the Entity has been freed, is pointer is still present in the EntityContainer.");
		}
#endif

		free(*p_entity);
		*p_entity = nullptr;
	};

	///////////////////////////////////
	///////////////////////////////////  EntityConfigurableContainer
	///////////////////////////////////

	void entityComponentListener_onComponentAttachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component);
	void entityComponentListener_onComponentDetachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component);
	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs);
	void entityComponentListener_pushEntity(EntityConfigurableContainer* l_entityComponentListener, Entity* p_entity);
	void entityComponentListener_pushEntityIfElligible(EntityConfigurableContainer* l_entityComponentListener, Component* l_comparedComponent);
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
		_Core::VectorT_alloc(&p_entityComponentListener->FilteredEntities, 16);

		p_entityComponentListener->OnEntityThatMatchesComponentTypesAdded = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesAdded;
		p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved = p_entityComponentListenerInitInfo->OnEntityThatMatchesComponentTypesRemoved;

		p_entityComponentListener->OnComponentAttachedEventListener = { entityComponentListener_onComponentAttachedCallback, p_entityComponentListener };
		p_entityComponentListener->OnComponentDetachedEventListener = { entityComponentListener_onComponentDetachedCallback , p_entityComponentListener };

		entityComponentListener_pushAllElligibleEntities(p_entityComponentListener, &p_entityComponentListenerInitInfo->ECS->EntityContainer.Entities);
		entityComponentListener_registerEvents(p_entityComponentListener, p_entityComponentListenerInitInfo->ECS);
	};

	void EntityConfigurableContainer_free(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;

		for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::VectorT_at(&p_entityComponentListener->ListenedComponentTypes, i);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, _ECS::Component>*) &p_entityComponentListener->OnComponentAttachedEventListener);
			_Core::ObserverT_unRegister(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, _ECS::Component>*) &p_entityComponentListener->OnComponentDetachedEventListener);
		}

		for (size_t i = 0; i < p_entityComponentListener->FilteredEntities.Size; i++)
		{
			Entity* l_entity = *_Core::VectorT_at(&p_entityComponentListener->FilteredEntities, i);
			entityComponentListener_removeEntity(p_entityComponentListener, l_entity);

		}

		_Core::VectorT_free(&p_entityComponentListener->ListenedComponentTypes);
		_Core::VectorT_free(&p_entityComponentListener->FilteredEntities);
	};

	void entityComponentListener_onComponentAttachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component)
	{
		entityComponentListener_pushEntityIfElligible(p_entityComponentListener, p_component);
	};

	void entityComponentListener_onComponentDetachedCallback(EntityConfigurableContainer* p_entityComponentListener, Component* p_component)
	{

		if (_Core::CompareT_find(_Core::VectorT_buildIterator(&p_entityComponentListener->ListenedComponentTypes),
			_Core::ComparatorT<ComponentType, ComponentType, void>{ComponentType_comparator, p_component->ComponentType }).Current)
		{
			entityComponentListener_removeEntity(p_entityComponentListener, p_component->AttachedEntity);
		}
	};

	void entityComponentListener_registerEvents(EntityConfigurableContainer* p_entityComponentListener, ECS* p_ecs)
	{
		ComponentEvents* l_componentEvents = &p_ecs->ComponentEvents;


		for (size_t i = 0; i < p_entityComponentListener->ListenedComponentTypes.Size; i++)
		{
			ComponentType* l_componentType = _Core::VectorT_at(&p_entityComponentListener->ListenedComponentTypes, i);

			if (!l_componentEvents->ComponentAttachedEvents.contains(*l_componentType))
			{
				_Core::ObserverT<Component> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentAttachedEvents[*l_componentType] = l_createdObserver;
			}
			_Core::ObserverT_register(&l_componentEvents->ComponentAttachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) &p_entityComponentListener->OnComponentAttachedEventListener);


			if (!l_componentEvents->ComponentDetachedEvents.contains(*l_componentType))
			{

				_Core::ObserverT<Component> l_createdObserver;
				_Core::ObserverT_alloc(&l_createdObserver);
				l_componentEvents->ComponentDetachedEvents[*l_componentType] = l_createdObserver;
			}
			_Core::ObserverT_register(&l_componentEvents->ComponentDetachedEvents[*l_componentType], (_Core::CallbackT<void, Component>*) &p_entityComponentListener->OnComponentDetachedEventListener);
		}
	};

	void entityComponentListener_pushEntityIfElligible(_GameEngine::_ECS::EntityConfigurableContainer* l_entityComponentListener, _GameEngine::_ECS::Component* l_comparedComponent)
	{
		if (_Core::CompareT_contains(_Core::VectorT_buildIterator(&l_entityComponentListener->ListenedComponentTypes),
			_Core::ComparatorT<ComponentType, ComponentType, void>{ComponentType_comparator, l_comparedComponent->ComponentType }))
		{
			bool l_addEntity = true;
			for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.Size; i++)
			{
				ComponentType* l_componentType = _Core::VectorT_at(&l_entityComponentListener->ListenedComponentTypes, i);
				bool l_filteredComponentTypeIsAnEntityComponent = false;

				for (size_t j = 0; j < l_comparedComponent->AttachedEntity->Components.Size; j++)
				{
					Component* l_component = *_Core::VectorT_at(&l_comparedComponent->AttachedEntity->Components, j);
					if (*l_component->ComponentType == *l_componentType)
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
		for (size_t i = 0; i < p_queriedEntities->Size; i++)
		{
			Entity** p_entity = _Core::VectorT_at(p_queriedEntities, i);
			for (size_t i = 0; i < l_entityComponentListener->ListenedComponentTypes.Size; i++)
			{
				ComponentType* l_askedComponentType = _Core::VectorT_at(&l_entityComponentListener->ListenedComponentTypes, i);
				bool l_componentTypeMatchFound = false;

				for (size_t j = 0; j < (*p_entity)->Components.Size; j++)
				{
					Component** l_component = _Core::VectorT_at(&(*p_entity)->Components, j);
					if (*l_askedComponentType == *(*l_component)->ComponentType)
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
		if (!_Core::CompareT_contains(_Core::VectorT_buildIterator(&l_entityComponentListener->FilteredEntities),
			_Core::ComparatorT<Entity*, Entity*, void>{Entity_comparator, & p_entity}))
		{
			_Core::VectorT_pushBack(&l_entityComponentListener->FilteredEntities, &p_entity);
		}

		_Core::CallbackT_call(&l_entityComponentListener->OnEntityThatMatchesComponentTypesAdded, p_entity);
	};

	void entityComponentListener_removeEntity(EntityConfigurableContainer* p_entityComponentListener, Entity* p_entity)
	{
		_Core::VectorIteratorT<Entity*> l_foundEntityIt = _Core::CompareT_find(_Core::VectorT_buildIterator(&p_entityComponentListener->FilteredEntities), _Core::ComparatorT<Entity*, Entity*, void>{Entity_comparator, & p_entity});
		if (l_foundEntityIt.Current)
		{
			_Core::VectorT_erase(&p_entityComponentListener->FilteredEntities, l_foundEntityIt.CurrentIndex);
			_Core::CallbackT_call(&p_entityComponentListener->OnEntityThatMatchesComponentTypesRemoved, p_entity);
		}
	};

};