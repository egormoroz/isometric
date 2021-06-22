#pragma once

struct Health {
	explicit Health(int hp = 0)
		: points(hp)
	{}
	int points;
};
