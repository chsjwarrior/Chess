#pragma once

typedef unsigned long long Tbitmap;
typedef unsigned int uInt;
typedef unsigned short uShort;
typedef unsigned char uChar;

constexpr Tbitmap FILE_A = 0x0101010101010101UL;
constexpr Tbitmap FILE_B = 0x0202020202020202UL;
constexpr Tbitmap FILE_C = 0x0404040404040404UL;
constexpr Tbitmap FILE_D = 0x0808080808080808UL;
constexpr Tbitmap FILE_E = 0x1010101010101010UL;
constexpr Tbitmap FILE_F = 0x2020202020202020UL;
constexpr Tbitmap FILE_G = 0x4040404040404040UL;
constexpr Tbitmap FILE_H = 0x8080808080808080UL;

constexpr Tbitmap RANK_1 = 0x00000000000000FFUL;
constexpr Tbitmap RANK_2 = 0x000000000000FF00UL;
constexpr Tbitmap RANK_3 = 0x0000000000FF0000UL;
constexpr Tbitmap RANK_4 = 0x00000000FF000000UL;
constexpr Tbitmap RANK_5 = 0x000000FF00000000UL;
constexpr Tbitmap RANK_6 = 0x0000FF0000000000UL;
constexpr Tbitmap RANK_7 = 0x00FF000000000000UL;
constexpr Tbitmap RANK_8 = 0xFF00000000000000UL;
//files
constexpr Tbitmap FILES[8] = {FILE_A, FILE_B , FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
//ranks
constexpr Tbitmap RANKS[8] = {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
// de baixo a esquerda para cima a direita
constexpr Tbitmap DIAGONAL_R[15] = {0x0100000000000000UL, 0x0201000000000000UL, 0x0402010000000000UL,
		0x0804020100000000UL, 0x1008040201000000UL, 0x2010080402010000UL, 0x4020100804020100UL, 0x8040201008040201UL,
		0x0080402010080402UL, 0x0000804020100804UL, 0x0000008040201008UL, 0x0000000080402010UL, 0x0000000000804020UL,
		0x0000000000008040UL, 0x0000000000000080UL};
// de baixo a direita para cima a esquerda
constexpr Tbitmap DIAGONAL_L[15] = {0x0000000000000001UL, 0x0000000000000102UL, 0x0000000000010204UL,
		0x0000000001020408UL, 0x0000000102040810UL, 0x0000010204081020UL, 0x0001020408102040UL, 0x0102040810204080UL,
		0x0204081020408000UL, 0x0408102040800000UL, 0x0810204080000000UL, 0x1020408000000000UL, 0x2040800000000000UL,
		0x4080000000000000UL, 0x8000000000000000UL};
/* posi��es iniciais das pe�as */
constexpr Tbitmap INITIAL_POSITION[6][2] = {{ RANK_2, RANK_7 }, { 0x0000000000000042UL, 0x4200000000000000UL  }, { 0x0000000000000024UL, 0x2400000000000000UL  },
	 { 0x0000000000000081UL, 0x8100000000000000UL }, { 0x0000000000000008UL, 0x0800000000000000UL }, { 0x0000000000000010UL, 0x1000000000000000UL }};

constexpr Tbitmap SQUARE_MASK = 0x1UL;

enum : uChar {
	A1, A2, A3, A4, A5, A6, A7, A8,
	B1, B2, B3, B4, B5, B6, B7, B8,
	C1, C2, C3, C4, C5, C6, C7, C8,
	D1, D2, D3, D4, D5, D6, D7, D8,
	E1, E2, E3, E4, E5, E6, E7, E8,
	F1, F2, F3, F4, F5, F6, F7, F8,
	G1, G2, G3, G4, G5, G6, G7, G8,
	H1, H2, H3, H4, H5, H6, H7, H8, NONE_SQUARE
};

enum : uChar { WHITE = 0, BLACK };

enum : uChar {
	PAWN = 0,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NONE_PIECE = 0xFF
};

struct BitBoard {
	Tbitmap bitMaps[6][2] = {0};
	Tbitmap flags = 0;
	Tbitmap attacks = 0;
	uInt move = 0;
	/* move uint32_t
	00000000 00000000 00000000 11111111 from 0xFF;
	00000000 00000000 11111111 00000000 to 0xFF << 8;
	00000000 11111111 00000000 00000000 capture 0xFF << 16;
	00000001 00000000 00000000 00000000 is en passant 0x1000000;
	00000010 00000000 00000000 00000000 is pawn promotion 0x2000000;
	11110000 00000000 00000000 00000000 castle key 0xF0 << 24;
	*/
	bool whiteTime = false;
	bool checkMate = true;

	BitBoard() = default;
};