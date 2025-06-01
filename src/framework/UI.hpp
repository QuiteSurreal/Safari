#pragma once

#include <concepts>

#include "Utils.hpp"
#include "Renderable.hpp"
#include "Texture.hpp"
#include "Fonts.hpp"
#include "Input.hpp"

class UI : public Renderable::UI
{
private:

	struct Binding;

public:

	enum class Align
	{
		Begin,
		Center,
		End,
	};

	class Element
	{
	public:

		Element() = default;
		virtual ~Element() = default;

		Element &setId(std::string id);

		Element &setColor(Vec4 color);

		Element &setMargin(int size);
		Element &setMargin(int sizeX, int sizeY);
		Element &setMargin(int sizeX1, int sizeX2, int sizeY1, int sizeY2);

		Element &setBorder(Vec4 color, int size);
		Element &setBorder(Vec4 color, int sizeX, int sizeY);
		Element &setBorder(Vec4 color, int sizeX1, int sizeX2, int sizeY1, int sizeY2);

		Element &setPadding(int size);
		Element &setPadding(int sizeX, int sizeY);
		Element &setPadding(int sizeX1, int sizeX2, int sizeY1, int sizeY2);

		Element &changeText(const char *text);

		void hide();
		void unhide();
		void toggle();

		inline bool isHidden() const { return hidden; }

		inline void setBinding(Binding *binding) { this->binding = binding; }

	private:

		bool hidden = false;
		std::string id;

		Ivec2 posTotal;
		Ivec2 sizeTotal;

		Ivec2 posOuter;
		Ivec2 sizeOuter;

		Ivec2 posInner;
		Ivec2 sizeInner;

		int marginSizeX1 {0}, marginSizeX2 {0}, marginSizeY1 {0}, marginSizeY2 {0};

		Vec4 borderColor {0.0f};
		int borderSizeX1 {0}, borderSizeX2 {0}, borderSizeY1 {0}, borderSizeY2 {0};

		int paddingSizeX1 {0}, paddingSizeX2 {0}, paddingSizeY1 {0}, paddingSizeY2 {0};

		Renderable::UI::Box box = {};

		std::vector<std::unique_ptr<Element>> children;

		Element *findElement(const std::string &id);

		void bindTotal(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs);
		void calcTotalSize(bool hide);
		void placeTotal(bool hide, Ivec2 pos, int level);

		virtual void bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs) = 0;
		virtual Ivec2 calcInnerSize(bool hide) const = 0;
		virtual void placeInner(bool hide, Ivec2 pos, int level) = 0;

		Binding *binding = nullptr;

		friend class UI;
	};

	class Image : public Element
	{
	public:

		Image(Texture texture, Ivec2 size);

	private:

		Renderable::UI::Sprite sprite = {};

		void bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs) override;
		Ivec2 calcInnerSize(bool hide) const override;
		void placeInner(bool hide, Ivec2 pos, int level) override;
	};

	class Text : public Element
	{
	public:

		Text(const char *text, Vec4 color, int fontSize);
		void changeText(const char *text);

	private:

		Vec4 color;
		int fontSize;

		Renderable::UI::Sprite sprite = {};

		void bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs) override;
		Ivec2 calcInnerSize(bool hide) const override;
		void placeInner(bool hide, Ivec2 pos, int level) override;
	};

	class ListVertical : public Element
	{
	public:

		ListVertical(Align align);

		template <typename T, typename... Args>
		requires std::derived_from<T, Element>
		T &append(Args&&... args)
		{
			auto child = std::make_unique<T>(std::forward<Args>(args)...);
			assert(binding);
			child->setBinding(binding);
			auto &result = *child;
			children.push_back(std::move(child));
			binding->dirty = true;
			return result;
		}

	private:

		Align align;

		void bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs) override;
		Ivec2 calcInnerSize(bool hide) const override;
		void placeInner(bool hide, Ivec2 pos, int level) override;
	};

	class ListHorizontal : public Element
	{
	public:

		ListHorizontal(Align align);

		template <typename T, typename... Args>
		requires std::derived_from<T, Element>
		T &append(Args&&... args)
		{
			auto child = std::make_unique<T>(std::forward<Args>(args)...);
			assert(binding);
			child->setBinding(binding);
			auto result = child.get();
			children.push_back(std::move(child));
			binding->dirty = true;
			return *result;
		}

	private:

		Align align;

		void bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs) override;
		Ivec2 calcInnerSize(bool hide) const override;
		void placeInner(bool hide, Ivec2 pos, int level) override;
	};

	Element &get(const std::string &id);

	bool isVisible(const std::string &id);

	bool isHidden(const std::string &id);

	const std::vector<std::string> &getClicks() const;

	template <typename T, typename... Args>
	requires std::derived_from<T, Element>
	T &begin(Args&&... args)
	{
		assert(!pending);
		pending = std::make_unique<Binding>();
		pending->root = std::make_unique<T>(std::forward<Args>(args)...);
		pending->root->setBinding(pending.get());
		return *dynamic_cast<T *>(pending->root.get());
	}

	void end(Align alignX, Align alignY);

	void interact();

private:

	struct Binding
	{
		std::unique_ptr<Element> root;
		Align alignX, alignY;
		bool dirty;
	};

	std::unique_ptr<Binding> pending;
	std::vector<std::unique_ptr<Binding>> bindings;

	std::vector<std::string> clicks;

	void arrange(Binding &binding);

	Element *findElement(const std::string &id, Binding * &bindingOut);

	bool interactElement(const std::unique_ptr<Element> &element, std::pair<Input::MouseButton, Ivec2> click);

	static bool intersect(Ivec2 posA, Ivec2 sizeA, Ivec2 point);

};
