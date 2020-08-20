#pragma once
struct BitBoard {
	uint64_t bitMaps[6][2] = {0};
	uint64_t flags = 0;
	uint64_t attacks = 0;
	uint32_t move = 0;
	/* move uint32_t
	00000000 00000000 00000000 11111111 from 0xFF;
	00000000 00000000 11111111 00000000 to 0xFF << 8;
	00000000 00001111 00000000 00000000 capture 0x7 << 16, 0x8 << 16;
	00000000 00010000 00000000 00000000 is en passant 0x100000;
	00000000 00100000 00000000 00000000 is pawn promotion 0x200000;
	00001111 00000000 00000000 00000000 castle key 0xF << 24;
	*/

	BitBoard() = default;
	friend std::ostream& operator<<(std::ostream& os, const BitBoard& bitBoard);
};

std::ostream& operator<<(std::ostream& os, const BitBoard& bitBoard) {
	uint64_t bitmap = bitBoard.bitMaps[0][0] | bitBoard.bitMaps[0][1] | bitBoard.bitMaps[1][0] | bitBoard.bitMaps[1][1] | bitBoard.bitMaps[2][0] | bitBoard.bitMaps[2][1] |
		bitBoard.bitMaps[3][0] | bitBoard.bitMaps[3][1] | bitBoard.bitMaps[4][0] | bitBoard.bitMaps[4][1] | bitBoard.bitMaps[5][0] | bitBoard.bitMaps[5][1];

	os << "BitBoard:\n";
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