#pragma once

#include <cstdint>

enum class Direction : unsigned int { NORTH = 0, SOUTH, WEST, EAST, SOUTHWEST, SOUTHEAST, NORTHWEST, NORTHEAST };

constexpr std::uint32_t MAX_COMPONENTS = 32;

using EntityID = std::uint32_t;

class EntityManager;

class Entity;

class SystemManager;
class EventManager;
