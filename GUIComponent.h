#pragma once
#include"olcPixelGameEngine.h"

/*
This is a simple class to create a game gui with buttons and labels.
It muss be improve to generic use in other games.
*/
namespace gui {
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
		const uint8_t PIXEL_SIZE = 8;
		uint8_t scale;
		olc::Pixel textColor;
		std::string text;

	public:
		Label() : Component(), text("label"), textColor(olc::BLACK), scale(1) {}
		explicit Label(const std::string& text, olc::Pixel textColor = olc::BLACK, uint8_t scale = 1) : Component(), text(text), textColor(textColor), scale(scale) {}
		~Label() {}

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc.DrawString(screenOffset.x, screenOffset.y, text, textColor, scale);
		}

		const int32_t getWidth() const override { return (int32_t) text.size() * scale * PIXEL_SIZE; }
		const int32_t getHeight() const override { return scale * PIXEL_SIZE; }

		const uint8_t getScale() const { return scale; }
		void setScale(const uint8_t& scale) { this->scale = scale; }

		const olc::Pixel getTextColor() const { return textColor; }
		void setTextColor(const olc::Pixel& color) { textColor = color; }

		const std::string getText() const { return text; }
		void setText(const std::string& text) { this->text = text; }
	};

	class Button : public Label {
	private:
		const uint8_t PADDING = 5;
		bool mousePressed = false;
		olc::Pixel backgroundColor = olc::GREY;
		olc::Pixel highLightColor = olc::WHITE;
		olc::Sprite* image = nullptr;

	public:
		Button(const Button& other) : Label(other.getText(), other.getTextColor(), other.getScale()), backgroundColor(other.getBackgroundColor()), highLightColor(other.getHighLightColor()), mousePressed(other.isMousePressed()) {}
		Button() : Label("button") {}
		explicit Button(const std::string& text, uint8_t scale = 1) : Label(text, olc::BLACK, scale) {}
		~Button() { image = nullptr; }

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(screenOffset.x + getWidth(), screenOffset.y + getHeight());

			olc::Pixel color = backgroundColor;
			//0=================mouseListener=================0
			if ((olc.GetMouseX() >= screenOffset.x && olc.GetMouseY() >= screenOffset.y) &&
				(olc.GetMouseX() <= temp.x && olc.GetMouseY() <= temp.y)) {

				color = highLightColor;

				mousePressed = (olc.GetMouse(0).bPressed || olc.GetMouse(1).bPressed || olc.GetMouse(2).bPressed);
			} else if (mousePressed)
				mousePressed = false;
			//0=================mouseListener=================0

			temp.x = getWidth();
			temp.y = getHeight();
			olc.FillRect(screenOffset, temp, color);

			temp.x = screenOffset.x + PADDING;
			temp.y = screenOffset.y + PADDING;
			Label::onUserUpdate(olc, elapsedTime, temp);
		}

		const bool isMousePressed() const { return mousePressed; }

		const int32_t getWidth() const override {
			if (image == nullptr)
				return Label::getWidth() + PADDING + PADDING;
			return max(image->width, Label::getWidth() + PADDING + PADDING);
		}
		const int32_t getHeight() const override {
			if (image == nullptr)
				return Label::getHeight() + PADDING + PADDING;
			return max(image->height, Label::getHeight() + PADDING + PADDING);
		}

		const olc::Pixel getBackgroundColor() const { return backgroundColor; }
		void setBackgroundColor(olc::Pixel backgroundColor) { this->backgroundColor = backgroundColor; }

		olc::Pixel getHighLightColor() const { return highLightColor; }
		void setHighLightColor(olc::Pixel color) { highLightColor = color; }
	};

	class MessageBox : public Component {
	private:
		const uint8_t MESSAGE_PADDING = 10, OPTION_PADDING = 3;
		mutable int16_t cellWidth = 1, cellHeight = 1;
		int16_t amountRows = 1, amountColumns = 1;
		olc::Pixel backgroudColor;
		Label title;
		std::map<std::string, Button> buttons;

		inline const int32_t getTitleWidth() const {
			return title.getWidth() + MESSAGE_PADDING + MESSAGE_PADDING;
		}

		inline const int32_t getTitleHeight() const {
			return title.getHeight() + MESSAGE_PADDING + MESSAGE_PADDING;
		}

	public:
		MessageBox() : Component(), backgroudColor(olc::DARK_GREY), title("Table", olc::BLACK) {}
		explicit MessageBox(const std::string& title, olc::Pixel backgroudColor = olc::DARK_GREY, uint8_t scale = 1) : Component(), backgroudColor(backgroudColor), title(title, olc::BLACK, scale) {}
		~MessageBox() {
			title.setText("");
			buttons.clear();
		}

		void onUserUpdate(olc::PixelGameEngine& olc, float elapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(getWidth(), getHeight());

			olc.FillRect(screenOffset, temp, backgroudColor);

			temp.x = screenOffset.x + MESSAGE_PADDING;
			temp.y = screenOffset.y + MESSAGE_PADDING;
			title.onUserUpdate(olc, elapsedTime, temp);

			if (buttons.empty())
				return;

			int16_t column = 0, row = 0;
			for (auto btn = buttons.begin(); btn != buttons.end(); ++btn) {
				temp.x = column * cellWidth + (screenOffset.x + OPTION_PADDING);
				temp.y = row * cellHeight + (screenOffset.y + getTitleHeight());

				btn->second.onUserUpdate(olc, elapsedTime, temp);

				if (++column == amountColumns) {
					column = 0;
					if (++row == amountRows)
						row = 0;
				}
			}
		}

		const int32_t getWidth() const override {
			if (buttons.empty())
				return getTitleWidth();

			cellWidth = OPTION_PADDING;
			for (auto btn = buttons.begin(); btn != buttons.end(); ++btn)
				cellWidth = max(btn->second.getWidth(), cellWidth);

			cellWidth += OPTION_PADDING + OPTION_PADDING;
			return max(amountColumns * cellWidth, getTitleWidth());
		}

		const int32_t getHeight() const override {
			if (buttons.empty())
				return getTitleHeight();

			cellHeight = OPTION_PADDING;
			for (auto btn = buttons.begin(); btn != buttons.end(); ++btn)
				cellHeight = max(btn->second.getHeight(), cellHeight);

			cellHeight += OPTION_PADDING + OPTION_PADDING;
			return amountRows * cellHeight + getTitleHeight();
		}

		const olc::Pixel getBackgroundColor() const { return backgroudColor; }
		void setBackgroundColor(olc::Pixel color) { backgroudColor = color; }

		Label& getTitle() { return title; }

		Button& operator[](std::string btn) {
			if (buttons.count(btn) == 0)
				buttons.emplace(btn, Button(btn));
			return buttons[btn];
		}

		void setButtonsGrid(int8_t rows, int8_t columns) {
			amountRows = rows;
			amountColumns = columns;
		}

		void clear() {
			amountRows = 0;
			amountColumns = amountRows;
			title.setText("");
			buttons.clear();
		}

		const bool empty() const { return buttons.empty(); }
	};
}