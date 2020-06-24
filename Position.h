#pragma once
#include <sstream>
/*
 Esta classe representa as marcações do tabuleiro de xadrez.
 file = 'A','B','C','D','E','F','G','H'.
 rank = '1','2','3','4','5','6','7','8'.
 */
class Position {
private:
	const uint8_t FILE_MASK = 0xF0;
	const uint8_t RANK_MASK = 0xF;
	uint8_t position;

public:
	Position() : position(0) {}

	explicit Position(const uint8_t file, const uint8_t rank) {
		setFile(file);
		setRank(rank);
	}

	explicit Position(const uint8_t square) {
		setSquare(square);
	}

	const uint8_t getRank() const {
		return RANK_MASK & position;
	}

	void setRank(uint8_t rank) {
		if (rank > 7)
			rank = 7;
		position = FILE_MASK & position | rank;
	}

	const uint8_t getFile() const {
		return position >> 4;
	}

	void setFile(uint8_t file) {
		if (file > 7)
			file = 7;
		position = RANK_MASK & position | (file << 4);
	}

	const uint8_t getSquare() const {
		return getFile() + (getRank() << 3);
		// return 8 * rank + file;
	}

	void setSquare(uint8_t square) {
		if (square > 63)
			square = 63;
		uint8_t file = (square & 7) << 4;
		uint8_t rank = square >> 3;
		position = file | rank;
		// file = square % 8; ou file = square & 7;
		// rank = square / 8; ou rank = square >> 3;
	}

	void invert() {
		setRank(7 - getRank());
	}

	const bool operator==(const Position& other) {
		if (this != &other)
			return position == other.position;
		return true;
	}

	const bool operator<(const Position& other) const {
		return getFile() < other.getFile() || getFile() == other.getFile() && getRank() < other.getRank();
	}

	const std::string toString() {
		return {char(getFile() + 97), char(getRank() + 49), '\0'};
	}

	friend std::ostringstream& operator<<(std::ostringstream& os, const Position& pos);
};

std::ostringstream& operator<<(std::ostringstream& os, const Position& pos) {
	std::string s;
	s = char(pos.getFile() + 97);
	s += char(pos.getRank() + 49);
	os << s;
	return os;
}