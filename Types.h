#pragma once

typedef unsigned long long Bitmap;
typedef unsigned int uInt;
typedef unsigned short uShort;
typedef unsigned char uChar;
//files
constexpr Bitmap FILES[8] = {0x0101010101010101UL, 0x0202020202020202UL , 0x0404040404040404UL, 0x0808080808080808UL,
								0x1010101010101010UL, 0x2020202020202020UL, 0x4040404040404040UL, 0x8080808080808080UL};
//ranks
constexpr Bitmap RANKS[8] = {0x00000000000000FFUL, 0x000000000000FF00UL, 0x0000000000FF0000UL, 0x00000000FF000000UL,
								0x000000FF00000000UL, 0x0000FF0000000000UL, 0x00FF000000000000UL, 0xFF00000000000000UL};
// de baixo a esquerda para cima a direita
constexpr Bitmap DIAGONAL_R[15] = {0x0100000000000000UL, 0x0201000000000000UL, 0x0402010000000000UL, 0x0804020100000000UL,
									0x1008040201000000UL, 0x2010080402010000UL, 0x4020100804020100UL, 0x8040201008040201UL,
									0x0080402010080402UL, 0x0000804020100804UL, 0x0000008040201008UL, 0x0000000080402010UL,
									0x0000000000804020UL, 0x0000000000008040UL, 0x0000000000000080UL};
// de baixo a direita para cima a esquerda
constexpr Bitmap DIAGONAL_L[15] = {0x0000000000000001UL, 0x0000000000000102UL, 0x0000000000010204UL, 0x0000000001020408UL,
									0x0000000102040810UL, 0x0000010204081020UL, 0x0001020408102040UL, 0x0102040810204080UL,
									0x0204081020408000UL, 0x0408102040800000UL, 0x0810204080000000UL, 0x1020408000000000UL,
									0x2040800000000000UL, 0x4080000000000000UL, 0x8000000000000000UL};
/* posições iniciais das peças */
constexpr Bitmap INITIAL_POSITION[6][2] = {{ RANKS[1], RANKS[6] }, { 0x0000000000000042UL, 0x4200000000000000UL  },
											{ 0x0000000000000024UL, 0x2400000000000000UL  }, { 0x0000000000000081UL, 0x8100000000000000UL },
											{ 0x0000000000000008UL, 0x0800000000000000UL }, { 0x0000000000000010UL, 0x1000000000000000UL }};

constexpr Bitmap PATH_SMALL_ROOK = 0x6000000000000060UL;

constexpr Bitmap PATH_BIG_ROOK = 0x0E0000000000000EUL;

constexpr Bitmap SQUARE_MASK = 0x1UL;

constexpr uInt MOVE_CLEAR = 0x00FFFF4040;

enum File : uChar { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, NONE_FILE };

enum Rank : uChar { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, NONE_RANK };

enum Square : uChar {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	NONE_SQUARE //= 0X40 = 64
};

enum Color : uChar { WHITE, BLACK };

enum PieceType : uChar {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NONE_PIECE_TYPE = 0x0F
};

enum Piece : uChar {
	WHITE_PAWN = (WHITE << 4) | PAWN,
	WHITE_KNIGHT = (WHITE << 4) | KNIGHT,
	WHITE_BISHOP = (WHITE << 4) | BISHOP,
	WHITE_ROOK = (WHITE << 4) | ROOK,
	WHITE_QUEEN = (WHITE << 4) | QUEEN,
	WHITE_KING = (WHITE << 4) | KING,
	BLACK_PAWN = (BLACK << 4) | PAWN,
	BLACK_KNIGHT = (BLACK << 4) | KNIGHT,
	BLACK_BISHOP = (BLACK << 4) | BISHOP,
	BLACK_ROOK = (BLACK << 4) | ROOK,
	BLACK_QUEEN = (BLACK << 4) | QUEEN,
	BLACK_KING = (BLACK << 4) | KING,
	NONE_PIECE = 0xFF
};

struct Move {
	uInt move = 0;
	uInt score = 0;
	/* move uint32_t
	00000000 00000000 00000000 11111111 from 0xFF;
	00000000 00000000 11111111 00000000 to 0xFF << 8;
	00000000 11111111 00000000 00000000 capture 0xFF << 16;
	00000001 00000000 00000000 00000000 is en passant 0x1000000;
	00000010 00000000 00000000 00000000 is pawn promotion 0x2000000;
	11110000 00000000 00000000 00000000 castle key 0xF0 << 28;
	*/
};

struct BitBoard {
	Bitmap bitMaps[6][2] = {0};
	Bitmap flags = 0;
	Bitmap attacks = 0;	
	uShort moveCounter = 0;

	bool whiteTime = false;
	bool checkMate = true;

	Move move;

	BitBoard() = default;
};

constexpr inline File operator~(const File& file) { return File(file ^ File::FILE_H); }

constexpr inline Rank operator~(const Rank& rank) { return Rank(rank ^ Rank::RANK_8); }

constexpr inline Square operator~(const Square& square) { return Square(square ^ Square::A8); }

constexpr inline Color operator~(const Color& color) { return Color(color ^ Color::BLACK); }

constexpr inline Piece operator~(const Piece& piece) { return Piece(piece ^ 0xF0); }