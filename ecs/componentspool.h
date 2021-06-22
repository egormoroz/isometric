#pragma once

#include "../util/pool.h"
#include <array>
#include "component.h"



class ComponentsPool : public NonCopyable {
	std::array<std::unique_ptr<BasePool>, MAX_COMPONENTS> pools_;
public:
	ComponentsPool() = default;
	~ComponentsPool() = default;

	template<typename C, typename ...Args>
	void assign(EntityID id, Args && ...args) {
		lazy_init<C>();
		BaseComponent::Family fam = C::family();
		pools_[fam]->expand(id + 1);
		new (pools_[fam]->get(id)) C(std::forward<Args>(args)...);
	}

	template<typename C>
	C* get(EntityID id) {
		BaseComponent::Family fam = C::family();
		if (!pools_[fam] || pools_[fam]->size() <= id)
			return nullptr;
		return static_cast<C*>(pools_[fam]->get(id));
	}

	template<typename C>
	const C* get(EntityID id) const {
		BaseComponent::Family fam = C::family();
		if (!pools_[fam] || pools_[fam]->size() <= id)
			return nullptr;
		return static_cast<const C*>(pools_[fam]->get(id));
	}

	template<typename C>
	void destroy(EntityID id) {
		this->destroy(id, C::family());
	}

	void destroy(EntityID id, BaseComponent::Family fam) {
		assert(pools_[fam]);
		pools_[fam]->destroy(id);
	}

private:
	template<typename C>
	void lazy_init() {
		BaseComponent::Family fam = C::family();
		if (!pools_[fam])
			pools_[fam].reset(new Pool<C>);
	}
};
