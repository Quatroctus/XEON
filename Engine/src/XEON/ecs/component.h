#pragma once

#include <string>

#include <glm/glm.hpp>

struct PositionComponent {
	glm::vec4 pos;
	glm::vec4 lastPos;
	PositionComponent(const glm::vec2& position) : pos{ glm::vec4(position, 0, 0) }, lastPos{ glm::vec4(position, 0, 0) } {}
	PositionComponent(const glm::vec3& position) : pos{ glm::vec4(position, 0) }, lastPos{ glm::vec4(position, 0) } {}
	PositionComponent(const glm::vec4& position) : pos{ position }, lastPos{ position } {}
};

struct NameComponent {
	std::string name;
};
