#pragma once
#include "BitBoard.h"
#include "Piece.h"
#include <optional>

namespace bitBoardOperations {
	constexpr uint64_t FILE_A = 0x0101010101010101UL;
	constexpr uint64_t FILE_B = 0x0202020202020202UL;
	constexpr uint64_t FILE_C = 0x0404040404040404UL;
	constexpr uint64_t FILE_D = 0x0808080808080808UL;
	constexpr uint64_t FILE_E = 0x1010101010101010UL;
	constexpr uint64_t FILE_F = 0x2020202020202020UL;
	constexpr uint64_t FILE_G = 0x4040404040404040UL;
	constexpr uint64_t FILE_H = 0x8080808080808080UL;

	constexpr uint64_t RANK_1 = 0x00000000000000FFUL;
	constexpr uint64_t RANK_2 = 0x000000000000FF00UL;
	constexpr uint64_t RANK_3 = 0x0000000000FF0000UL;
	constexpr uint64_t RANK_4 = 0x00000000FF000000UL;
	constexpr uint64_t RANK_5 = 0x000000FF00000000UL;
	constexpr uint64_t RANK_6 = 0x0000FF0000000000UL;
	constexpr uint64_t RANK_7 = 0x00FF000000000000UL;
	constexpr uint64_t RANK_8 = 0xFF00000000000000UL;

	constexpr uint64_t FILES[8] = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
	constexpr uint64_t RANKS[8] = {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};

	// de baixo a esquerda para cima a direita
	constexpr uint64_t DIAGONAL_R[15] = {0x0100000000000000UL, 0x0201000000000000UL, 0x0402010000000000UL,
			0x0804020100000000UL, 0x1008040201000000UL, 0x2010080402010000UL, 0x4020100804020100UL, 0x8040201008040201UL,
			0x0080402010080402UL, 0x0000804020100804UL, 0x0000008040201008UL, 0x0000000080402010UL, 0x0000000000804020UL,
			0x0000000000008040UL, 0x0000000000000080UL};
	// de baixo a direita para cima a esquerda
	constexpr uint64_t DIAGONAL_L[15] = {0x0000000000000001UL, 0x0000000000000102UL, 0x0000000000010204UL,
			0x0000000001020408UL, 0x0000000102040810UL, 0x0000010204081020UL, 0x0001020408102040UL, 0x0102040810204080UL,
			0x0204081020408000UL, 0x0408102040800000UL, 0x0810204080000000UL, 0x1020408000000000UL, 0x2040800000000000UL,
			0x4080000000000000UL, 0x8000000000000000UL};
	/* posições iniciais das peças */
	constexpr uint64_t INITIAL_POSITION[6][2] = {{ RANK_2, RANK_7 }, { 0x0000000000000042UL, 0x4200000000000000UL  }, { 0x0000000000000024UL, 0x2400000000000000UL  },
		 { 0x0000000000000081UL, 0x8100000000000000UL }, { 0x0000000000000008UL, 0x0800000000000000UL }, { 0x0000000000000010UL, 0x1000000000000000UL }};

	/* seta os bitmaps das peças na posição inicial */
	void setInitialPosition(BitBoard& bitBoard) {
		for (uint8_t namePiece = 0; namePiece < 6; namePiece++) {
			bitBoard.bitMaps[namePiece][Piece::COLOR::WHITE] = INITIAL_POSITION[namePiece][Piece::COLOR::WHITE];
			bitBoard.bitMaps[namePiece][Piece::COLOR::BLACK] = INITIAL_POSITION[namePiece][Piece::COLOR::BLACK];
		}
		bitBoard.flags = bitBoard.bitMaps[Piece::NAME::KING][Piece::COLOR::WHITE] | bitBoard.bitMaps[Piece::NAME::KING][Piece::COLOR::BLACK] | bitBoard.bitMaps[Piece::NAME::ROOK][Piece::COLOR::WHITE]
			| bitBoard.bitMaps[Piece::NAME::ROOK][Piece::COLOR::BLACK];
		bitBoard.attacks = 0;
	}

	inline const bool hasIntersection(const uint64_t bitmap1, const uint64_t bitmap2) {
		return (bitmap1 & bitmap2) != 0;
	}

	inline const uint64_t getUnion(const uint64_t bitmap1, const uint64_t bitmap2) {
		return bitmap1 | bitmap2;
	}

	inline const uint64_t getIntersections(const uint64_t bitmap1, const uint64_t bitmap2) {
		return bitmap1 & bitmap2;
	}

	inline const uint64_t unsetIntersections(const uint64_t bitmap1, const uint64_t bitmap2) {
		return bitmap1 & ~bitmap2;
	}

	/* retornar um bitmap com um bit aceso */
	const uint64_t getBitmapFromSquare(const uint8_t& square) {
		uint64_t bitmap = 1;
		return bitmap << square;
	}

	/* acende un bit no bitmap da peça */
	void setPieceOnSquare(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorPiece, const uint8_t& square) {
		bitBoard.bitMaps[namePiece][colorPiece] = getUnion(bitBoard.bitMaps[namePiece][colorPiece], getBitmapFromSquare(square));
	}

	/* apaga um bit no bitmap da peça */
	void unsetPieceOnSquare(BitBoard& bitBoard, const uint8_t& namePiece, const uint8_t& colorpiece, const uint8_t& square) {
		bitBoard.bitMaps[namePiece][colorpiece] = unsetIntersections(bitBoard.bitMaps[namePiece][colorpiece], getBitmapFromSquare(square));
	}

	/* procura por um bit aceso em todos os bitmaps, se estiver aceso gera uma peça */
	std::optional<Piece> getPieceFromSquare(const BitBoard& bitBoard, const uint8_t& square) {
		uint64_t bitmap = getBitmapFromSquare(square);

		for (uint8_t namePiece = 0; namePiece < 6; namePiece++)
			if (hasIntersection(bitBoard.bitMaps[namePiece][Piece::BLACK], bitmap))
				return std::make_optional<Piece>((Piece::NAME)namePiece, Piece::BLACK);
			else if (hasIntersection(bitBoard.bitMaps[namePiece][Piece::WHITE], bitmap))
				return std::make_optional<Piece>((Piece::NAME)namePiece, Piece::WHITE);

		return std::nullopt;
	}

	/* retorna um int da posição a partir do primeiro bit aceso do bitmap */
	const uint8_t getSquareFromBitmap(const uint64_t& bitmap) {
		uint32_t x, y;
		if (bitmap == 0) return 64;
		uint32_t n = 63;
		y = (uint32_t) bitmap; if (y != 0) { n = n - 32; x = y; } else x = (uint32_t) (bitmap >> 32);
		y = x << 16; if (y != 0) { n = n - 16; x = y; }
		y = x << 8; if (y != 0) { n = n - 8; x = y; }
		y = x << 4; if (y != 0) { n = n - 4; x = y; }
		y = x << 2; if (y != 0) { n = n - 2; x = y; }
		return n - ((x << 1) >> 31);
	}

	const bool isSquareOccupied(const BitBoard& bitBoard, const Piece::COLOR& color, const uint8_t& square) {
		uint64_t bitmap = getBitmapFromSquare(square);

		for (uint8_t namePiece = 0; namePiece < 6; namePiece++)
			if (hasIntersection(bitBoard.bitMaps[namePiece][color], bitmap))
				return true;

		return false;
	}

	const bool isSquareAttacked(const BitBoard& bitBoard, const uint8_t& square) {
		return hasIntersection(bitBoard.attacks, getBitmapFromSquare(square));
	}

	const uint8_t bitCount(uint64_t bitmap) {
		bitmap = ((bitmap >> 1) & 0x5555555555555555UL) + (bitmap & 0x5555555555555555UL);
		bitmap = ((bitmap >> 2) & 0x3333333333333333UL) + (bitmap & 0x3333333333333333UL);
		uint32_t v = (uint32_t) ((bitmap >> 32) + bitmap);
		v = ((v >> 4) & 0x0f0f0f0f) + (v & 0x0f0f0f0f);
		v = ((v >> 8) & 0x00ff00ff) + (v & 0x00ff00ff);
		return ((v >> 16) & 0x0000ffff) + (v & 0x0000ffff);
	}

	const uint64_t reverse(uint64_t bitmap) {
		bitmap = (bitmap & 0x5555555555555555L) << 1 | (bitmap >> 1) & 0x5555555555555555L;
		bitmap = (bitmap & 0x3333333333333333L) << 2 | (bitmap >> 2) & 0x3333333333333333L;
		bitmap = (bitmap & 0x0f0f0f0f0f0f0f0fL) << 4 | (bitmap >> 4) & 0x0f0f0f0f0f0f0f0fL;
		bitmap = (bitmap & 0x00ff00ff00ff00ffL) << 8 | (bitmap >> 8) & 0x00ff00ff00ff00ffL;
		bitmap = (bitmap << 48) | ((bitmap & 0xffff0000L) << 16) |
			((bitmap >> 16) & 0xffff0000L) | (bitmap >> 48);
		return bitmap;
	}

	const uint64_t remainder(const uint64_t bitmap) {
		return bitmap & (bitmap - 1);
	}

	const bool isKingCheck(const BitBoard& bitBoard, const uint8_t& colorKing) {
		return bitBoardOperations::hasIntersection(bitBoard.attacks, bitBoard.bitMaps[Piece::NAME::KING][colorKing]);
	}
};