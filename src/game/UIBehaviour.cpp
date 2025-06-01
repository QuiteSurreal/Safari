#include "UIBehaviour.hpp"
#include "Input.hpp"

void UIBehaviour::start(Entity &entity)
{
	const Texture placeholder = Texture::fromImage("placeholder.png");
	const Texture camera = Texture::fromImage("camera.png");
	const Texture droneStation = Texture::fromImage("droneStation.png");
	const Texture airship = Texture::fromImage("airship.png");
	const Texture hunter = Texture::fromImage("hunter.png");
	const Texture hunterHut = Texture::fromImage("hunterHut.png");
	const Texture water = Texture::fromImage("WaterVertical.png");
	const Texture road = Texture::fromImage("RoadHorizontal.png");
	const Texture jeep = Texture::fromImage("Jeep.png");

	auto &ui = entity.getComponent<UI>();

	auto &menuCenter = ui.begin<UI::ListVertical>(UI::Align::Center);
		menuCenter.setId("menuCenter");
		menuCenter.setColor(Vec4 { 0.9f });
		menuCenter.setPadding(20, 0);
		menuCenter.append<UI::Text>("Safari", colorText, 100)
			.setPadding(0, 0, 20, 0);
		auto &menuDiff = menuCenter.append<UI::ListHorizontal>(UI::Align::Center);
			menuDiff.setMargin(4);
			menuDiff.append<UI::Text>("easy", colorText, 22)
				.setId("diffEasy")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
			menuDiff.append<UI::Text>("normal", colorText, 22)
				.setId("diffNormal")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
			menuDiff.append<UI::Text>("hard", colorText, 22)
				.setId("diffHard")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
		auto &menuLang = menuCenter.append<UI::ListHorizontal>(UI::Align::Center);
			menuLang.setMargin(4);
			menuLang.append<UI::Text>("english", colorText, 22)
				.setId("langEnglish")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
			menuLang.append<UI::Text>("finnish", colorText, 22)
				.setId("langFinnish")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
			menuLang.append<UI::Text>("russian", colorText, 22)
				.setId("langRussian")
				.setPadding(8, 8, 4, 4)
				.setMargin(4);
		auto &menuButton = menuCenter.append<UI::ListHorizontal>(UI::Align::Center);
			menuButton.append<UI::Text>("start", colorText, 40)
				.setId("start")
				.setPadding(0, 8)
				.setMargin(12, 0);
			menuButton.append<UI::Text>("quit", colorText, 40)
				.setId("quit")
				.setPadding(0, 8)
				.setMargin(12, 0);
	ui.end(UI::Align::Center, UI::Align::Center);

	auto &result = ui.begin<UI::ListVertical>(UI::Align::Center);
		result.setId("result");
		result.setColor(Vec4 { 0.9f });
		result.setPadding(20, 0);
		result.append<UI::Text>(" ", colorText, 100)
			.setId("resultText")
			.setPadding(0, 0, 20, 0);
		auto &resultButton = result.append<UI::ListHorizontal>(UI::Align::Center);
			resultButton.append<UI::Text>("menu", colorText, 40)
				.setId("resultMenu")
				.setPadding(0, 8)
				.setMargin(12, 0);
			resultButton.append<UI::Text>("quit", colorText, 40)
				.setId("resultQuit")
				.setPadding(0, 8)
				.setMargin(12, 0);
	ui.end(UI::Align::Center, UI::Align::Center);

	auto &gameTopLeft = ui.begin<UI::ListVertical>(UI::Align::Begin);
		gameTopLeft.setId("gameTopLeft");
		auto &listTopLeft1 = gameTopLeft.append<UI::ListHorizontal>(UI::Align::Center);
			listTopLeft1.setMargin(8);
			listTopLeft1.append<UI::Text>("-", colorText, 20)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(0, 8, 0, 0)
				.setId("money");
			listTopLeft1.append<UI::Text>("-", colorText, 20)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(0, 8, 0, 0)
				.setId("time");
	ui.end(UI::Align::Begin, UI::Align::Begin);

	auto &gameTopRight = ui.begin<UI::ListHorizontal>(UI::Align::Center);
		gameTopRight.setId("gameTopRight");
		gameTopRight.setMargin(8);
		gameTopRight.append<UI::Text>(">", colorText, 20)
			.setBorder(colorBorder, 2, 0, 2, 2)
			.setColor(colorBgActive)
			.setPadding(8, 8, 6, 2)
			.setId("speed1");
		gameTopRight.append<UI::Text>(">>", colorText, 20)
			.setBorder(colorBorder, 2, 0, 2, 2)
			.setColor(colorBg)
			.setPadding(8, 8, 6, 2)
			.setId("speed2");
		gameTopRight.append<UI::Text>(">>>", colorText, 20)
			.setBorder(colorBorder, 2)
			.setColor(colorBg)
			.setPadding(8, 8, 6, 2)
			.setId("speed3");
		gameTopRight.append<UI::Text>("menu", colorText, 20)
			.setBorder(colorBorder, 2)
			.setColor(colorBg)
			.setPadding(8, 4)
			.setMargin(8, 0, 0, 0)
			.setId("menu");
	ui.end(UI::Align::End, UI::Align::Begin);

	auto &gameBottom = ui.begin<UI::ListHorizontal>(UI::Align::End);
		gameBottom.setId("gameBottom");
		auto &listMenu = gameBottom.append<UI::ListVertical>(UI::Align::Begin);
			listMenu.append<UI::Text>("stats", colorText, 32)
				.setId("buttonStats")
				.setBorder(colorBorder, 2)
				.setColor(colorBg)
				.setMargin(8, 8, 8, 0)
				.setPadding(8, 8, 6, 2);
			listMenu.append<UI::Text>("build", colorText, 32)
				.setId("buttonBuild")
				.setBorder(colorBorder, 2)
				.setColor(colorBg)
				.setMargin(8, 8, 8, 8)
				.setPadding(8, 8, 6, 2);

		auto &listStats = gameBottom.append<UI::ListVertical>(UI::Align::Begin);
			listStats.setId("listStats");
			listStats.setColor(colorBg);
			listStats.setBorder(colorBorder, 2);
			listStats.setMargin(0, 0, 0, 8);
			listStats.setPadding(0, 4);
			listStats.append<UI::Text>("-", colorText, 22)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(8, 4)
				.setId("jeepCount");
			listStats.append<UI::Text>("-", colorText, 22)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(8, 4)
				.setId("enRouteCount");
			listStats.append<UI::Text>("-", colorText, 22)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(8, 4)
				.setId("waitingVisitorsCount");
			listStats.append<UI::Text>("-", colorText, 22)
				.setBorder(colorBorder, 2)
				.setColor(colorBgActive)
				.setPadding(8, 8, 4, 2)
				.setMargin(8, 4)
				.setId("rating");

		auto &listBuild = gameBottom.append<UI::ListVertical>(UI::Align::Begin);
			listBuild.setId("listBuild");
			listBuild.setMargin(0, 0, 0, 8);
			auto &listTabs = listBuild.append<UI::ListHorizontal>(UI::Align::Begin);
				listTabs.append<UI::Text>("stuff", colorText, 22)
					.setId("buttonStructures")
					.setColor(colorBg)
					.setPadding(6, 4)
					.setBorder(colorBorder, 2, 2, 2, 0);
				listTabs.append<UI::Text>("plants", colorText, 22)
					.setId("buttonPlants")
					.setColor(colorBg)
					.setPadding(6, 4)
					.setBorder(colorBorder, 0, 2, 2, 0);
				listTabs.append<UI::Text>("extra", colorText, 22)
					.setId("buttonExtras")
					.setColor(colorBg)
					.setPadding(6, 4)
					.setBorder(colorBorder, 0, 2, 2, 0);
			auto &listStructures = listBuild.append<UI::ListHorizontal>(UI::Align::Begin);
				listStructures.setId("listStructures");
				listStructures.setBorder(colorBorder, 2);
				auto &listStructure1 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure1.setId("structure1");
					listStructure1.setPadding(8);
					listStructure1.append<UI::Image>(road, Ivec2 {100});
					listStructure1.append<UI::Text>("road", colorText, 22)
						.setId("textRoad")
						.setMargin(0, 8);
					listStructure1.setBorder(colorBorder, 0, 2, 0, 0);
					listStructure1.append<UI::Text>("free", colorText, 22)
						.setId("roadPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);
				auto &listStructure2 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure2.setId("structure2");
					listStructure2.setPadding(8);
					listStructure2.append<UI::Image>(camera, Ivec2 {100});
					listStructure2.append<UI::Text>("camera", colorText, 22)
						.setId("textCamera")
						.setMargin(0, 8);
					listStructure2.setBorder(colorBorder, 0, 2, 0, 0);
					listStructure2.append<UI::Text>("free", colorText, 22)
						.setId("cameraPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);
				auto &listStructure3 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure3.setId("structure3");
					listStructure3.setPadding(8);
					listStructure3.append<UI::Image>(droneStation, Ivec2 {100});
					listStructure3.append<UI::Text>("drone station", colorText, 22)
						.setId("textDroneStation")
						.setMargin(0, 8);
					listStructure3.setBorder(colorBorder, 0, 2, 0, 0);
					listStructure3.append<UI::Text>("free", colorText, 22)
						.setId("droneStationPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);
				auto &listStructure4 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure4.setId("structure4");
					listStructure4.setPadding(8);
					listStructure4.append<UI::Image>(airship, Ivec2 {100});
					listStructure4.append<UI::Text>("balloon", colorText, 22)
						.setId("textBalloon")
						.setMargin(0, 8);
					listStructure4.setBorder(colorBorder, 0, 2, 0, 0);
					listStructure4.append<UI::Text>("free", colorText, 22)
						.setId("balloonPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);
				auto &listStructure5 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure5.setId("structure5");
					listStructure5.setPadding(8);
					listStructure5.append<UI::Image>(hunterHut, Ivec2 {100});
					listStructure5.append<UI::Text>("hunter hut", colorText, 22)
						.setId("textHunterHut")
						.setMargin(0, 8);
					listStructure5.setBorder(colorBorder, 0, 2, 0, 0);
					listStructure5.append<UI::Text>("free", colorText, 22)
						.setId("hunterHutPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);
				auto &listStructure6 = listStructures.append<UI::ListVertical>(UI::Align::Center);
					listStructure6.setId("structure6");
					listStructure6.setPadding(8);
					listStructure6.append<UI::Image>(water, Ivec2 {100});
					listStructure6.append<UI::Text>("water", colorText, 22)
						.setId("textWater")
						.setMargin(0, 8);
					listStructure6.append<UI::Text>("free", colorText, 22)
						.setId("waterPrice")
						.setBorder(colorBorder, 2)
						.setPadding(8, 8, 8, 4);

				auto &listPlants = listBuild.append<UI::ListHorizontal>(UI::Align::Begin);
					listPlants.setId("listPlants");
					listPlants.setBorder(colorBorder, 2);
					auto &listPlant1 = listPlants.append<UI::ListVertical>(UI::Align::Center);
						listPlant1.setId("plant1");
						listPlant1.setPadding(8);
						listPlant1.append<UI::Image>(Texture::fromImage("reedb1.png"), Ivec2 {100});
						listPlant1.append<UI::Text>("cane", colorText, 22)
							.setId("textCane")
							.setMargin(0, 8);
						listPlant1.setBorder(colorBorder, 0, 2, 0, 0);
						listPlant1.append<UI::Text>("free", colorText, 22)
							.setId("canePrice")
							.setBorder(colorBorder, 2)
							.setPadding(8, 8, 8, 4);
					auto &listPlant2 = listPlants.append<UI::ListVertical>(UI::Align::Center);
						listPlant2.setId("plant2");
						listPlant2.setPadding(8);
						listPlant2.append<UI::Image>(Texture::fromImage("normalGrass.png"), Ivec2 {100});
						listPlant2.append<UI::Text>("normal grass", colorText, 22)
							.setId("textNormalGrass")
							.setMargin(0, 8);
						listPlant2.setBorder(colorBorder, 0, 2, 0, 0);
						listPlant2.append<UI::Text>("free", colorText, 22)
							.setId("normalGrassPrice")
							.setBorder(colorBorder, 2)
							.setPadding(8, 8, 8, 4);
					auto &listPlant3 = listPlants.append<UI::ListVertical>(UI::Align::Center);
						listPlant3.setId("plant3");
						listPlant3.setPadding(8);
						listPlant3.append<UI::Image>(Texture::fromImage("redGrass.png"), Ivec2 {100});
						listPlant3.append<UI::Text>("red grass", colorText, 22)
							.setId("textRedGrass")
							.setMargin(0, 8);
						listPlant3.setBorder(colorBorder, 0, 2, 0, 0);
						listPlant3.append<UI::Text>("free", colorText, 22)
							.setId("redGrassPrice")
							.setBorder(colorBorder, 2)
							.setPadding(8, 8, 8, 4);
					auto &listPlant4 = listPlants.append<UI::ListVertical>(UI::Align::Center);
						listPlant4.setId("plant4");
						listPlant4.setPadding(8);
						listPlant4.append<UI::Image>(Texture::fromImage("exoticGrass.png"), Ivec2 {100});
						listPlant4.append<UI::Text>("exotic grass", colorText, 22)
							.setId("textExoticGrass")
							.setMargin(0, 8);
						listPlant4.append<UI::Text>("free", colorText, 22)
							.setId("exoticGrassPrice")
							.setBorder(colorBorder, 2)
							.setPadding(8, 8, 8, 4);

				auto &listExtras = listBuild.append<UI::ListHorizontal>(UI::Align::Begin);
						listExtras.setId("listExtras");
						listExtras.setBorder(colorBorder, 2);
						auto &listextra1 = listExtras.append<UI::ListVertical>(UI::Align::Center);
							listextra1.setId("extra1");
							listextra1.setPadding(8);
							listextra1.append<UI::Image>(hunter, Ivec2 {100});
							listextra1.append<UI::Text>("hunter", colorText, 22)
								.setId("textHunter")
								.setMargin(0, 8);
							listextra1.setBorder(colorBorder, 0, 2, 0, 0);
							listextra1.append<UI::Text>("free", colorText, 22)
								.setId("hunterPrice")
								.setBorder(colorBorder, 2)
								.setPadding(8, 8, 8, 4);
						auto &listextra2 = listExtras.append<UI::ListVertical>(UI::Align::Center);
							listextra2.setId("extra2");
							listextra2.setPadding(8);
							listextra2.append<UI::Image>(placeholder, Ivec2 {100});
							listextra2.append<UI::Text>("tracker", colorText, 22)
								.setId("textTracker")
								.setMargin(0, 8);
							listextra2.setBorder(colorBorder, 0, 2, 0, 0);
							listextra2.append<UI::Text>("free", colorText, 22)
								.setId("trackerPrice")
								.setBorder(colorBorder, 2)
								.setPadding(8, 8, 8, 4);
						auto &listextra3 = listExtras.append<UI::ListVertical>(UI::Align::Center);
							listextra3.setId("extra3");
							listextra3.setPadding(8);
							listextra3.append<UI::Image>(jeep, Ivec2 {100});
							listextra3.append<UI::Text>("jeep", colorText, 22)
								.setId("textJeep")
								.setMargin(0, 8);
							listextra3.setBorder(colorBorder, 0, 2, 0, 0);
							listextra3.append<UI::Text>("free", colorText, 22)
								.setId("jeepPrice")
								.setBorder(colorBorder, 2)
								.setPadding(8, 8, 8, 4);
						auto &listextra4 = listExtras.append<UI::ListVertical>(UI::Align::Center);
							listextra4.setId("extra4");
							listextra4.setPadding(8);
							listextra4.append<UI::Image>(placeholder, Ivec2 {100});
							listextra4.append<UI::Text>("poacher", colorText, 22)
								.setId("textPoacher")
								.setMargin(0, 8);
							listextra4.append<UI::Text>("free", colorText, 22)
								.setId("poacherPrice")
								.setBorder(colorBorder, 2)
								.setPadding(8, 8, 8, 4);

	ui.end(UI::Align::Begin, UI::Align::End);

	toggleLang("english");

	openMenu();
}

void UIBehaviour::openMenu()
{
	auto &ui = getEntity().getComponent<UI>();

	ui.get("menuCenter").unhide();
	ui.get("gameTopLeft").hide();
	ui.get("gameTopRight").hide();
	ui.get("gameBottom").hide();
	ui.get("result").hide();

	clickMenu = true;
	clickGame = false;

	speed = {};
	mode = {};

	ui.get("buttonStats").setColor(colorBg);
	ui.get("buttonBuild").setColor(colorBg);
	
	toggleDiff("easy");
}

void UIBehaviour::openGame()
{
	auto &ui = getEntity().getComponent<UI>();

	clickGame = true;

	ui.get("menuCenter").hide();
	ui.get("gameTopLeft").unhide();
	ui.get("gameTopRight").unhide();
	ui.get("gameBottom").unhide();
	ui.get("result").hide();

	setMoney(1956);
	setCount(0, "jeepCount");
	setCount(0, "enRouteCount");
	setCount(0, "waitingVisitorsCount");
	setRating(1.0f);

	ui.get("listStats").hide();
	ui.get("listBuild").hide();
}

void UIBehaviour::openResult(bool won)
{
	auto &ui = getEntity().getComponent<UI>();

	if (won) {
		if (strcmp(language, "english") == 0) {
			ui.get("resultText").changeText("YOU WON");
		}
		else if (strcmp(language, "finnish") == 0) {
			ui.get("resultText").changeText("VOITIT");
		}
		else if (strcmp(language, "russian") == 0) {
			ui.get("resultText").changeText("ВЫ ПОБЕДИЛИ");
		}
		else {
			assert(false);
		}
	}
	else {
		if (strcmp(language, "english") == 0) {
			ui.get("resultText").changeText("YOU LOST");
		}
		else if (strcmp(language, "finnish") == 0) {
			ui.get("resultText").changeText("HÄVISIT");
		}
		else if (strcmp(language, "russian") == 0) {
			ui.get("resultText").changeText("ВЫ ПРОИГРАЛИ");
		}
		else {
			assert(false);
		}
	}

	ui.get("menuCenter").hide();
	ui.get("gameTopLeft").hide();
	ui.get("gameTopRight").hide();
	ui.get("gameBottom").hide();
	ui.get("result").unhide();

	mode = {};
}

void UIBehaviour::toggleDiff(const char *diff)
{
	auto &ui = getEntity().getComponent<UI>();
	ui.get("diffEasy").setBorder(strcmp(diff, "easy") == 0 ? colorBorderOn : colorBorderOff, 2);
	ui.get("diffNormal").setBorder(strcmp(diff, "normal") == 0 ? colorBorderOn : colorBorderOff, 2);
	ui.get("diffHard").setBorder(strcmp(diff, "hard") == 0 ? colorBorderOn : colorBorderOff, 2);
	difficulty = diff;
}

void UIBehaviour::toggleLang(const char *lang)
{
	language = lang;
	
	auto &ui = getEntity().getComponent<UI>();
	ui.get("langEnglish").setBorder(strcmp(lang, "english") == 0 ? colorBorderOn : colorBorderOff, 2);
	ui.get("langFinnish").setBorder(strcmp(lang, "finnish") == 0 ? colorBorderOn : colorBorderOff, 2);
	ui.get("langRussian").setBorder(strcmp(lang, "russian") == 0 ? colorBorderOn : colorBorderOff, 2);

	if (strcmp(lang, "english") == 0) {
		
		ui.get("diffEasy").changeText("easy");
		ui.get("diffNormal").changeText("normal");
		ui.get("diffHard").changeText("hard");
		ui.get("langEnglish").changeText("english");
		ui.get("langFinnish").changeText("finnish");
		ui.get("langRussian").changeText("russian");
		ui.get("start").changeText("start");
		ui.get("quit").changeText("quit");

		ui.get("menu").changeText("menu");
		
		ui.get("resultMenu").changeText("menu");
		ui.get("resultQuit").changeText("quit");

		ui.get("buttonStats").changeText("stats");
		ui.get("buttonBuild").changeText("build");

		ui.get("buttonStructures").changeText("stuff");
		ui.get("buttonPlants").changeText("plants");
		ui.get("buttonExtras").changeText("extra");

		ui.get("roadPrice").changeText("free");
		ui.get("cameraPrice").changeText("free");
		ui.get("droneStationPrice").changeText("free");
		ui.get("balloonPrice").changeText("free");
		ui.get("hunterHutPrice").changeText("free");
		ui.get("waterPrice").changeText("free");
		ui.get("canePrice").changeText("free");
		ui.get("normalGrassPrice").changeText("free");
		ui.get("redGrassPrice").changeText("free");
		ui.get("exoticGrassPrice").changeText("free");
		ui.get("hunterPrice").changeText("free");
		ui.get("trackerPrice").changeText("free");
		ui.get("jeepPrice").changeText("free");
		ui.get("poacherPrice").changeText("free");

		ui.get("textRoad").changeText("road");
		ui.get("textCamera").changeText("camera");
		ui.get("textDroneStation").changeText("drone station");
		ui.get("textBalloon").changeText("balloon");
		ui.get("textHunterHut").changeText("hunter hut");
		ui.get("textWater").changeText("water");
		ui.get("textHunter").changeText("hunter");
		ui.get("textTracker").changeText("tracker");
		ui.get("textJeep").changeText("jeep");
		ui.get("textPoacher").changeText("poacher");
		ui.get("textCane").changeText("cane");
		ui.get("textNormalGrass").changeText("normal grass");
		ui.get("textRedGrass").changeText("red grass");
		ui.get("textExoticGrass").changeText("exotic grass");
	
	}

	if (strcmp(lang, "finnish") == 0) {
		
		ui.get("diffEasy").changeText("helppo");
		ui.get("diffNormal").changeText("normaali");
		ui.get("diffHard").changeText("vaikea");
		ui.get("langEnglish").changeText("englanti");
		ui.get("langFinnish").changeText("suomi");
		ui.get("langRussian").changeText("venäjä");
		ui.get("start").changeText("aloita");
		ui.get("quit").changeText("lopeta");

		ui.get("menu").changeText("valikko");

		ui.get("resultMenu").changeText("valikko");
		ui.get("resultQuit").changeText("lopeta");

		ui.get("buttonStats").changeText("tilastot");
		ui.get("buttonBuild").changeText("rakenna");

		ui.get("buttonStructures").changeText("tavarat");
		ui.get("buttonPlants").changeText("kasvit");
		ui.get("buttonExtras").changeText("lisä");

		ui.get("roadPrice").changeText("ilmainen");
		ui.get("cameraPrice").changeText("ilmainen");
		ui.get("droneStationPrice").changeText("ilmainen");
		ui.get("balloonPrice").changeText("ilmainen");
		ui.get("hunterHutPrice").changeText("ilmainen");
		ui.get("waterPrice").changeText("ilmainen");
		ui.get("canePrice").changeText("ilmainen");
		ui.get("normalGrassPrice").changeText("ilmainen");
		ui.get("redGrassPrice").changeText("ilmainen");
		ui.get("exoticGrassPrice").changeText("ilmainen");
		ui.get("hunterPrice").changeText("ilmainen");
		ui.get("trackerPrice").changeText("ilmainen");
		ui.get("jeepPrice").changeText("ilmainen");
		ui.get("poacherPrice").changeText("ilmainen");

		ui.get("textRoad").changeText("tie");
		ui.get("textCamera").changeText("kamera");
		ui.get("textDroneStation").changeText("drooniasema");
		ui.get("textBalloon").changeText("ilmapallo");
		ui.get("textHunterHut").changeText("metsästäjän maja");
		ui.get("textWater").changeText("vesi");
		ui.get("textHunter").changeText("metsästäjä");
		ui.get("textTracker").changeText("jäljittäjä");
		ui.get("textJeep").changeText("maastoauto");
		ui.get("textPoacher").changeText("salametsästäjä");		
		ui.get("textCane").changeText("sokeriruoko");		
		ui.get("textNormalGrass").changeText("normaali ruoho");
		ui.get("textRedGrass").changeText("punainen ruoho");
		ui.get("textExoticGrass").changeText("eksoottinen ruoho");
	
	}

	if (strcmp(lang, "russian") == 0) {
		
		ui.get("diffEasy").changeText("легко");
		ui.get("diffNormal").changeText("нормально");
		ui.get("diffHard").changeText("трудно");
		ui.get("langEnglish").changeText("английский");
		ui.get("langFinnish").changeText("финский");
		ui.get("langRussian").changeText("русский");
		ui.get("start").changeText("начать");
		ui.get("quit").changeText("выйти");

		ui.get("menu").changeText("меню");

		ui.get("resultMenu").changeText("меню");
		ui.get("resultQuit").changeText("выйти");

		ui.get("buttonStats").changeText("статистика");
		ui.get("buttonBuild").changeText("построить");

		ui.get("buttonStructures").changeText("вещи");
		ui.get("buttonPlants").changeText("растения");
		ui.get("buttonExtras").changeText("дополнительно");

		ui.get("roadPrice").changeText("бесплатно");
		ui.get("cameraPrice").changeText("бесплатно");
		ui.get("droneStationPrice").changeText("бесплатно");
		ui.get("balloonPrice").changeText("бесплатно");
		ui.get("hunterHutPrice").changeText("бесплатно");
		ui.get("waterPrice").changeText("бесплатно");
		ui.get("canePrice").changeText("бесплатно");
		ui.get("normalGrassPrice").changeText("бесплатно");
		ui.get("redGrassPrice").changeText("бесплатно");
		ui.get("exoticGrassPrice").changeText("бесплатно");
		ui.get("hunterPrice").changeText("бесплатно");
		ui.get("trackerPrice").changeText("бесплатно");
		ui.get("jeepPrice").changeText("бесплатно");
		ui.get("poacherPrice").changeText("бесплатно");

		ui.get("textRoad").changeText("дорога");
		ui.get("textCamera").changeText("камера");
		ui.get("textDroneStation").changeText("станция дронов");
		ui.get("textBalloon").changeText("воздушный шар");
		ui.get("textHunterHut").changeText("хижина охотника");
		ui.get("textWater").changeText("вода");
		ui.get("textHunter").changeText("охотник");
		ui.get("textTracker").changeText("следопыт");
		ui.get("textJeep").changeText("джип");
		ui.get("textPoacher").changeText("тростник");
		ui.get("textCane").changeText("тростник");
		ui.get("textNormalGrass").changeText("нормальная трава");
		ui.get("textRedGrass").changeText("красная трава");
		ui.get("textExoticGrass").changeText("экзотическая трава");
	
	}

}

void UIBehaviour::earlyUpdate(Entity &entity)
{
	UI &ui = entity.getComponent<UI>();
	ui.interact();

	speed = {};

	clickMenu = false;
	clickGame = false;

	auto &speed1 = ui.get("speed1");
	auto &speed2 = ui.get("speed2");
	auto &speed3 = ui.get("speed3");

	auto &buttonStats = ui.get("buttonStats");
	auto &buttonBuild = ui.get("buttonBuild");
	auto &listStats = ui.get("listStats");
	auto &listBuild = ui.get("listBuild");

	auto &buttonStructures = ui.get("buttonStructures");
	auto &buttonPlants = ui.get("buttonPlants");
	auto &buttonExtras = ui.get("buttonExtras");
	auto &listStructures = ui.get("listStructures");
	auto &listPlants = ui.get("listPlants");
	auto &listExtras = ui.get("listExtras");

	for (const std::string &id : ui.getClicks()) {
		if (id == "resultMenu") {
			openMenu();
			continue;
		}
		if (id == "resultQuit") {
			Input::quit();
			continue;
		}
		if (id == "menu") {
			openMenu();
			continue;
		}
		if (id == "start") {
			openGame();
			continue;
		}
		if (id == "quit") {
			Input::quit();
			continue;
		}
		if (id == "diffEasy") {
			toggleDiff("easy");
			continue;
		}
		if (id == "diffNormal") {
			toggleDiff("normal");
			continue;
		}
		if (id == "diffHard") {
			toggleDiff("hard");
			continue;
		}
		if (id == "langEnglish") {
			toggleLang("english");
			continue;
		}
		if (id == "langFinnish") {
			toggleLang("finnish");
			continue;
		}
		if (id == "langRussian") {
			toggleLang("russian");
			continue;
		}
		if (id == "speed1") {
			speed = 1;
			speed1.setColor(colorBgActive);
			speed2.setColor(colorBg);
			speed3.setColor(colorBg);
		}
		if (id == "speed2") {
			speed = 2;
			speed1.setColor(colorBg);
			speed2.setColor(colorBgActive);
			speed3.setColor(colorBg);
		}
		if (id == "speed3") {
			speed = 3;
			speed1.setColor(colorBg);
			speed2.setColor(colorBg);
			speed3.setColor(colorBgActive);
		}
		if (id == "buttonStats") {
			if (listStats.isHidden()) {
				buttonStats.setColor(colorBgActive);
				buttonBuild.setColor(colorBg);
				listStats.unhide();
				listBuild.hide();
			}
			else {
				buttonStats.setColor(colorBg);
				listStats.hide();
			}
		}
		if (id == "buttonBuild") {
			if (listBuild.isHidden()) {

				buttonStats.setColor(colorBg);
				buttonBuild.setColor(colorBgActive);
				listStats.hide();
				listBuild.unhide();

				buttonStructures.setColor(colorBgActive);
				buttonPlants.setColor(colorBg);
				buttonExtras.setColor(colorBg);
				listStructures.unhide();
				listPlants.hide();
				listExtras.hide();
				toggleStructure({});
				togglePlant({});
				toggleExtras({});
			}
			else {
				buttonBuild.setColor(colorBg);
				listBuild.hide();
			}
		}
		if (id == "buttonStructures") {
			buttonStructures.setColor(colorBgActive);
			buttonPlants.setColor(colorBg);
			buttonExtras.setColor(colorBg);
			listStructures.unhide();
			listPlants.hide();
			listExtras.hide();
			toggleStructure({});
			togglePlant({});
			toggleExtras({});
		}
		if (id == "buttonPlants") {
			buttonStructures.setColor(colorBg);
			buttonPlants.setColor(colorBgActive);
			buttonExtras.setColor(colorBg);
			listStructures.hide();
			listPlants.unhide();
			listExtras.hide();
			toggleStructure({});
			togglePlant({});
			toggleExtras({});
		}
		if (id == "buttonExtras") {
			buttonStructures.setColor(colorBg);
			buttonPlants.setColor(colorBg);
			buttonExtras.setColor(colorBgActive);
			listStructures.hide();
			listPlants.hide();
			listExtras.unhide();
			toggleStructure({});
			togglePlant({});
			toggleExtras({});
		}
		if (id == "structure1" || id == "structure2" || id == "structure3" || id == "structure4" || id == "structure5" || id == "structure6") {
			toggleStructure(id);
		}
		if (id == "plant1" || id == "plant2" || id == "plant3" || id == "plant4") {
			togglePlant(id);
		}
		if (id == "extra1" || id == "extra2" || id == "extra3" || id == "extra4") {
			toggleExtras(id);
		}
	}
}

void UIBehaviour::setMoney(Uint money) const
{
	char buffer[0xFF];
	snprintf(buffer, sizeof (buffer), "%u€", money);
	auto &ui = Labels::getUnique("global").getComponent<UI>();
	auto &element = ui.get("money");
	element.changeText(buffer);
}

void UIBehaviour::setRating(float rating) const
{
	char buffer[0xFF];
	
	if (strcmp(language, "english") == 0) {
		snprintf(buffer, sizeof (buffer), "rating: %.2f", rating);
	}
	else if (strcmp(language, "finnish") == 0) {
		snprintf(buffer, sizeof (buffer), "arvosana: %.2f", rating);
	}
	else if (strcmp(language, "russian") == 0) {
		snprintf(buffer, sizeof (buffer), "рейтинг: %.2f", rating);
	}
	else {
		assert(false);
	}
	
	auto &ui = Labels::getUnique("global").getComponent<UI>();
	auto &element = ui.get("rating");
	element.changeText(buffer);
}

void UIBehaviour::setCount(Uint count, std::string name) const
{
	char buffer[0xFF];
	auto &ui = Labels::getUnique("global").getComponent<UI>();
	auto &element = ui.get(name);

	if (name == "jeepCount")
	{
		if (strcmp(language, "english") == 0) {
			snprintf(buffer, sizeof (buffer), "jeeps: %u", count);
		}
		else if (strcmp(language, "finnish") == 0) {
			snprintf(buffer, sizeof (buffer), "maastoautoja: %u", count);
		}
		else if (strcmp(language, "russian") == 0) {
			snprintf(buffer, sizeof (buffer), "джипов: %u", count);
		}
		else {
			assert(false);
		}
	}
	else if (name == "waitingVisitorsCount")
	{
		if (strcmp(language, "english") == 0) {
			snprintf(buffer, sizeof (buffer), "visitors waiting: %u", count);
		}
		else if (strcmp(language, "finnish") == 0) {
			snprintf(buffer, sizeof (buffer), "vierailijoita odottaa: %u", count);
		}
		else if (strcmp(language, "russian") == 0) {
			snprintf(buffer, sizeof (buffer), "посетителей в ожидании: %u", count);
		}
		else {
			assert(false);
		}		
	}
	else if (name == "enRouteCount")
	{
		if (strcmp(language, "english") == 0) {
			snprintf(buffer, sizeof (buffer), "moving jeeps: %u", count);
		}
		else if (strcmp(language, "finnish") == 0) {
			snprintf(buffer, sizeof (buffer), "liikkuvia maastoautoja: %u", count);
		}
		else if (strcmp(language, "russian") == 0) {
			snprintf(buffer, sizeof (buffer), "движущихся джипов: %u", count);
		}
		else {
			assert(false);
		}
	}
	element.changeText(buffer);
}

void UIBehaviour::setPrice(Uint price, std::string name) const
{

	char buffer[0xFF];
	snprintf(buffer, sizeof (buffer), "%u€", price);
	auto &ui = Labels::getUnique("global").getComponent<UI>();
	auto &element = ui.get(name);
	element.changeText(buffer);
}

void UIBehaviour::setTime(Uint days, Uint hours, Uint minutes) const
{
	char buffer[0xFF];
	
	if (strcmp(language, "english") == 0) {
		snprintf(buffer, sizeof (buffer), "day %u, %02u:%02u", days, hours, minutes);
	}
	else if (strcmp(language, "finnish") == 0) {
		snprintf(buffer, sizeof (buffer), "päivä %u, %02u:%02u", days, hours, minutes);
	}
	else if (strcmp(language, "russian") == 0) {
		snprintf(buffer, sizeof (buffer), "день %u, %02u:%02u", days, hours, minutes);
	}
	else {
		assert(false);
	}
	
	auto &ui = Labels::getUnique("global").getComponent<UI>();
	auto &element = ui.get("time");
	element.changeText(buffer);
}

void UIBehaviour::toggleStructure(std::optional<std::string> id)
{
	auto &ui = getEntity().getComponent<UI>();
	auto &entity1 = ui.get("structure1");
	auto &entity2 = ui.get("structure2");
	auto &entity3 = ui.get("structure3");
	auto &entity4 = ui.get("structure4");
	auto &entity5 = ui.get("structure5");
	auto &entity6 = ui.get("structure6");
	if (!id || (mode && *mode == *id)) {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		entity5.setColor(colorBg);
		entity6.setColor(colorBg);
		mode = {};
		return;
	}
	if (*id == "structure1") {
		entity1.setColor(colorBgActive);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		entity5.setColor(colorBg);
		entity6.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "structure2") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBgActive);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		entity5.setColor(colorBg);
		entity6.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "structure3") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBgActive);
		entity4.setColor(colorBg);
		entity5.setColor(colorBg);
		entity6.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "structure4") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBgActive);
		entity5.setColor(colorBg);
		entity6.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "structure5") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		entity5.setColor(colorBgActive);
		entity6.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "structure6") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		entity5.setColor(colorBg);
		entity6.setColor(colorBgActive);
		mode = *id;
		return;
	}
	assert(false);
}

void UIBehaviour::togglePlant(std::optional<std::string> id)
{
	auto &ui = getEntity().getComponent<UI>();
	auto &entity1 = ui.get("plant1");
	auto &entity2 = ui.get("plant2");
	auto &entity3 = ui.get("plant3");
	auto &entity4 = ui.get("plant4");
	if (!id || (mode && *mode == *id)) {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = {};
		return;
	}
	if (*id == "plant1") {
		entity1.setColor(colorBgActive);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "plant2") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBgActive);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "plant3") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBgActive);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "plant4") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBgActive);
		mode = *id;
		return;
	}
	assert(false);
}

void UIBehaviour::toggleExtras(std::optional<std::string> id)
{
	auto &ui = getEntity().getComponent<UI>();
	auto &entity1 = ui.get("extra1");
	auto &entity2 = ui.get("extra2");
	auto &entity3 = ui.get("extra3");
	auto &entity4 = ui.get("extra4");
	if (!id || (mode && *mode == *id)) {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = {};
		return;
	}
	if (*id == "extra1") {
		entity1.setColor(colorBgActive);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "extra2") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBgActive);
		entity3.setColor(colorBg);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "extra3") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBgActive);
		entity4.setColor(colorBg);
		mode = *id;
		return;
	}
	if (*id == "extra4") {
		entity1.setColor(colorBg);
		entity2.setColor(colorBg);
		entity3.setColor(colorBg);
		entity4.setColor(colorBgActive);
		mode = *id;
		return;
	}
	assert(false);
}

