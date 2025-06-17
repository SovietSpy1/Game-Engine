#pragma once
#include <DX3D/Core/Base.h>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <string>
#include <bitset>
#include <cassert>

namespace dx3d {
	constexpr UINT MAX_COMPONENTS = 64;

	class GameManager final : public Base
	{
	public:
		//constructor
		GameManager();
	};

	struct Entity {
		UINT id{};
		bool operator==(const Entity& other) const {
			return id == other.id;
		}
	};

	class ComponentListBase : public Base{
	public:
		ComponentListBase(const BaseDesc& desc) : Base(desc) {};
		virtual ~ComponentListBase() = default;
		virtual void Remove(Entity entity) = 0;
	};

	template<typename ComponentType>
	class ComponentList : public ComponentListBase {
	public:
		ComponentList(const BaseDesc& desc) : ComponentListBase(desc) {};
		ComponentType* Get(Entity entity) {
			auto it = entityComponentMap.find(entity);
			if (it == entityComponentMap.end()) {
				return nullptr;
			}
			return &it->second;
		}

		void Insert(Entity entity, ComponentType component) {
			entityComponentMap[entity] = component;
		}

		void Remove(Entity entity) override {
			entityComponentMap.erase(entity);
		}

		//variables
		std::unordered_map<Entity, ComponentType> entityComponentMap;
	};

	class Registry final : public Base{
	public:
		Registry(const BaseDesc& desc) : Base(desc) {};
		//functions
		Entity CreateEntity() {
			return Entity{ nextEntity++ };
		}

		template <typename ComponentType>
		void RegisterComponent() {
			std::type_index typeIndex = typeid(ComponentType);
			if (typeIndexMap.count(typeIndex)) {
				std::string message = std::string(typeid(ComponentType).name()) + " already registered!";
				return;
			}
			typeComponentMap[typeIndex] = std::make_unique<ComponentList<ComponentType>>(BaseDesc{m_logger});
			typeIndexMap[typeIndex] = componentMaskIndex++;
		}

		template <typename ComponentType>
		void UnRegisterComponent() {
			std::type_index typeIndex = typeid(ComponentType);
			if (!typeIndexMap.count(typeIndex)) {
				std::string message = std::string(typeid(ComponentType).name()) + " cannot be unregistered because it is not registered!";
				return;
			}

			UINT componentIndex = typeIndexMap[typeIndex];
			for (auto& [entity, mask] : entityMaskMap) {
				mask.reset(componentIndex);
			}
			typeComponentMap.erase(typeIndex);
			typeIndexMap.erase(typeIndex);
		}

		template <typename ComponentType>
		ComponentList<ComponentType>* GetComponentList() {
			std::type_index typeIndex = typeid(ComponentType);
			if (!typeIndexMap.count(typeIndex)) {
				std::string message = std::string(typeid(ComponentType).name()) + " list cannot be retrieved because it isn't registered!";
				return nullptr;
			}
			return static_cast<ComponentList<ComponentType>*>(typeComponentMap[typeIndex].get());
		}

		template <typename ComponentType>
		ComponentType* GetComponent(Entity entity) {
			auto* componentList = GetComponentList<ComponentType>();
			if (!componentList) return nullptr;
			return componentList->Get(entity);
		}

		template <typename ComponentType>
		void AddComponent(Entity entity, ComponentType component) {
			std::type_index typeIndex = typeid(ComponentType);
			if (!typeIndexMap.count(typeIndex)) {
				std::string message = "cannot add unregistered component " + std::string(typeid(ComponentType).name());
				return;
			}

			auto* componentList = static_cast<ComponentList<ComponentType>*>(typeComponentMap[typeIndex].get());
			componentList->Insert(entity, component);
			entityMaskMap[entity].set(typeIndexMap[typeIndex]);
		}

		template <typename ComponentType>
		void RemoveComponent(Entity entity) {
			std::type_index typeIndex = typeid(ComponentType);
			if (!typeIndexMap.count(typeIndex)) {
				std::string message = "cannot remove unregistered component " + std::string(typeid(ComponentType).name());
				return;
			}

			typeComponentMap[typeIndex]->Remove(entity);
			entityMaskMap[entity].reset(typeIndexMap[typeIndex]);
		}

		template <typename... ComponentTypes>
		std::bitset<MAX_COMPONENTS> GetMask() {
			std::bitset<MAX_COMPONENTS> mask{};
			(..., (assert(typeIndexMap.count(typeid(ComponentTypes)) && "Can't Get Mask For Unregistered Component"),
				mask.set(typeIndexMap[typeid(ComponentTypes)])));
			return mask;
		}

		//variables
		std::unordered_map<std::type_index, std::unique_ptr<ComponentListBase>> typeComponentMap;
		std::unordered_map<std::type_index, UINT> typeIndexMap;
		std::unordered_map<Entity, std::bitset<MAX_COMPONENTS>> entityMaskMap;
		UINT componentMaskIndex = 0;
		UINT nextEntity = 0;
	};
}

// Hash function for Entity should be defined elsewhere in your codebase