#pragma once
#include "OLCBehavior.h"
#include "GUIComponent.h"

class MenuManager : OLCBehavior {
private:
	const uint8_t scale = 2;

	uint8_t stack = 0;
	float timer = 0;

	gui::Label mainTitle, dificultTitle, colorTitle, exitingTitle;
	gui::Button playBtn, exitBtn;
	gui::Button oneBtn, twoBtn, threBtn;
	gui::Button whiteBtn, blackBtn;
	gui::Button yesBtn, noBtn;

public:
	MenuManager() {}
	~MenuManager() {}

	bool OnUserCreate(olc::PixelGameEngine& olc) override {
		mainTitle.setText("Chess");
		mainTitle.setScale(scale);
		playBtn.setText("play");
		playBtn.setScale(scale);
		exitBtn.setText("exit");
		exitBtn.setScale(scale);

		dificultTitle.setText("Choice the dificult:");
		dificultTitle.setScale(scale);
		oneBtn.setText("1");
		oneBtn.setScale(scale);
		twoBtn.setText("2");
		twoBtn.setScale(scale);
		threBtn.setText("3");
		threBtn.setScale(scale);

		colorTitle.setText("Choice your color:");
		colorTitle.setScale(scale);
		whiteBtn.setText("Brancas");
		whiteBtn.setScale(scale);
		blackBtn.setText("Negras");
		blackBtn.setScale(scale);

		exitingTitle.setText("Do you realy want to exit?");
		exitingTitle.setScale(2);
		yesBtn.setText("yes");
		yesBtn.setScale(2);
		noBtn.setText("no");
		noBtn.setScale(2);

		return true;
	}

	bool OnUserUpdate(olc::PixelGameEngine& olc, float elapsedTime) override {
		timer += elapsedTime;

		if (stack == 0) {
			mainTitle.onUserUpdate(olc, elapsedTime, {205, 200});
			playBtn.onUserUpdate(olc, elapsedTime, {210, 220});
			exitBtn.onUserUpdate(olc, elapsedTime, {210, 245});

			if (timer >= 1) {
				timer = 0;
				if (playBtn.isMousePressed())
					stack = 1;
				else if (exitBtn.isMousePressed())
					stack = 3;
			}

		} else if (stack == 1) {
			dificultTitle.onUserUpdate(olc, elapsedTime, {60, 200});
			oneBtn.onUserUpdate(olc, elapsedTime, {210, 220});
			twoBtn.onUserUpdate(olc, elapsedTime, {210, 245});
			threBtn.onUserUpdate(olc, elapsedTime, {210,270});

			if (timer >= 1) {
				timer = 0;
				if (oneBtn.isMousePressed() || twoBtn.isMousePressed() || threBtn.isMousePressed())
					stack = 2;
			}

		} else if (stack == 2) {
			colorTitle.onUserUpdate(olc, elapsedTime, {60, 200});
			whiteBtn.onUserUpdate(olc, elapsedTime, {210, 220});
			blackBtn.onUserUpdate(olc, elapsedTime, {210, 245});
		} else if (stack == 3) {
			exitingTitle.onUserUpdate(olc, elapsedTime, {60, 200});
			yesBtn.onUserUpdate(olc, elapsedTime, {195, 220});
			noBtn.onUserUpdate(olc, elapsedTime, {255, 220});

			if (timer >= 1) {
				timer = 0;
				if (noBtn.isMousePressed())
					stack = 0;
			}
		}

		if (olc.GetKey(olc::ESCAPE).bPressed)
			if (stack != 0)
				stack = 0;

		return true;
	}
};