#pragma once

#include <variant>
#include <optional>

#include "Utils.hpp"
#include "Transform.hpp"
#include "Texture.hpp"
#include "Entity.hpp"
#include "ComponentManager.hpp"
#include "Mesh.hpp"

class Renderer;

namespace Renderable
{

	class LightGlobal : virtual public Component
	{
	public:
		virtual Vec3 getDirection() const = 0;
		virtual void setDirection(const Vec3& value) = 0;

		virtual Vec3 getColor() const = 0;
		virtual void setColor(const Vec3& value) = 0;

		virtual float getStrength() const = 0;
		virtual void setStrength(float value) = 0;

	protected:

		friend class ::Renderer;
	};

	class LightPoint : virtual public Component
	{
	public:

		virtual const Vec3* getPosition() const = 0;
		virtual void setPosition(const Vec3* value) = 0;

		virtual Vec3 getColor() const = 0;
		virtual void setColor(const Vec3& value) = 0;

		virtual float getRadius() const = 0;
		virtual void setRadius(float value) = 0;


	protected:

		friend class ::Renderer;
	};

	class Sprite : virtual public Component
	{
	public:

		virtual const Vec3* getPosition() const = 0;
		virtual void setPosition(const Vec3* value) = 0;

		virtual const Mat4* getTransform() const = 0;
		virtual void setTransform(const Mat4* value) = 0;

		virtual Vec3 getOffset() const = 0;
		virtual void setOffset(const Vec3& value) = 0;

		virtual Texture getTexture() const = 0;
		virtual void setTexture(const Texture& value) = 0;

		virtual float getOpacity() const = 0;
		virtual void setOpacity(float value) = 0;


	protected:

		friend class ::Renderer;

	private:

		const float shadowOffset = randf(0.0f, 1e-2f);
	};

	class ViewPoint : virtual public Component
	{
	protected:

		Vec3 pos;
		Mat4 proj;
		Mat4 view;

		friend class ::Renderer;
	};

	class Grid : virtual public Component
	{
	public:

		inline float getScale() const { return scale; }
		inline Ivec2 getSize() const { return size; }
		inline Vec3 getPos() const { return pos; }

		inline std::optional<Texture> &at(Ivec2 tile) const
		{
			assert(tile.x >= 0 && tile.y >= 0);
			assert(tile.x < size.x && tile.y < size.y);
			return textures[tile.y * size.x + tile.x];
		}

	protected:

		float scale;
		Ivec2 size;
		Vec3 pos;

		std::unique_ptr<std::optional<Texture>[]> textures;

		friend class ::Renderer;

	};

	class Terrain : virtual public Component
	{
	protected:

		Mesh mesh;
		Texture texture;

		friend class ::Renderer;
	};

	class Background : virtual public Component
	{
	public:

		virtual Vec3 getColor() const = 0;
		virtual void setColor(const Vec3& value) = 0;


	protected:

		friend class ::Renderer;
	};

	class UI : virtual public Component
	{
	protected:

		struct Base
		{
			bool hidden;
			Uint level;
			Ivec2 pos;
			Ivec2 size;
		};

		struct Box : Base
		{
			Vec4 color;
			Ivec4 borderSize;
			Vec4 borderColor;
		};

		struct Sprite : Base
		{
			Texture texture = {};
		};

		struct Text : Base
		{
			Vec4 color;
		};

		std::vector<std::variant<Box *, Sprite *, Text *>> inputs;

		friend class ::Renderer;
	};

	template <typename T>
	T &getUnique()
	{
		return ComponentManager::getUniqueComponent<T>();
	}

	template <typename T>
	void visitAll(auto visitor)
	{
		ComponentManager::visitComponents<T>(visitor);
	}

}
