#pragma once

struct Piece {
	enum Name : uint8_t {
		PAWN = 0,
		KNIGHT,
		BISHOP,
		ROOK,
		QUEEN,
		KING
	};

	enum Color : uint8_t { WHITE = 0, BLACK };

	const Name NAME;
	const Color COLOR;

	Piece() = delete;
	Piece& operator=(Piece&) = delete;

	explicit Piece(const Piece::Name name, const Piece::Color color) : NAME(name), COLOR(color) {
		std::cout << "Piece(Piece::Name, Piece::Color): " << *this;
	}
	explicit Piece(const Piece& other) : NAME(other.NAME), COLOR(other.COLOR) {
		std::cout << "Piece(Piece&): " << *this;
	}
	~Piece() {
		std::cout << "~Piece(): " << *this;
	}

	const Piece::Color otherColor(Piece::Color color) const {
		if (color == Piece::Color::WHITE)
			return Piece::Color::BLACK;
		return Piece::Color::WHITE;
	}

	const Piece::Color otherColor() const {
		return otherColor(COLOR);
	}

	friend std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece);
	friend std::ostream& operator<<(std::ostream& os, const Piece& piece);
};

std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece) {
	if (piece.COLOR == Piece::Color::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.NAME) {
		case Piece::Name::PAWN:
			os << "Pawn";
			break;
		case Piece::Name::KNIGHT:
			os << "Knight";
			break;
		case Piece::Name::BISHOP:
			os << "Bishop";
			break;
		case Piece::Name::ROOK:
			os << "Rook";
			break;
		case Piece::Name::QUEEN:
			os << "Queen";
			break;
		case Piece::Name::KING:
			os << "King";
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const Piece& piece) {
	if (piece.COLOR == Piece::Color::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.NAME) {
		case Piece::Name::PAWN:
			os << "Pawn";
			break;
		case Piece::Name::KNIGHT:
			os << "Knight";
			break;
		case Piece::Name::BISHOP:
			os << "Bishop";
			break;
		case Piece::Name::ROOK:
			os << "Rook";
			break;
		case Piece::Name::QUEEN:
			os << "Queen";
			break;
		case Piece::Name::KING:
			os << "King";
	}
	os << std::endl;
	return os;
}