#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

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
	enum NAME : uint8_t {
		PAWN = 0,
		KNIGHT = 1,
		BISHOP = 2,
		ROOK = 3,
		QUEEN = 4,
		KING = 5,		
	};

	enum COLOR : uint8_t { WHITE = 0, BLACK = 1 };

	const NAME name;
	const COLOR color;

	Piece() = delete;
	explicit Piece(const Piece::NAME name, const Piece::COLOR color) : name(name), color(color) {
		std::cout << "construted: " << *this;
	}	
	~Piece() {
		std::cout << "destroyed: " << *this;
	}

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

#endif //PIECE_H_INCLUDED