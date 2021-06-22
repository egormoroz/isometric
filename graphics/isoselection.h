#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class IsoSelection : public sf::Drawable {
	sf::ConvexShape cs_;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
		target.draw(cs_, states);
	}
public:
	IsoSelection() {
		cs_.setPointCount(4);
	}

	void set_size(float x, float y) {
		cs_.setPoint(0, { 0.f, 0.f });
		cs_.setPoint(1, { x / 2, y / 2 });
		cs_.setPoint(2, { 0, y });
		cs_.setPoint(3, { -x / 2, y / 2 });
	}

	void set_fill_color(const sf::Color &clr) {
		cs_.setFillColor(clr);
	}
	
	void set_outline_color(const sf::Color &clr) {
		cs_.setOutlineColor(clr);
	}

	void set_outline_thickness(float t) {
		cs_.setOutlineThickness(t);
	}

	void set_position(float x, float y) {
		cs_.setPosition(x - y, (x + y) / 2);
	}
};
