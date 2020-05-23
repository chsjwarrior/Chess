#ifndef BITMAPS_H_INCLUDED
#define BITMAPS_H_INCLUDED

struct BitBoard {
	const size_t bitMapsLength = 6;
	uint64_t bitMaps[6][2] = { 0 };
	uint64_t flags = 0;
	uint64_t attacks = 0;

	BitBoard() = default;
	BitBoard(const BitBoard& other) {
		for (uint8_t namePiece = 0; namePiece < 6; namePiece++) {
			bitMaps[namePiece][0] = other.bitMaps[namePiece][0];
			bitMaps[namePiece][1] = other.bitMaps[namePiece][1];
		}
		flags = other.flags;
		attacks = 0;
	}

	const uint64_t allPiecesColor(uint8_t colorPiece) const {
		if (colorPiece > 1)
			colorPiece = 1;
		return bitMaps[0][colorPiece] | bitMaps[1][colorPiece] | bitMaps[2][colorPiece] | bitMaps[3][colorPiece] | bitMaps[4][colorPiece] | bitMaps[5][colorPiece];
	}

	const uint64_t allPieces() const {
		return allPiecesColor(0) | allPiecesColor(1);
	}

	friend std::ostream& operator<<(std::ostream& os, const BitBoard& bitBoard);
};

std::ostream& operator<<(std::ostream& os, const BitBoard& bitBoard) {
	uint64_t bitmap = bitBoard.bitMaps[0][0] | bitBoard.bitMaps[0][1] |
		bitBoard.bitMaps[1][0] | bitBoard.bitMaps[1][1] |
		bitBoard.bitMaps[2][0] | bitBoard.bitMaps[2][1] |
		bitBoard.bitMaps[3][0] | bitBoard.bitMaps[3][1] |
		bitBoard.bitMaps[4][0] | bitBoard.bitMaps[4][1] |
		bitBoard.bitMaps[5][0] | bitBoard.bitMaps[5][1];
	os << "bitBoard:\n";
	for (int i = 56; i >= 0; i = i - 8) {
		for (int j = 0; j < 8; j++)
			if ((bitmap >> (i + j) & 1) != 0)
				os << '1';
			else
				os << '0';
		os << "\n";
	}
	os << "Flags:\n";
	bitmap = bitBoard.flags;
	for (int i = 56; i >= 0; i = i - 8) {
		for (int j = 0; j < 8; j++)
			if ((bitmap >> (i + j) & 1) != 0)
				os << '1';
			else
				os << '0';
		os << "\n";
	}
	os << "Attacks:\n";
	bitmap = bitBoard.attacks;
	for (int i = 56; i >= 0; i = i - 8) {
		for (int j = 0; j < 8; j++)
			if ((bitmap >> (i + j) & 1) != 0)
				os << '1';
			else
				os << '0';
		os << "\n";
	}
	return os;
}
#endif //BITMAPS_H_INCLUDED