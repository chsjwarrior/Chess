#pragma once
#include "Types.h"

namespace bitBoardOperations {
	inline const Piece makePiece(PieceType pieceType, Color color) {
		if (pieceType >= 6 || color >= 2)
			return NONE_PIECE;
		return Piece((color << 4) | pieceType);
	}

	inline const PieceType getPieceTypeOfPiece(Piece piece) {
		return PieceType(piece & 0x0F);
	}

	inline const Color getColorOfPiece(Piece piece) {
		return Color(piece >> 4);
	}

	inline const bool hasIntersection(Bitmap bitmap1, Bitmap bitmap2) {
		return (bitmap1 & bitmap2) != 0;
	}

	inline const Bitmap getUnion(Bitmap bitmap1, Bitmap bitmap2) {
		return bitmap1 | bitmap2;
	}

	inline const Bitmap getIntersections(Bitmap bitmap1, Bitmap bitmap2) {
		return bitmap1 & bitmap2;
	}

	inline const Bitmap unsetIntersections(Bitmap bitmap1, Bitmap bitmap2) {
		return bitmap1 & ~bitmap2;
	}

	inline const Bitmap remainder(Bitmap bitmap) {
		return bitmap & (bitmap - 1);
	}

	inline const File getFileOfSquare(Square square) {
		return File(square & 7);
		// file = square % 8; ou file = square & 7;
	}

	inline const Rank getRankOfSquare(Square square) {
		return Rank(square >> 3);
		// rank = square / 8; ou rank = square >> 3;
	}

	/* retornar um bitmap com um bit aceso */
	inline const Bitmap getBitmapOfSquare(Square square) {
		return SQUARE_MASK << square;
	}

	inline const Square getSquareOfFileRank(File file, Rank rank) {
		return Square((rank << 3) + file);
		// 8 * rank + file;
	}

	/* retorna um int da posição a partir do primeiro bit aceso do bitmap */
	const Square getSquareOfBitmap(Bitmap bitmap) {
		if (bitmap == 0)
			return NONE_SQUARE;
		uInt x, y = (uInt)bitmap, n = 63;
		if (y != 0) { n = n - 32; x = y; }
		else x = (uInt)(bitmap >> 32);
		y = x << 16; if (y != 0) { n = n - 16; x = y; }
		y = x << 8; if (y != 0) { n = n - 8; x = y; }
		y = x << 4; if (y != 0) { n = n - 4; x = y; }
		y = x << 2; if (y != 0) { n = n - 2; x = y; }
		return static_cast<Square>(n - ((x << 1) >> 31));
	}

	const uChar bitCount(Bitmap bitmap) {
		bitmap = ((bitmap >> 1) & 0x5555555555555555UL) + (bitmap & 0x5555555555555555UL);
		bitmap = ((bitmap >> 2) & 0x3333333333333333UL) + (bitmap & 0x3333333333333333UL);
		uInt v = (uInt)((bitmap >> 32) + bitmap);
		v = ((v >> 4) & 0x0F0F0F0F) + (v & 0x0F0F0F0F);
		v = ((v >> 8) & 0x00FF00FF) + (v & 0x00F00FF);
		return ((v >> 16) & 0x0000FFFF) + (v & 0x0000FFFF);
	}

	const Bitmap reverse(Bitmap bitmap) {
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
		Bitmap hashKey = 0;
		uShort moveCounter = 0;
		bitBoard.whiteTime = true;
		bitBoard.checkMate = false;
	}

	/* acende un bit no bitmap da peça */
	void setPieceOnSquare(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) {
		bitBoard.bitMaps[pieceType][color] = getUnion(bitBoard.bitMaps[pieceType][color], getBitmapOfSquare(square));
	}

	/* apaga um bit no bitmap da peça */
	void unsetPieceOnSquare(BitBoard& bitBoard, PieceType pieceType, Color color, Square square) {
		bitBoard.bitMaps[pieceType][color] = unsetIntersections(bitBoard.bitMaps[pieceType][color], getBitmapOfSquare(square));
	}

	/* procura por um bit aceso em todos os bitmaps, se estiver aceso gera uma peça */
	const Piece getPieceFromSquare(const BitBoard& bitBoard, Square square) {
		const Bitmap bitmap = getBitmapOfSquare(square);

		Piece piece = NONE_PIECE;

		for (uChar pieceType = 0; pieceType < 6 && piece == NONE_PIECE; pieceType++)
			if (hasIntersection(bitBoard.bitMaps[pieceType][WHITE], bitmap))
				piece = makePiece(static_cast<PieceType>(pieceType), WHITE);
			else if (hasIntersection(bitBoard.bitMaps[pieceType][BLACK], bitmap))
				piece = makePiece(static_cast<PieceType>(pieceType), BLACK);

		return piece;
	}

	const bool isSquareOccupied(BitBoard& bitBoard, Color color, Square square) {
		const Bitmap bitmap = getBitmapOfSquare(square);

		bool isOccupied = false;
		for (uint8_t namePiece = 0; namePiece < 6 && isOccupied == false; namePiece++)
			isOccupied = hasIntersection(bitBoard.bitMaps[namePiece][color], bitmap);

		return isOccupied;
	}

	const bool isSquareAttacked(BitBoard& bitBoard, Square square) {
		return hasIntersection(bitBoard.attacks, getBitmapOfSquare(square));
	}

	const Bitmap getBitmapAllPiecesByColor(const BitBoard& bitBoard, Color color) {
		return bitBoard.bitMaps[0][color] | bitBoard.bitMaps[1][color] | bitBoard.bitMaps[2][color] |
			bitBoard.bitMaps[3][color] | bitBoard.bitMaps[4][color] | bitBoard.bitMaps[5][color];
	}

	const Bitmap getBitmapAllPieces(const BitBoard& bitBoard) {
		return getBitmapAllPiecesByColor(bitBoard, WHITE) | getBitmapAllPiecesByColor(bitBoard, BLACK);
	}

	const Square getMoveFrom(BitBoard& bitBoard) {
		return Square(bitBoard.move.move & 0x0FF);
	}

	const Square getMoveTo(BitBoard& bitBoard) {
		return Square((bitBoard.move.move >> 8) & 0xFFU);
	}

	const Piece getCaptured(BitBoard& bitBoard) {
		return Piece((bitBoard.move.move >> 16) & 0xFFU);
	}

	const bool isEnPassantCapture(BitBoard& bitBoard) {
		return hasIntersection(bitBoard.move.move, 0x1000000U);
	}

	const bool isPawnPromotion(BitBoard& bitBoard) {
		return hasIntersection(bitBoard.move.move, 0x2000000U);
	}

	const bool& isSmallRook(BitBoard& bitBoard, Color color) {
		if (color == WHITE)
			return hasIntersection(bitBoard.move.move >> 28, 0x1U);

		return hasIntersection(bitBoard.move.move >> 28, 0x4U);
	}

	const bool& isBigRook(BitBoard& bitBoard, Color color) {
		if (color == WHITE)
			return hasIntersection(bitBoard.move.move >> 28, 0x2U);

		return hasIntersection(bitBoard.move.move >> 28, 0x8U);
	}

	const void parseFEN(BitBoard& bitBoard, const char* fen) {
		File file = FILE_A;
		Rank rank = RANK_8;
		uShort count = 0;
		Piece piece;

		while ((rank >= RANK_1) && *fen) {
			count = 1;
			switch (*fen) {
				case 'p': piece = BLACK_PAWN; break;
				case 'r': piece = BLACK_ROOK; break;
				case 'n': piece = BLACK_KNIGHT; break;
				case 'b': piece = BLACK_BISHOP; break;
				case 'k': piece = BLACK_KING; break;
				case 'q': piece = BLACK_QUEEN; break;
				case 'P': piece = WHITE_PAWN; break;
				case 'R': piece = WHITE_ROOK; break;
				case 'N': piece = WHITE_KNIGHT; break;
				case 'B': piece = WHITE_BISHOP; break;
				case 'K': piece = WHITE_KING; break;
				case 'Q': piece = WHITE_QUEEN; break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					piece = NONE_PIECE;
					count = *fen - '0';
					break;
				case '/':
				case ' ':
					rank = static_cast<Rank>(static_cast<uChar>(rank) - 1);
					file = FILE_A;
					fen++;
					continue;
				default:
					std::cout << "FEN Error!" << std::endl;
					return;
			}

			for (uChar i = 0; i < count; i++) {
				if (piece != NONE_PIECE)
					setPieceOnSquare(bitBoard, getPieceTypeOfPiece(piece), getColorOfPiece(piece), getSquareOfFileRank(file, rank));
				file = static_cast<File>(static_cast<uChar>(file) + 1);
			}
			fen++;
		}

		bitBoard.whiteTime = *fen == 'w';
		fen += 2;

		bitBoard.flags = INITIAL_POSITION[KING][0] | INITIAL_POSITION[KING][1] |
			INITIAL_POSITION[ROOK][0] | INITIAL_POSITION[ROOK][1];

		for (uShort i = 0; i < 4; i++) {
			if (*fen == ' ') break;
			switch (*fen) {
				case 'K':
				case 'Q': bitBoard.flags = unsetIntersections(bitBoard.flags, RANKS[RANK_1]); break;
				case 'k':
				case 'q': bitBoard.flags = unsetIntersections(bitBoard.flags, RANKS[RANK_8]);
			}
			fen++;
		}
	}

	const bool isKingCheck(const BitBoard& bitBoard, Color color) {
		return bitBoardOperations::hasIntersection(bitBoard.attacks, bitBoard.bitMaps[KING][color]);
	}

	void printBitmap(const char title[], Bitmap bitmap) {
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