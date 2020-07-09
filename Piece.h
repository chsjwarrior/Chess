#pragma once

/*
namespace pieces {
	const std::array<NAME, 6> allPiecesName = { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

	COLOR otherColor(const uint8_t color) {
		if (color == COLOR::BLACK)
			return COLOR::WHITE;
		return COLOR::BLACK;
	}
}
*/

struct Piece {
	enum class NAME : uint8_t {
		PAWN = 0,
		KNIGHT,
		BISHOP,
		ROOK,
		QUEEN,
		KING
	};

	enum class COLOR : uint8_t { WHITE = 0, BLACK };

	const NAME name;
	const COLOR color;

	Piece() = delete;
	explicit Piece(const Piece::NAME name, const Piece::COLOR color) : name(name), color(color) {
		//std::cout << "construted: " << *this;
	}
	Piece(const Piece& other) : name(other.name), color(other.color) {
		//std::cout << "copy construted: " << *this;
	}
	~Piece() {
		//std::cout << "destroyed: " << *this;
	}
	Piece& operator=(Piece&) = delete;

	friend std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece);
	friend std::ostream& operator<<(std::ostream& os, const Piece& piece);
};

std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece) {
	if (piece.color == Piece::COLOR::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.name) {
		case Piece::NAME::PAWN:
			os << "Pawn";
			break;
		case Piece::NAME::KNIGHT:
			os << "Knight";
			break;
		case Piece::NAME::BISHOP:
			os << "Bishop";
			break;
		case Piece::NAME::ROOK:
			os << "Rook";
			break;
		case Piece::NAME::QUEEN:
			os << "Queen";
			break;
		case Piece::NAME::KING:
			os << "King";
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const Piece& piece) {
	if (piece.color == Piece::COLOR::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.name) {
		case Piece::NAME::PAWN:
			os << "Pawn";
			break;
		case Piece::NAME::KNIGHT:
			os << "Knight";
			break;
		case Piece::NAME::BISHOP:
			os << "Bishop";
			break;
		case Piece::NAME::ROOK:
			os << "Rook";
			break;
		case Piece::NAME::QUEEN:
			os << "Queen";
			break;
		case Piece::NAME::KING:
			os << "King";
	}
	os << std::endl;
	return os;
}