#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

namespace pieces {
	enum NAME : uint8_t {
		PAWN = 0,
		KNIGHT = 1,
		BISHOP = 2,
		ROOK = 3,
		QUEEN = 4,
		KING = 5,
		NUM_PIECES = 6
	};

	enum COLOR : uint8_t { WHITE = 0, BLACK = 1, NUM_COLOR = 2 };

	const std::array<NAME, 6> allPiecesName = { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

	COLOR otherColor(const uint8_t color) {
		if (color == COLOR::BLACK)
			return COLOR::WHITE;
		return COLOR::BLACK;
	}
}

struct Piece {
	const pieces::NAME NAME;
	const pieces::COLOR COLOR;

	Piece() = delete;	
	explicit Piece(const pieces::NAME name, const pieces::COLOR color) : NAME(name), COLOR(color) {
		std::cout << "created: " << *this;
	}
	~Piece() {
		std::cout << "destroyed: " << *this;
	}

	friend std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece);
	friend std::ostream& operator<<(std::ostream& os, const Piece& piece);
};

std::ostringstream& operator<<(std::ostringstream& os, const Piece& piece) {
	if (piece.COLOR == pieces::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.NAME) {
	case pieces::PAWN:
		os << "Pawn";
		break;
	case pieces::KNIGHT:
		os << "Knight";
		break;
	case pieces::BISHOP:
		os << "Bishop";
		break;
	case pieces::ROOK:
		os << "Rook";
		break;
	case pieces::QUEEN:
		os << "Queen";
		break;
	case pieces::KING:
		os << "King";
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const Piece& piece) {
	if (piece.COLOR == pieces::BLACK)
		os << "Black ";
	else
		os << "White ";

	switch (piece.NAME) {
	case pieces::PAWN:
		os << "Pawn";
		break;
	case pieces::KNIGHT:
		os << "Knight";
		break;
	case pieces::BISHOP:
		os << "Bishop";
		break;
	case pieces::ROOK:
		os << "Rook";
		break;
	case pieces::QUEEN:
		os << "Queen";
		break;
	case pieces::KING:
		os << "King";
	}
	os << std::endl;
	return os;
}

#endif //PIECE_H_INCLUDED