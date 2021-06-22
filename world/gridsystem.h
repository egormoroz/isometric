#pragma once

#include "../util/noncopyable.h"
#include <array>
#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include "../util/pool.h"
#include <cmath>


class BaseLevelArea : public sf::FloatRect {
public:
	using Family = std::uint32_t;
	BaseLevelArea() 
		: sf::FloatRect()
	{}
	BaseLevelArea(float x, float y, float w, float h)
		: sf::FloatRect(x, y, w, h)
	{}

protected:
	static Family family_counter;
};

constexpr BaseLevelArea::Family MAXLEVELAREAS = 32;

template<typename Derived>
class LevelArea : public BaseLevelArea {
public:
	LevelArea() = default;
	LevelArea(float x, float y, float w, float h)
		: BaseLevelArea(x, y, w, h) {}

	static Family family() {
		static const Family fam = family_counter++;
		assert(fam < MAXLEVELAREAS);
		return fam;
	}
};


//TODO: find out the best sector size
class GridSystem : public NonCopyable {
	std::array<std::unique_ptr<BasePool>, MAXLEVELAREAS> pools_;
	struct Sector {
		std::array<std::vector<size_t>, MAXLEVELAREAS> areas;
	};
	std::vector<std::vector<Sector>> sectors_;
	int width_ = 0;
	int height_ = 0;
	const float sector_width_;
	const float sector_height_;

	
public:
	GridSystem()
		: sector_width_(200.f), sector_height_(200.f)
	{}
	~GridSystem() {
		if (!sectors_.empty()) {
			this->clear();
		}
	}

	void set_size(float width, float height) {
		width_ = static_cast<size_t>(ceil(width / sector_width_));
		height_ = static_cast<size_t>(ceil(height / sector_height_));
		sectors_.resize(width_, std::vector<Sector>(height_));
	}

	template<typename Area, typename ...Args>
	void add_area(Args && ...args) {
		BaseLevelArea::Family fam = Area::family();
		if (!pools_[fam]) {
			pools_[fam].reset(new Pool<Area, 256>);
		}
		size_t id = pools_[fam]->size();
		pools_[fam]->expand(id + 1);
		Area * area = new (pools_[fam]->get(id)) Area(std::forward<Args>(args)...);

		int x = std::max(static_cast<int>(floor(area->left / sector_width_)), 0),
			y = std::max(static_cast<int>(floor(area->top / sector_height_)), 0),
			xx = std::min(static_cast<int>(ceil((area->left + area->width) / sector_width_)), width_),
			yy = std::min(static_cast<int>(ceil((area->top + area->height) / sector_height_)), height_);
		for (int i = x; i < xx; ++i) {
			for (int j = y; j < yy; ++j) {
				sectors_[i][j].areas[fam].emplace_back(id);
			}
		}
	}

	template<typename Area, typename Predicate>
	void each_intersected(const sf::FloatRect &bounds, Predicate p) const {
		BaseLevelArea::Family fam = Area::family();
		if (!pools_[fam])
			return;
		int x = std::max(static_cast<int>(floor(bounds.left / sector_width_)), 0),
			y = std::max(static_cast<int>(floor(bounds.top / sector_height_)), 0),
			xx = std::min(static_cast<int>(ceil((bounds.left + bounds.width) / sector_width_)), width_),
			yy = std::min(static_cast<int>(ceil((bounds.top + bounds.height) / sector_height_)), height_);
		for (int i = x; i < xx; ++i) {
			for (int j = y; j < yy; ++j) {
				for (size_t idx : sectors_[i][j].areas[fam]) {
					const Area &area = *static_cast<const Area*>(pools_[fam]->get(idx));
					if (area.intersects(bounds)) {
						p(area);
					}
				}
			}
		}
	}

	void clear() {
		for (BaseLevelArea::Family i = 0; i < MAXLEVELAREAS; ++i) {
			if (pools_[i]) {
				pools_[i]->destroy_all();
			}
		}
		sectors_.clear();
	}

};

