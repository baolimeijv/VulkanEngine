#pragma once

#include"LveModel.h";
#include"glm/gtc/matrix_transform.hpp"

#include<memory>

namespace lve
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 0.5f,0.5f,0.5f};
		glm::vec3 rotation;


		glm::mat4 mat4()
		{
			auto transform = glm::translate(glm::mat4(1.0f), translation);
			transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, scale);
			return transform;
		}
	};

	class LveGameObject
	{
	public:
		using id_t = unsigned int;

		static LveGameObject createGameObject()
		{
			static id_t currentId = 0;
			return LveGameObject(currentId++);
		}

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject& operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		id_t getId() {return id;};

		std::shared_ptr<LveModel>model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		LveGameObject(id_t objId) :id(objId) {}
		id_t id;


	};
}