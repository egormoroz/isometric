#pragma once

#include "../util/noncopyable.h"
#include <unordered_map>
#include <memory>
#include <cassert>

class EntityManager;
class EventManager;

class BaseSystem : public NonCopyable {
public:
	using Family = std::uint32_t;
	BaseSystem() = default;
	virtual ~BaseSystem() = default;

	virtual void update(EntityManager &es, EventManager &events, float delta_time) = 0;

protected:
	static Family family_counter;
};

template<typename Derived>
class System : public BaseSystem {
public:
	static Family family() {
		static Family family = family_counter++;
		return family;
	}
};

class SystemManager : public NonCopyable {
	std::unordered_map<BaseSystem::Family, std::unique_ptr<BaseSystem>> systems_;
	EntityManager &entities_;
	EventManager &events_;
public:
	SystemManager(EntityManager &entities, EventManager &events)
		: entities_(entities), events_(events)
	{}
	template<typename S, typename ...Args>
	void add(Args && ...args) {
		systems_.emplace(S::family(), std::make_unique<S>(std::forward<Args>(args)...));
	}

	template<typename S>
	S& get() {
		auto found = systems_.find(S::family());
		assert(found != systems_.end());
		return *found;
	}

	template<typename S>
	void destroy() {
		auto found = systems_.find(S::family());
		assert(found != systems_.end());
		systems_.erase(found);
	}
	
	template<typename S>
	void update(float delta_time) {
		auto found = systems_.find(S::family());
		assert(found != systems_.end());
		found->second->update(entities_, events_, delta_time);
	}

	void update_all(float delta_time) {
		for (auto &p : systems_) {
			p.second->update(entities_, events_, delta_time);
		}
	}
};
