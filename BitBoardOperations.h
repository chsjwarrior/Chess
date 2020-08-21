#pragma once
#include "Types.h"
#include <optional>

namespace bitBoardOperations {
	inline const uChar createPiece(const uChar pieceName, const uChar colorPiece) {
		if (pieceName >= 6 || colorPiece >= 2)
			return NONE_PIECE;
		return (colorPiece << 4) | pieceName;
	}

	inline const uChar getPieceName(const uChar piece) {
		return piece & 0x0F;
	}

	inline const uChar getPieceColor(const uChar color) {
		return color >> 4;
	}

	inline const uChar getOtherColor(const uChar color) {
		if (color == WHITE)
			return BLACK;
		return WHITE;
	}

	inline const bool hasIntersection(const Tbitmap bitmap1, const Tbitmap bitmap2) {
		return (bitmap1 & bitmap2) != 0;
	}

	inline const Tbitmap getUnion(const Tbitmap bitmap1, const Tbitmap bitmap2) {
		return bitmap1 | bitmap2;
	}

	inline const Tbitmap getIntersections(const Tbitmap bitmap1, const Tbitmap bitmap2) {
		return bitmap1 & bitmap2;
	}

	inline const Tbitmap unsetIntersections(const Tbitmap bitmap1, const Tbitmap bitmap2) {
		return bitmap1 & ~bitmap2;
	}

	inline const Tbitmap remainder(const Tbitmap bitmap) {
		return bitmap & (bitmap - 1);
	}

	inline const uChar getFileFromSquare(const uChar square) {
		return square & 7;
		// file = square % 8; ou file = square & 7;
	}

	inline const uChar getRankFromSquare(const uChar square) {
		return square >> 3;
		// rank = square / 8; ou rank = square >> 3;
	}

	/* retornar um bitmap com um bit aceso */
	inline const Tbitmap getBitmapFromSquare(const uChar square) {
		return SQUARE_MASK << square;
	}

	inline const uChar getSquareFromFileRank(const uint8_t file, const uChar rank) {
		return 8 * rank + file;
	}

	/* retorna um int da posição a partir do primeiro bit aceso do bitmap */
	const uChar getSquareFromBitmap(const Tbitmap bitmap) {
		if (bitmap == 0)
			return 64;
		uInt x, y = (uInt)bitmap, n = 63;
		if (y != 0) { n = n - 32; x = y; }
		else x = (uInt)(bitmap >> 32);
		y = x << 16; if (y != 0) { n = n - 16; x = y; }
		y = x << 8; if (y != 0) { n = n - 8; x = y; }
		y = x << 4; if (y != 0) { n = n - 4; x = y; }
		y = x << 2; if (y != 0) { n = n - 2; x = y; }
		return n - ((x << 1) >> 31);
	}

	const uChar bitCount(Tbitmap bitmap) {
		bitmap = ((bitmap >> 1) & 0x5555555555555555UL) + (bitmap & 0x5555555555555555UL);
		bitmap = ((bitmap >> 2) & 0x3333333333333333UL) + (bitmap & 0x3333333333333333UL);
		uInt v = (uInt)((bitmap >> 32) + bitmap);
		v = ((v >> 4) & 0x0F0F0F0F) + (v & 0x0F0F0F0F);
		v = ((v >> 8) & 0x00FF00FF) + (v & 0x00F00FF);
		return ((v >> 16) & 0x0000FFFF) + (v & 0x0000FFFF);
	}

	const Tbitmap reverse(Tbitmap bitmap) {
		bitmap = (bitmap & 0x5555555555555555L) << 1 | (bitmap >> 1) & 0x5555555555555555L;
		bitmap = (bitmap & 0x3333333333333333L) << 2 | (bitmap >> 2) & 0x3333333333333333L;
		bitmap = (bitmap & 0x0F0F0F0F0F0F0F0FL) << 4 | (bitmap >> 4) & 0x0F0F0F0F0F0F0F0FL;
		bitmap = (bitmap & 0x00FF00FF00FF00FFL) << 8 | (bitmap >> 8) & 0x00FF00FF00FF00FFL;
		bitmap = (bitmap << 48) | ((bitmap & 0xFFFF0000L) << 16) |
			((bitmap >> 16) & 0xFFFF0000L) | (bitmap >> 48);
		return bitmap;
	}

	/* seta os bitmaps das peças na posição inicial */
	void setInitialPosition(BitBoard& bitBoard) {
		for (uChar namePiece = 0; namePiece < 6; namePiece++) {
			bitBoard.bitMaps[namePiece][0] = INITIAL_POSITION[namePiece][0];
			bitBoard.bitMaps[namePiece][1] = INITIAL_POSITION[namePiece][1];
		}
		bitBoard.flags = bitBoard.bitMaps[KING][0] | bitBoard.bitMaps[KING][1] |
			bitBoard.bitMaps[ROOK][0] | bitBoard.bitMaps[ROOK][1];
		bitBoard.attacks = 0;
		bitBoard.move = 0;
		bitBoard.whiteTime = true;
		bitBoard.checkMate = false;
	}

	/* acende un bit no bitmap da peça */
	void setPieceOnSquare(BitBoard& bitBoard, const uChar namePiece, const uChar colorPiece, const uChar square) {
		bitBoard.bitMaps[namePiece][colorPiece] = getUnion(bitBoard.bitMaps[namePiece][colorPiece], getBitmapFromSquare(square));
	}

	/* apaga um bit no bitmap da peça */
	void unsetPieceOnSquare(BitBoard& bitBoard, const uChar namePiece, const uChar colorpiece, const uChar square) {
		bitBoard.bitMaps[namePiece][colorpiece] = unsetIntersections(bitBoard.bitMaps[namePiece][colorpiece], getBitmapFromSquare(square));
	}

	/* procura por um bit aceso em todos os bitmaps, se estiver aceso gera uma peça */
	const uChar getPieceFromSquare(const BitBoard& bitBoard, const uChar square) {
		const uint64_t bitmap = getBitmapFromSquare(square);

		uChar piece = NONE_PIECE;

		for (uChar namePiece = 0; namePiece < 6 && piece == NONE_PIECE; namePiece++)
			if (hasIntersection(bitBoard.bitMaps[namePiece][0], bitmap))
				piece = createPiece(namePiece, WHITE);
			else if (hasIntersection(bitBoard.bitMaps[namePiece][1], bitmap))
				piece = createPiece(namePiece, BLACK);

		return piece;
	}

	const bool isSquareOccupied(const BitBoard& bitBoard, const uChar colorPiece, const uChar square) {
		const uint64_t bitmap = getBitmapFromSquare(square);

		for (uint8_t namePiece = 0; namePiece < 6; namePiece++)
			if (hasIntersection(bitBoard.bitMaps[namePiece][colorPiece], bitmap))
				return true;
		return false;
	}

	const bool isSquareAttacked(const BitBoard& bitBoard, const uChar square) {
		return hasIntersection(bitBoard.attacks, getBitmapFromSquare(square));
	}

	const Tbitmap allPiecesColor(const BitBoard& bitBoard, uChar colorPiece) {
		return bitBoard.bitMaps[0][colorPiece] | bitBoard.bitMaps[1][colorPiece] | bitBoard.bitMaps[2][colorPiece] |
			bitBoard.bitMaps[3][colorPiece] | bitBoard.bitMaps[4][colorPiece] | bitBoard.bitMaps[5][colorPiece];
	}

	const Tbitmap allPieces(const BitBoard& bitBoard) {
		return allPiecesColor(bitBoard, WHITE) | allPiecesColor(bitBoard, BLACK);
	}

	const uChar getMoveFrom(const BitBoard& bitBoard) {
		return bitBoard.move & 0xFF;
	}

	const uChar getMoveTo(const BitBoard& bitBoard) {
		return (bitBoard.move >> 8) & 0xFF;
	}

	const uChar getCaptured(const BitBoard& bitBoard) {
		uChar name = (bitBoard.move >> 16) & 0x0F;
		uChar color = (bitBoard.move >> 16) & 0xF0;
		return createPiece(name, color);
	}

	const uint8_t isEnPassantCapture(const BitBoard& bitBoard) {
		return hasIntersection(bitBoard.move, 0x100000);
	}

	const bool isPawnPromotion(const BitBoard& bitBoard) {
		return hasIntersection(bitBoard.move, 0x200000);
	}

	const bool& isSmallRook(const BitBoard& bitBoard, const uChar colorPiece) {
		if (colorPiece == WHITE)
			return hasIntersection(bitBoard.move >> 24, 0x1);

		return hasIntersection(bitBoard.move >> 24, 0x4);
	}

	const bool& isBigRook(const BitBoard& bitBoard, const uChar colorPiece) {
		if (colorPiece == WHITE)
			return hasIntersection(bitBoard.move >> 24, 0x2);

		return hasIntersection(bitBoard.move >> 24, 0x8);
	}

	const bool isKingCheck(const BitBoard& bitBoard, const uChar colorKing) {
		return bitBoardOperations::hasIntersection(bitBoard.attacks, bitBoard.bitMaps[KING][colorKing]);
	}

	void printBitmap(const char title[], const Tbitmap bitmap) {
		std::cout << title << std::endl;
		for (int r = 56; r >= 0; r = r - 8) {
			for (int f = 0; f < 8; f++)
				if ((bitmap >> (r + f) & 1) != 0)
					std::cout << '1';
				else
					std::cout << '0';
			std::cout << std::endl;
		}
	}
};