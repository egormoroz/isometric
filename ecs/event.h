#pragma once

#include <unordered_map>
#include <cassert>
#include "simplesignal.h"

class BaseEvent {
public:
	using Family = std::uint32_t;

protected:
	static Family family_counter;
};

template<typename Derived>
class Event : public BaseEvent {
public:
	static Family family() {
		static Family family = family_counter++;
		return family;
	}
};

class EventManager {
	std::unordered_map<BaseEvent::Family, Simple::Signal<void(const void*)>> sigs_;
public:
	
	template<typename Event, typename ...Args>
	void emit(Args && ...args) {
		Event e(std::forward<Args>(args)...);
		sigs_[Event::family()].emit(&e);
	}

	template<typename Event, typename Receiver>
	void subscribe(Receiver &r) {
		sigs_[Event::family()].connect([&r](const void *pev) { 
			r.receive(*static_cast<const Event*>(pev));
		});
	}
};
