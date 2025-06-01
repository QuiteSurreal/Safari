#pragma once

#include "UI.hpp"
#include "Behaviour.hpp"
#include "Labels.hpp"

class UIBehaviour : public Behaviour
{
public:

	void setMoney(Uint money) const;
	void setRating(float rating) const;
	void setCount(Uint count, std::string name) const;	
	void setPrice(Uint money, std::string name) const;	
	void setTime(Uint days, Uint hours, Uint minutes) const;
	
	bool clickMenu;
	bool clickGame;

	std::optional<Uint> speed;
	std::optional<std::string> mode;
	std::string difficulty;

	void openMenu();
	void openGame();
	void openResult(bool won);

private:

	const char *language = "english";

	static constexpr Vec4 colorText = {0.0f, 0.0f, 0.0f, 1.0f};
	static constexpr Vec4 colorBorder = {0.0f, 0.0f, 0.0f, 1.0f};
	static constexpr Vec4 colorBgActive = {1.0f, 1.0f, 1.0f, 0.8f};
	static constexpr Vec4 colorBg = {0.5f, 0.5f, 1.0f, 0.8f};
	static constexpr Vec4 colorBorderOff = Vec4 { 0.0f };
	static constexpr Vec4 colorBorderOn = Vec4 { Vec3 {0.0f}, 1.0f };

	void start(Entity &entity) override;
	void earlyUpdate(Entity &entity) override;

	void toggleDiff(const char *diff);
	void toggleLang(const char *lang);

	void toggleStructure(std::optional<std::string> id);
	void togglePlant(std::optional<std::string> id);
	void toggleExtras(std::optional<std::string> id);

};
