#include "UI.hpp"

UI::Element &UI::Element::setId(std::string id)
{
	this->id = std::move(id);
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setMargin(int size)
{
	marginSizeX1 = size;
	marginSizeX2 = size;
	marginSizeY1 = size;
	marginSizeY2 = size;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setMargin(int sizeX, int sizeY)
{
	marginSizeX1 = sizeX;
	marginSizeX2 = sizeX;
	marginSizeY1 = sizeY;
	marginSizeY2 = sizeY;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setMargin(int sizeX1, int sizeX2, int sizeY1, int sizeY2)
{
	marginSizeX1 = sizeX1;
	marginSizeX2 = sizeX2;
	marginSizeY1 = sizeY1;
	marginSizeY2 = sizeY2;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setBorder(Vec4 color, int size)
{
	borderColor = color;
	borderSizeX1 = size;
	borderSizeX2 = size;
	borderSizeY1 = size;
	borderSizeY2 = size;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setBorder(Vec4 color, int sizeX, int sizeY)
{
	borderColor = color;
	borderSizeX1 = sizeX;
	borderSizeX2 = sizeX;
	borderSizeY1 = sizeY;
	borderSizeY2 = sizeY;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setBorder(Vec4 color, int sizeX1, int sizeX2, int sizeY1, int sizeY2)
{
	borderColor = color;
	borderSizeX1 = sizeX1;
	borderSizeX2 = sizeX2;
	borderSizeY1 = sizeY1;
	borderSizeY2 = sizeY2;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setPadding(int size)
{
	paddingSizeX1 = size;
	paddingSizeX2 = size;
	paddingSizeY1 = size;
	paddingSizeY2 = size;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setPadding(int sizeX, int sizeY)
{
	paddingSizeX1 = sizeX;
	paddingSizeX2 = sizeX;
	paddingSizeY1 = sizeY;
	paddingSizeY2 = sizeY;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setPadding(int sizeX1, int sizeX2, int sizeY1, int sizeY2)
{
	paddingSizeX1 = sizeX1;
	paddingSizeX2 = sizeX2;
	paddingSizeY1 = sizeY1;
	paddingSizeY2 = sizeY2;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::setColor(Vec4 color)
{
	this->box.color = color;
	binding->dirty = true;
	return *this;
}

UI::Element &UI::Element::changeText(const char *text)
{
	UI::Text *derived = dynamic_cast<UI::Text *>(this);
	assert(derived);
	derived->changeText(text);
	binding->dirty = true;
	return *this;
}

void UI::Element::hide()
{
	if (!hidden) {
		hidden = true;
		binding->dirty = true;
	}
}

void UI::Element::unhide()
{
	if (hidden) {
		hidden = false;
		binding->dirty = true;
	}
}

void UI::Element::toggle()
{
	hidden = !hidden;
	binding->dirty = true;
}

void UI::Element::bindTotal(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs)
{
	inputs.push_back(&box);
	bindInner(inputs);
}

UI::Element *UI::Element::findElement(const std::string &id)
{
	if (this->id == id) {
		return this;
	}
	for (auto &child : children) {
		Element *result = child->findElement(id);
		if (result) {
			return result;
		}
	}
	return nullptr;
}

void UI::Element::calcTotalSize(bool hide)
{
	hide = hide || hidden;
	if (hide) {
		calcInnerSize(hide);
		sizeInner = {};
		sizeOuter = {};
		sizeTotal = {};
		return;
	}
	sizeInner = calcInnerSize(hide);
	sizeOuter = sizeInner + Ivec2 {paddingSizeX1 + paddingSizeX2, paddingSizeY1 + paddingSizeY2} + Ivec2 {borderSizeX1 + borderSizeX2, borderSizeY1 + borderSizeY2};
	sizeTotal = sizeOuter + Ivec2 {marginSizeX1 + marginSizeX2, marginSizeY1 + marginSizeY2};
}

void UI::Element::placeTotal(bool hide, Ivec2 pos, int level)
{
	hide = hide || hidden;

	posTotal = pos;
	posOuter = posTotal + Ivec2 {marginSizeX1, marginSizeY1};
	posInner = posOuter + Ivec2 {borderSizeX1, borderSizeY1} + Ivec2 {paddingSizeX1, paddingSizeY1};

	box.hidden = hide;
	box.level = level;
	box.pos = posOuter;
	box.size = sizeOuter;
	box.borderSize = Ivec4 {borderSizeX1, borderSizeX2, borderSizeY1, borderSizeY2};
	box.borderColor = borderColor;

	placeInner(hide, posInner, level + 1);
}

UI::Image::Image(Texture texture, Ivec2 size)
{
	sprite.texture = texture;
	sprite.size = size;
}

void UI::Image::bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs)
{
	inputs.push_back(&sprite);
}

Ivec2 UI::Image::calcInnerSize(bool) const
{
	return sprite.size;
}

void UI::Image::placeInner(bool hide, Ivec2 pos, int level)
{
	sprite.hidden = hide;
	sprite.level = level;
	sprite.pos = pos;
}

UI::Text::Text(const char *text, Vec4 color, int fontSize)
	: color {color}, fontSize {fontSize}
{
	changeText(text);
}

void UI::Text::changeText(const char *text)
{
	if (sprite.texture.getTexture()) {
		sprite.texture.destroy();
	}
	sprite.texture = Fonts::createTextTexture(text, fontSize, color, sprite.size);
}

void UI::Text::bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs)
{
	inputs.push_back(&sprite);
}

Ivec2 UI::Text::calcInnerSize(bool) const
{
	return sprite.size;
}

void UI::Text::placeInner(bool hide, Ivec2 pos, int level)
{
	sprite.hidden = hide;
	sprite.level = level;
	sprite.pos = pos;
}

UI::ListVertical::ListVertical(Align align) : align {align}
{
}

void UI::ListVertical::bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs)
{
	for (auto &child : children) {
		child->bindTotal(inputs);
	}
}

Ivec2 UI::ListVertical::calcInnerSize(bool hide) const
{
	Ivec2 size = {};
	for (auto &child : children) {
		child->calcTotalSize(hide);
		size.y += child->sizeTotal.y;
		size.x = std::max(size.x, child->sizeTotal.x);
	}
	return size;
}

void UI::ListVertical::placeInner(bool hide, Ivec2 pos, int level)
{
	int offsetY = 0, offsetX;
	for (auto &child : children) {
		switch (align) {
			case Align::Begin: {
				offsetX = 0;
				break;
			}
			case Align::Center: {
				offsetX = (sizeInner.x - child->sizeTotal.x) / 2;
				break;
			}
			case Align::End: {
				offsetX = (sizeInner.x - child->sizeTotal.x);
				break;
			}
		}
		child->placeTotal(hide, pos + Ivec2 {offsetX, offsetY}, level + 1);
		offsetY += child->sizeTotal.y;
	}
	assert(offsetY == sizeInner.y);
}

UI::ListHorizontal::ListHorizontal(Align align) : align {align}
{
}

void UI::ListHorizontal::bindInner(std::vector<std::variant<Renderable::UI::Box *, Renderable::UI::Sprite *, Renderable::UI::Text *>> &inputs)
{
	for (auto &child : children) {
		child->bindTotal(inputs);
	}
}

Ivec2 UI::ListHorizontal::calcInnerSize(bool hide) const
{
	Ivec2 size = {};
	for (auto &child : children) {
		child->calcTotalSize(hide);
		size.x += child->sizeTotal.x;
		size.y = std::max(size.y, child->sizeTotal.y);
	}
	return size;
}

void UI::ListHorizontal::placeInner(bool hide, Ivec2 pos, int level)
{
	int offsetX = 0, offsetY;
	for (auto &child : children) {
		switch (align) {
			case Align::Begin: {
				offsetY = 0;
				break;
			}
			case Align::Center: {
				offsetY = (sizeInner.y - child->sizeTotal.y) / 2;
				break;
			}
			case Align::End: {
				offsetY = (sizeInner.y - child->sizeTotal.y);
				break;
			}
		}
		child->placeTotal(hide, pos + Ivec2 {offsetX, offsetY}, level + 1);
		offsetX += child->sizeTotal.x;
	}
	assert(offsetX == sizeInner.x);
}

UI::Element &UI::get(const std::string &id)
{
	Binding *binding {};
	Element *element = findElement(id, binding);
	if (!element || !binding) {
		printf("%s\n", id.c_str());
	}
	assert(element && binding);
	return *element;
}

bool UI::isVisible(const std::string &id)
{
	return !isHidden(id);
}

bool UI::isHidden(const std::string &id)
{
	Binding *binding {};
	Element *element = findElement(id, binding);
	assert(element && binding);
	return element->hidden;
}

void UI::end(Align alignX, Align alignY)
{
	assert(pending);
	pending->alignX = alignX;
	pending->alignY = alignY;
	pending->dirty = true;
	pending->root->bindTotal(inputs);
	bindings.push_back(std::move(pending));
}

const std::vector<std::string> &UI::getClicks() const
{
	return clicks;
}

void UI::arrange(Binding &binding)
{
	binding.root->calcTotalSize(false);
	const Ivec2 windowSize = Input::getWindowSize();
	Ivec2 pos;
	switch (binding.alignX) {
		case Align::Begin: {
			pos.x = 0;
			break;
		}
		case Align::Center: {
			pos.x = (windowSize.x - binding.root->sizeTotal.x) / 2;
			break;
		}
		case Align::End: {
			pos.x = (windowSize.x - binding.root->sizeTotal.x);
			break;
		}
	}
	switch (binding.alignY) {
		case Align::Begin: {
			pos.y = 0;
			break;
		}
		case Align::Center: {
			pos.y = (windowSize.y - binding.root->sizeTotal.y) / 2;
			break;
		}
		case Align::End: {
			pos.y = (windowSize.y - binding.root->sizeTotal.y);
			break;
		}
	}
	binding.root->placeTotal(false, pos, 0);
	binding.dirty = false;
}

UI::Element *UI::findElement(const std::string &id, Binding * &bindingOut)
{
	for (const auto &binding : bindings) {
		Element *result = binding->root->findElement(id);
		if (result) {
			bindingOut = binding.get();
			return result;
		}
	}
	return nullptr;
}

void UI::interact()
{
	this->clicks.clear();
	for (const auto &binding : bindings) {
		if (binding->dirty) {
			arrange(*binding);
		}
	}
	Input::visitClickList([this](auto click) {
		bool captured = false;
		for (const auto &binding : bindings) {
			if (binding->dirty) {
				arrange(*binding);
			}
			captured = captured || interactElement(binding->root, click);
		}
		return captured;
	});
}

bool UI::interactElement(const std::unique_ptr<Element> &element, std::pair<Input::MouseButton, Ivec2> click)
{
	if (element->hidden) {
		return false;
	}
	bool clicked = false;
	for (const auto &child : element->children) {
		clicked = clicked || interactElement(child, click);
	}
	const auto [button, pos] = click;
	if (intersect(element->posOuter, element->sizeOuter, pos)) {
		this->clicks.push_back(element->id);
		clicked = true;
	}
	return clicked;
}

bool UI::intersect(Ivec2 posA, Ivec2 sizeA, Ivec2 point)
{
	return posA.x <= point.x && point.x <= posA.x + sizeA.x && posA.y <= point.y && point.y <= posA.y + sizeA.y;
}
