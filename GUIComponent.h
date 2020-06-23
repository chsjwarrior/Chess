#pragma once
#include"olcPixelGameEngine.h"

namespace gui {

	constexpr uint8_t PIXEL = 8;

	class Component {
	public:
		Component(Component&) = delete;
		Component& operator=(Component&) = delete;
		Component() = default;
		virtual ~Component() {}

		virtual void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) = 0;

		virtual const int32_t getWidth() const = 0;

		virtual const int32_t getHeight() const = 0;
	};

	class Label : public Component {
	protected:
		uint8_t scale;
		olc::Pixel textColor;
		std::string text;

	public:
		Label() : Component(), text("label"), textColor(olc::BLACK), scale(1) {}
		explicit Label(const std::string& text, olc::Pixel textColor = olc::BLACK, uint8_t scale = 1) : Component(), text(text), textColor(textColor), scale(scale) {}
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
		const uint8_t PADDING = 5;
		bool mousePressed = false;
		bool mouseReleased = false;
		olc::Pixel backgroundColor = olc::GREY;
		olc::Pixel highLightColor = olc::WHITE;

	public:
		Button() : Label("button") {}
		explicit Button(const std::string& text, uint8_t scale = 1) : Label(text, olc::BLACK, scale) {}
		~Button() {}

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(screenOffset.x + getWidth(), screenOffset.y + getHeight());
			olc::vi2d point(olc.GetMouseX(), olc.GetMouseY());

			olc::Pixel color = backgroundColor;

			//=================mouseListener=================
			if ((point.x >= screenOffset.x && point.y >= screenOffset.y) &&
				(point.x <= temp.x && point.y <= temp.y)) {

				color = highLightColor;

				mousePressed = (olc.GetMouse(0).bPressed || olc.GetMouse(1).bPressed || olc.GetMouse(2).bPressed);
				mouseReleased = (olc.GetMouse(0).bReleased || olc.GetMouse(1).bReleased || olc.GetMouse(2).bReleased);

			} else if (mousePressed || mouseReleased) {
				mousePressed = false;
				mouseReleased = mousePressed;
			}
			//=================mouseListener=================

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

		void setBackgroundColor(olc::Pixel color) {
			backgroundColor = color;
		}

		void setHighLightColor(olc::Pixel color) {
			highLightColor = color;
		}

		const bool isMousePressed() const {
			return mousePressed;
		}

		const bool isMouseReleased() const {
			return mouseReleased;
		}
	};

	/*
	this class is probably temporary.
	I want to implement a container class with layout manager.
	*/
	class Table : public Component {
	private:
		const uint8_t TITLE_PADDING = 10, TABLE_PADDING = 3;
		mutable int16_t cellWidth = 1, cellHeight = 1;
		int16_t amountRows = 1, amountColumns = 1;
		olc::Pixel backgroudColor;
		Label title;

		const int32_t getTitleWidth() const {
			return title.getWidth() + TITLE_PADDING + TITLE_PADDING;
		}

		const int32_t getTitleHeight() const {
			return title.getHeight() + TITLE_PADDING + TITLE_PADDING;
		}

	public:
		Table() : Component(), backgroudColor(olc::DARK_GREY), title("Table", olc::BLACK) {}
		explicit Table(const std::string& title, olc::Pixel backgroudColor = olc::DARK_GREY) : Component(), backgroudColor(backgroudColor), title(title, olc::BLACK) {}
		~Table() {}

		std::list<Component*> items;

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(getWidth(), getHeight());

			olc.FillRect(screenOffset, temp, backgroudColor);

			temp.x = screenOffset.x + TITLE_PADDING;
			temp.y = screenOffset.y + TITLE_PADDING;
			title.onUserUpdate(olc, elapsedTime, temp);

			if (items.empty())
				return;

			int16_t column = 0, row = 0;
			for (auto item = items.begin(); item != items.end(); ++item) {
				temp.x = column * cellWidth + (screenOffset.x + TABLE_PADDING);
				temp.y = row * cellHeight + (screenOffset.y + getTitleHeight());

				(*item)->onUserUpdate(olc, elapsedTime, temp);

				if (++column == amountColumns) {
					column = 0;
					if (++row == amountRows)
						row = 0;
				}
			}
		}

		const int32_t getWidth() const override {
			if (items.empty())
				return getTitleWidth();

			cellWidth = TABLE_PADDING;
			for (auto item = items.begin(); item != items.end(); ++item)
				cellWidth = max((*item)->getWidth(), cellWidth);

			cellWidth += TABLE_PADDING + TABLE_PADDING;
			return max(amountColumns * cellWidth, getTitleWidth());
		}

		const int32_t getHeight() const override {
			if (items.empty())
				return getTitleHeight();

			cellHeight = TABLE_PADDING;
			for (auto item = items.begin(); item != items.end(); ++item)
				cellHeight = max((*item)->getHeight(), cellHeight);

			cellHeight += TABLE_PADDING + TABLE_PADDING;
			return amountRows * cellHeight + getTitleHeight();
		}

		Label& getTitle() {
			return title;
		}

		void setBackgroundColor(olc::Pixel color) {
			backgroudColor = color;
		}

		void setTable(int8_t rows, int8_t columns) {
			amountRows = rows;
			amountColumns = columns;
		}
	};
}