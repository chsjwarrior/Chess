#pragma once
class MoveMaker {
private:
	const Bitmap EN_PASSANT_RANK[2] = {RANKS[RANK_5], RANKS[RANK_4]};
	const Bitmap PANW_PROMOTION_RANK[2] = {RANKS[RANK_1], RANKS[RANK_8]};

	const Square origin;
	const Square destiny;
	Piece capture;
	bool enPassantCapture, pawnPromotion, smallRook, bigRook;

	void setCapture(const BitBoard& bitBoard, const Square& destiny) {
		capture = bitBoardOperations::getPieceFromSquare(bitBoard, destiny);
	}

	//Este método verifica se o peão vai realizar uma captura em en passant
	void setEnPassantCapture(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Bitmap origin, const Bitmap destiny) {
		if (pieceType != PAWN)
			return;

		const Bitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, EN_PASSANT_RANK[~color]);
		if (enPassant != 0) {
			if (color == WHITE) {
				if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
				else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny >> 8, enPassant);
			}
			else {
				if (bitBoardOperations::hasIntersection(origin << 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
				else if (bitBoardOperations::hasIntersection(origin >> 1, enPassant))
					enPassantCapture = bitBoardOperations::hasIntersection(destiny << 8, enPassant);
			}

			if (enPassantCapture)
				bitBoardOperations::unsetPieceOnSquare(bitBoard, pieceType, ~color, bitBoardOperations::getSquareOfBitmap(enPassant));
		}
	}

	/*
	 * Este método verifica se o peão move duas casas para frente onde ele se coloca
	 * na posição de enPassant.
	 */
	void setEnPassantMove(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Bitmap origin, const Bitmap destiny) {
		bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, EN_PASSANT_RANK[~color]);
		if (pieceType == PAWN)
			if (bitBoardOperations::hasIntersection(origin, INITIAL_POSITION[pieceType][color]))
				bitBoard.flags = bitBoardOperations::getUnion(bitBoard.flags, bitBoardOperations::getIntersections(destiny, EN_PASSANT_RANK[color]));
	}

	// Este método verifca se ocorreu uma promoção do peão
	void setPawnPromotion(const BitBoard& bitBoard, const PieceType& piece, const Color& color, const Bitmap destiny) {
		if (piece == PAWN)
			pawnPromotion = bitBoardOperations::hasIntersection(destiny, PANW_PROMOTION_RANK[color]);
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setSmallRook(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Bitmap origin, const Bitmap destiny) {
		if (pieceType == KING) {
			const Bitmap kingBitmap = INITIAL_POSITION[pieceType][color];
			smallRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
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
	void setBigRook(BitBoard& bitBoard, const PieceType& pieceType, const Color& color, const Bitmap origin, const Bitmap destiny) {
		if (pieceType == KING) {
			const uint64_t kingBitmap = INITIAL_POSITION[pieceType][color];
			bigRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
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
		destiny(destiny),
		capture(NONE_PIECE),
		enPassantCapture(false),
		pawnPromotion(enPassantCapture),
		smallRook(pawnPromotion),
		bigRook(smallRook) {}
	~MoveMaker() {}

	void makeMove(BitBoard& bitBoard) {
		const Bitmap originBitmap = bitBoardOperations::getBitmapOfSquare(origin);
		const Bitmap destinyBitmap = bitBoardOperations::getBitmapOfSquare(destiny);
		Piece piece = bitBoardOperations::getPieceFromSquare(bitBoard, origin);
		Color color = bitBoardOperations::getColorOfPiece(piece);
		PieceType pieceType = bitBoardOperations::getPieceTypeOfPiece(piece);

		setCapture(bitBoard, destiny);
		setEnPassantCapture(bitBoard, pieceType, color, originBitmap, destinyBitmap);
		setEnPassantMove(bitBoard, pieceType, color, originBitmap, destinyBitmap);
		setPawnPromotion(bitBoard, pieceType, color, destinyBitmap);

		bitBoardOperations::unsetPieceOnSquare(bitBoard, pieceType, color, origin);
		if (capture != NONE_PIECE)
			bitBoardOperations::unsetPieceOnSquare(bitBoard, bitBoardOperations::getPieceTypeOfPiece(capture), bitBoardOperations::getColorOfPiece(capture), destiny);
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

	const Piece& getCapture() const {
		return capture;
	}

	const bool& isEnPassantCapture() {
		return enPassantCapture;
	}

	const bool& isPawnPromotion() const {
		return pawnPromotion;
	}

	const bool& isSmallRook() const {
		return smallRook;
	}

	const bool& isBigRook() const {
		return bigRook;
	}
};