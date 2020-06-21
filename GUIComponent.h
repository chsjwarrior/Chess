#pragma once
#include"olcPixelGameEngine.h"

namespace gui {

	constexpr uint8_t PIXEL = 8;

	class Component;

	struct MouseListener {
		virtual void onMousePressed(olc::PixelGameEngine& olc, Component& source) = 0;
		virtual void onMouseReleased(olc::PixelGameEngine& olc, Component& source) = 0;
	};

	class Component {
	protected:

		MouseListener* mouseListener;

	public:
		Component(Component&) = delete;
		Component& operator=(Component&) = delete;
		Component() {
			mouseListener = nullptr;
		}
		virtual ~Component() {}

		virtual void onUserUpdate(olc::PixelGameEngine& olc, const float& fElapsedTime, const olc::vi2d& screenOffset) = 0;

		virtual const int32_t getWidth() const = 0;

		virtual const int32_t getHeight() const = 0;

		void addMouseListener(MouseListener* mouseListener) {
			this->mouseListener = mouseListener;
		}
	};

	class Label : public Component {
	private:
		uint8_t scale;
		olc::Pixel textColor;
		std::string text;

	public:
		Label() = delete;
		explicit Label(const std::string& text) : Component(), text(text) {
			scale = 1;
		}
		~Label() {}

		void onUserUpdate(olc::PixelGameEngine& olc, const float& fElapsedTime, const olc::vi2d& screenOffset) override {
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
		olc::Pixel backgroudColor;
		olc::Pixel highLightColor;

	public:
		Button() = delete;
		explicit Button(std::string text) : Label(text) {
			backgroudColor = olc::GREY;
			highLightColor = olc::WHITE;
		}
		~Button() {}

		void onUserUpdate(olc::PixelGameEngine& olc, const float& fElapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(screenOffset.x + getWidth(), screenOffset.y + getHeight());
			olc::vi2d point(olc.GetMouseX(), olc.GetMouseY());

			olc::Pixel color = backgroudColor;

			//mouseListener
			if ((point.x >= screenOffset.x && point.y >= screenOffset.y) &&
				(point.x <= temp.x && point.y <= temp.y)) {
				color = highLightColor;

				if (mouseListener != nullptr)
					if (olc.GetMouse(0).bPressed)
						mouseListener->onMousePressed(olc, *this);
			}
			//-------------------------

			temp.x = getWidth();
			temp.y = getHeight();

			olc.FillRect(screenOffset, temp, color);

			temp.x = screenOffset.x + PADDING;
			temp.y = screenOffset.y + PADDING;

			Label::onUserUpdate(olc, fElapsedTime, temp);
		}

		const int32_t getWidth() const override {
			return Label::getWidth() + PADDING + PADDING;
		}

		const int32_t getHeight() const override {
			return Label::getHeight() + PADDING + PADDING;
		}

		void setBackgroundColor(olc::Pixel& color) {
			backgroudColor = color;
		}

		void setHighLightColor(olc::Pixel& color) {
			highLightColor = color;
		}
	};

	class Table : public Component {
	private:
		int16_t amountRows = 1, amountColumns = 1, cellWidth = 1, cellHeight = 1;
		int32_t width = 5, height = 5;
		olc::Pixel backgroudColor;
		Label title;
		std::list<Button> buttons;

	public:
		Table() : Component(), title("Table") {
			title.setTextColor(olc::WHITE);
		}
		explicit Table(const std::string& title) : Component(), title(title) {
			this->title.setTextColor(olc::WHITE);
		}
		~Table() {
			buttons.clear();
		}

		void onUserCreate(olc::PixelGameEngine& olc) {
			title.setScale(2);

			// Longest child name determines cell width

			cellWidth = title.getWidth();
			cellHeight = title.getHeight();

			for (auto btn = buttons.begin(); btn != buttons.end(); ++btn) {
				cellWidth = max(btn->getWidth(), cellWidth);
				cellHeight = max(btn->getHeight(), cellHeight);
			}

			width = amountColumns * cellWidth;
			height = (amountRows + 1) * cellHeight;
		}

		void onUserUpdate(olc::PixelGameEngine& olc, const float& fElapsedTime, const olc::vi2d& screenOffset) override {
			olc::vi2d temp(width, height);

			olc.FillRect(screenOffset, temp, backgroudColor);

			//padding			
			temp.x = screenOffset.x + 1;
			temp.y = screenOffset.y + 1;
			title.onUserUpdate(olc, fElapsedTime, temp);

			auto btn = buttons.begin();
			for (uint8_t x = 0; x < amountColumns; x++) {
				for (uint8_t y = 1; y < amountRows + 1; y++) {

					temp.x = screenOffset.x + (x * cellWidth);
					temp.y = screenOffset.y + (y * cellHeight);
					if (btn != buttons.end()) {
						btn->onUserUpdate(olc, fElapsedTime, temp);
						btn++;
					}

				}
			}
		}

		const int32_t getWidth() const override {
			return width;
		}

		const int32_t getHeight() const override {
			return height;
		}

		Label& getTitle() {
			return title;
		}	

		void addButton(const std::string& label) {
			buttons.emplace_back(label);
			buttons.back().setScale(1);
		}

		void setBackgroundColor(olc::Pixel& color) {
			backgroudColor = color;
		}

		void setAmountVisibleCells(const int8_t& rows, const int8_t& columns) {
			amountRows = rows;
			amountColumns = columns;
		}
	};
}