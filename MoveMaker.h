#pragma once
class MoveMaker {
private:
	const Bitmap EN_PASSANT_RANK[2] = {RANKS[RANK_4], RANKS[RANK_5]};
	const Bitmap PANW_PROMOTION_RANK[2] = {RANKS[RANK_8], RANKS[RANK_1]};

	const Square origin;
	const Square destiny;

	void setCapture(BitBoard& bitBoard) {
		Piece piece = bitBoardOperations::getPieceFromSquare(bitBoard, destiny);
		if (piece != NONE_PIECE)
			bitBoardOperations::unsetPieceOnSquare(bitBoard, bitBoardOperations::getPieceTypeOfPiece(piece), bitBoardOperations::getColorOfPiece(piece), destiny);
		bitBoard.move = bitBoard.move | (piece << 16);
	}

	//Este método verifica se o peão vai realizar uma captura em en passant
	void setEnPassantCapture(BitBoard& bitBoard, PieceType pieceType, Color color, Bitmap origin, Bitmap destiny) {
		if (pieceType == PAWN) {
			const Bitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, EN_PASSANT_RANK[~color]);

			if (enPassant != 0) {
				bool enPassantCapture = false;
				if (color == WHITE) {
					if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
						enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
					else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
						enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
				}
				else if (color == BLACK) {
					if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
						enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
					else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
						enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
				}

				if (enPassantCapture) {
					bitBoardOperations::unsetPieceOnSquare(bitBoard, pieceType, ~color, bitBoardOperations::getSquareOfBitmap(enPassant));
					bitBoard.move = bitBoardOperations::getUnion(bitBoard.move, 0x100000);
				}
			}
		}
	}

	/*
	 * Este método verifica se o peão move duas casas para frente onde ele se coloca
	 * na posição de enPassant.
	 */
	void setEnPassantMove(BitBoard& bitBoard, PieceType pieceType, Color color, Bitmap origin, Bitmap destiny) {
		bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, EN_PASSANT_RANK[~color]);
		if (pieceType == PAWN)
			if (bitBoardOperations::hasIntersection(origin, INITIAL_POSITION[pieceType][color]))
				bitBoard.flags = bitBoardOperations::getUnion(bitBoard.flags, bitBoardOperations::getIntersections(destiny, EN_PASSANT_RANK[color]));
	}

	// Este método verifca se ocorreu uma promoção do peão
	void setPawnPromotion(BitBoard& bitBoard, PieceType pieceType, Color color, Bitmap destiny) {
		if (pieceType == PAWN)
			if (bitBoardOperations::hasIntersection(destiny, PANW_PROMOTION_RANK[color]))
				bitBoard.move = bitBoard.move | 0x200000;
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setSmallRook(BitBoard& bitBoard, PieceType pieceType, Color color, Bitmap origin, Bitmap destiny) {
		if (pieceType == KING) {
			const Bitmap kingBitmap = INITIAL_POSITION[pieceType][color];

			bool smallRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap << 2, destiny);

			if (smallRook) {
				Square square = bitBoardOperations::getSquareOfBitmap(kingBitmap << 3);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, ROOK, color, square);
				square = bitBoardOperations::getSquareOfBitmap(kingBitmap << 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, ROOK, color, square);
			}
		}
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setBigRook(BitBoard& bitBoard, PieceType pieceType, Color color, Bitmap origin, Bitmap destiny) {
		if (pieceType == KING) {
			const uint64_t kingBitmap = INITIAL_POSITION[pieceType][color];

			bool bigRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap >> 2, destiny);

			if (bigRook) {
				Square square = bitBoardOperations::getSquareOfBitmap(kingBitmap >> 4);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, ROOK, color, square);
				square = bitBoardOperations::getSquareOfBitmap(kingBitmap >> 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, ROOK, color, square);
			}
		}
	}

public:
	explicit MoveMaker(const Square& origin, const Square& destiny) :
		origin(origin),
		destiny(destiny) {}
	~MoveMaker() {}

	void makeMove(BitBoard& bitBoard) {
		const Bitmap originBitmap = bitBoardOperations::getBitmapOfSquare(origin);
		const Bitmap destinyBitmap = bitBoardOperations::getBitmapOfSquare(destiny);
		Piece piece = bitBoardOperations::getPieceFromSquare(bitBoard, origin);
		Color color = bitBoardOperations::getColorOfPiece(piece);
		PieceType pieceType = bitBoardOperations::getPieceTypeOfPiece(piece);

		setCapture(bitBoard);
		setEnPassantCapture(bitBoard, pieceType, color, originBitmap, destinyBitmap);
		setEnPassantMove(bitBoard, pieceType, color, originBitmap, destinyBitmap);
		setPawnPromotion(bitBoard, pieceType, color, destinyBitmap);

		bitBoardOperations::unsetPieceOnSquare(bitBoard, pieceType, color, origin);
		bitBoardOperations::setPieceOnSquare(bitBoard, pieceType, color, destiny);
		//bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, originBitmap);

		setSmallRook(bitBoard, pieceType, color, originBitmap, destinyBitmap);
		setBigRook(bitBoard, pieceType, color, originBitmap, destinyBitmap);
	}

	const Square& getOrigin() const {
		return origin;
	}

	const Square& getDestiny() const {
		return destiny;
	}
};