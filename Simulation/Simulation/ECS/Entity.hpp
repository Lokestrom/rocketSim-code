#pragma once

#include "helpers/ID.hpp"
#include "helpers/TransformComponent3D.hpp"

#include "DataRegistry.hpp"

#include <algorithm>
#include <iterator>

class Entity
{
public:
	Entity() = delete;
	Entity(Database::String name, ID::ID_T localID);

	IDview getID() const noexcept {
		return _id;
	}
private:
	//should use a pure id remove the name and localID it is not needed any more
	ID _id;
};

class EntityContainer {
public:
	template<DataType Component>
	void addComponent(const ID::UUID id, const Component::value_type& data);
	template<DataType Component>
	void addComponent(const ID::UUID id, Component::value_type&& data);
	template<DataType Component>
	void removeComponent(const ID::UUID id) noexcept;
	void removeComponent(ID::UUID id, DataRegistry::ComponentID componentID) noexcept;

	template<DataType... Components>
	Entity& addEntity(Database::String name, ID::ID_T localID, Components::value_type&&... components);
	void removeEntity(ID::UUID id) noexcept;

	const std::vector<Entity>& getEntities() const noexcept;
	const std::vector<DataRegistry::ComponentID> getEntityComponents(ID::UUID id);

	void clear() noexcept;
	size_t size() const noexcept { return _entities.size(); }

private:
	bool contains(ID::UUID id) noexcept;
	size_t index(ID::UUID id) noexcept;

	void removeAllComponents(ID::UUID id) noexcept;

private:
	std::vector<Entity> _entities;
	std::vector<std::vector<DataRegistry::ComponentID>> _components;

	//currently O(N). can sort the entities to make it O(log(N)) or use a map for O(1)
};

template<DataType Component>
void EntityContainer::addComponent(const ID::UUID id, const Component::value_type& data)
{
	assert(contains(id) && "EntityContainer does not contain the given id");
	DataArray<Component>& componentArray = DataRegistry::get<Component>();
	componentArray.add(id, data);
	_components[index(id)].push_back(DataRegistry::getID<Component>());
}

template<DataType Component>
void EntityContainer::addComponent(const ID::UUID id, Component::value_type&& data)
{
	assert(contains(id) && "EntityContainer does not contain the given id");
	DataArray<Component>& componentArray = DataRegistry::get<Component>();

	componentArray.add(id, std::forward<typename Component::value_type>(data));
	_components[index(id)].push_back(DataRegistry::getID<Component>());
}

template<DataType Component>
void EntityContainer::removeComponent(const ID::UUID id) noexcept
{
	assert(contains(id) && "EntityContainer does not contain the given id");
	//assert(std::ranges::contains(_components(index(id)), DataRegistry::getID<Component>()) && "The entity does not have the component");
	
	removeComponent(id, DataRegistry::getID<Component>());
}

inline void EntityContainer::removeComponent(ID::UUID id, DataRegistry::ComponentID componentID) noexcept
{
	DataRegistry::removeComponent(id, componentID);
	_components[index(id)].erase(std::remove(_components[index(id)].begin(), _components[index(id)].end(), componentID), _components[index(id)].end());
}

inline const std::vector<Entity>& EntityContainer::getEntities() const noexcept
{
	return _entities;
}

inline const std::vector<DataRegistry::ComponentID> EntityContainer::getEntityComponents(ID::UUID id)
{
	return _components[index(id)];
}

inline void EntityContainer::clear() noexcept
{
	while(_entities.size() != 0) {
		removeEntity(_entities.front().getID().getUUID());
	}
}

template<DataType ...Components>
Entity& EntityContainer::addEntity(Database::String name, ID::ID_T localID, Components::value_type&& ...components)
{
	_entities.emplace_back(name, localID);
	_components.push_back({});
	(addComponent<Components>(_entities.back().getID().getUUID(), components), ...);
	return _entities.back();
}

inline void EntityContainer::removeEntity(ID::UUID id) noexcept {
	assert(contains(id) && "EntityContainer does not contain the given id");
	
	removeAllComponents(id);
	size_t i = index(id);
	_entities.erase(_entities.begin() + i);
	_components.erase(_components.begin() + i);
}

inline bool EntityContainer::contains(ID::UUID id) noexcept
{
	return std::ranges::contains(_entities, id, [id](Entity& e) { return e.getID().getUUID(); });
}

inline size_t EntityContainer::index(ID::UUID id) noexcept
{
	assert(contains(id) && "EntityContainer does not contain the given id");
	return std::ranges::find_if(_entities, [id](Entity& e) { return e.getID().getUUID() == id; }) - _entities.begin();
}

inline void EntityContainer::removeAllComponents(ID::UUID id) noexcept
{
	size_t i = index(id);
	for (auto compID : _components[i]) {
		DataRegistry::removeComponent(id, compID);
	}
	_components[i].clear();
}
