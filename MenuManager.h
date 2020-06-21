#pragma once
#include "OLCBehavior.h"
#include "GUIComponent.h"

class MenuManager : OLCBehavior {
private:
	gui::Table mainMenu, choiceDificult, exiting;
	gui::Table& current = mainMenu;

public:
	MenuManager() {}
	~MenuManager() {}

	bool OnUserCreate(olc::PixelGameEngine& olc) override {
		choiceDificult.getTitle().setText("Choice the dificult:");
		choiceDificult.setAmountVisibleCells(5, 1);
		choiceDificult.addButton("1");
		choiceDificult.addButton("2");
		choiceDificult.addButton("3");
		choiceDificult.addButton("4");
		choiceDificult.addButton("5");

		exiting.getTitle().setText("Do you really want exit?");
		exiting.setAmountVisibleCells(1, 2);
		exiting.addButton("yes");
		exiting.addButton("no");
		
		mainMenu.getTitle().setText("Chess");
		mainMenu.setAmountVisibleCells(2, 1);
		mainMenu.addButton("play");
		mainMenu.addButton("exit");

		mainMenu.onUserCreate(olc);

		return true;
	}

	bool OnUserUpdate(olc::PixelGameEngine& olc, const float& fElapsedTime) override {

		current.onUserUpdate(olc, fElapsedTime, {200,200});

		return true;
	}
};