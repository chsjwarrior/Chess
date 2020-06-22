#pragma once
#include"olcPixelGameEngine.h"

namespace gui {

	constexpr uint8_t PIXEL = 8;

	class Component {
	public:
		Component(Component&) = delete;
		Component& operator=(Component&) = delete;
		Component() {}
		virtual ~Component() {}

		virtual void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) = 0;

		virtual const int32_t getWidth() const = 0;

		virtual const int32_t getHeight() const = 0;
	};

	class Label : public Component {
	protected:
		uint8_t scale = 1;
		olc::Pixel textColor = olc::BLACK;
		std::string text;

	public:
		Label() : Component(), text("label") {}
		explicit Label(const std::string& text, const uint8_t& scale = 1) : Component(), text(text), scale(scale) {}
		virtual ~Label() {}

		virtual void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc.DrawString(screenOffset.x, screenOffset.y, text, textColor, scale);
		}

		const int32_t getWidth() const override {
			return (int32_t) text.size() * scale * PIXEL;
		}

		const int32_t getHeight() const override {
			return scale * PIXEL;
		}

		void setScale(const uint8_t& scale) {
			this->scale = scale;
		}

		void setTextColor(const olc::Pixel& color) {
			textColor = color;
		}

		void setText(const std::string& text) {
			this->text = text;
		}
	};

	class Button : public Label {
	private:
		const uint8_t PADDING = 2;
		olc::Pixel backgroundColor = olc::GREY;
		olc::Pixel highLightColor = olc::WHITE;
		bool mousePressed = false;
		bool mouseReleased = false;

	public:
		Button() : Label("button") {}
		explicit Button(const std::string& text, const uint8_t& scale = 1) : Label(text, scale) {}
		~Button() {}

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(screenOffset.x + getWidth(), screenOffset.y + getHeight());
			olc::vi2d point(olc.GetMouseX(), olc.GetMouseY());

			olc::Pixel color = backgroundColor;

			//mouseListener
			if ((point.x >= screenOffset.x && point.y >= screenOffset.y) &&
				(point.x <= temp.x && point.y <= temp.y)) {
				color = highLightColor;

				mousePressed = (olc.GetMouse(0).bPressed || olc.GetMouse(1).bPressed || olc.GetMouse(2).bPressed);
				mouseReleased = (olc.GetMouse(0).bReleased || olc.GetMouse(1).bReleased || olc.GetMouse(2).bReleased);
			}
			//-------------------------

			temp.x = getWidth();
			temp.y = getHeight();

			olc.FillRect(screenOffset, temp, color);

			temp.x = screenOffset.x + PADDING;
			temp.y = screenOffset.y + PADDING;

			Label::onUserUpdate(olc, elapsedTime, temp);
		}

		const int32_t getWidth() const override {
			return Label::getWidth() + PADDING + PADDING;
		}

		const int32_t getHeight() const override {
			return Label::getHeight() + PADDING + PADDING;
		}

		void setBackgroundColor(olc::Pixel& color) {
			backgroundColor = color;
		}

		void setHighLightColor(olc::Pixel& color) {
			highLightColor = color;
		}

		const bool& isMousePressed() { return mousePressed; }
		const bool& isMouseReleased() { return mouseReleased; }
	};

	/*
	this class is probably temporary.
	I will want to implement a container class with layout manager.

	class Table : public Component {
	private:
		mutable int16_t cellWidth = 1, cellHeight = 1;
		int16_t amountRows = 1, amountColumns = 1;
		olc::Pixel backgroudColor;
		Label title;
		std::list<Button*> items;

	public:
		Table() : Component(), title("Table", 2) {
			title.setTextColor(olc::WHITE);
		}
		explicit Table(const std::string& title) : Component(), title(title, 2) {
			this->title.setTextColor(olc::WHITE);
		}
		~Table() {
			items.clear();
		}

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(getWidth(), getHeight());

			olc.FillRect(screenOffset, temp, backgroudColor);

			//padding
			temp.x = screenOffset.x + 1;
			temp.y = screenOffset.y + 1;
			title.onUserUpdate(olc, elapsedTime, temp);

			auto item = items.begin();
			for (uint8_t x = 0; x < amountColumns; x++) {
				for (uint8_t y = 1; y < amountRows + 1; y++) {

					temp.x = screenOffset.x + (x * cellWidth);
					temp.y = screenOffset.y + (y * cellHeight);
					if (item != items.end()) {
						(*item)->onUserUpdate(olc, elapsedTime, temp);
						//item->onUserUpdate(olc, elapsedTime, temp);
						item++;
					}

				}
			}
		}

		const int32_t getWidth() const override {
			cellWidth = title.getWidth();

			for (auto item = items.begin(); item != items.end(); ++item)
				cellWidth = max((*item)->getWidth(), cellWidth);

			return amountColumns * cellWidth;
		}

		const int32_t getHeight() const override {
			cellHeight = title.getHeight();

			for (auto item = items.begin(); item != items.end(); ++item)
				cellHeight = max((*item)->getHeight(), cellHeight);

			return (amountRows + 1) * cellHeight;
		}

		Label& getTitle() {
			return title;
		}

		void addItem(const Button* item) {
			items.push_back(item);
		}

		void removeAllItems() {
			items.clear();
		}

		void setBackgroundColor(olc::Pixel& color) {
			backgroudColor = color;
		}

		void setAmountVisibleCells(const int8_t& rows, const int8_t& columns) {
			amountRows = rows;
			amountColumns = columns;
		}
	};
	*/
}