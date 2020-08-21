#pragma once
class MoveMaker {
private:
	const Tbitmap EN_PASSANT_RANK[2] = {RANK_5, RANK_4};
	const Tbitmap PANW_PROMOTION_RANK[2] = {RANK_1, RANK_8};

	const uChar origin;
	const uChar destiny;
	uChar capture;
	bool enPassantCapture, pawnPromotion, smallRook, bigRook;

	void setCapture(const BitBoard& bitBoard, const uChar& destiny) {
		capture = bitBoardOperations::getPieceFromSquare(bitBoard, destiny);
	}

	//Este método verifica se o peão vai realizar uma captura em en passant
	void setEnPassantCapture(BitBoard& bitBoard, const uChar& piece, const uChar& color, const Tbitmap origin, const Tbitmap destiny) {
		if (piece != PAWN)
			return;

		const Tbitmap enPassant = bitBoardOperations::getIntersections(bitBoard.flags, EN_PASSANT_RANK[bitBoardOperations::getOtherColor(color)]);
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
				bitBoardOperations::unsetPieceOnSquare(bitBoard, piece, bitBoardOperations::getOtherColor(color),
													   bitBoardOperations::getSquareFromBitmap(enPassant));
		}
	}

	/*
	 * Este método verifica se o peão move duas casas para frente onde ele se coloca
	 * na posição de enPassant.
	 */
	void setEnPassantMove(BitBoard& bitBoard, const uChar& piece, const uChar& color, const Tbitmap origin, const Tbitmap destiny) {
		bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, EN_PASSANT_RANK[bitBoardOperations::getOtherColor(color)]);
		if (piece == PAWN)
			if (bitBoardOperations::hasIntersection(origin, INITIAL_POSITION[piece][color]))
				bitBoard.flags = bitBoardOperations::getUnion(bitBoard.flags, bitBoardOperations::getIntersections(destiny, EN_PASSANT_RANK[color]));
	}

	// Este método verifca se ocorreu uma promoção do peão
	void setPawnPromotion(const BitBoard& bitBoard, const uChar& piece, const uChar& color, const Tbitmap destiny) {
		if (piece == PAWN)
			pawnPromotion = bitBoardOperations::hasIntersection(destiny, PANW_PROMOTION_RANK[color]);
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setSmallRook(BitBoard& bitBoard, const uChar& piece, const uChar& color, const Tbitmap origin, const Tbitmap destiny) {
		if (piece == KING) {
			const Tbitmap kingBitmap = INITIAL_POSITION[piece][color];
			smallRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap << 2, destiny);

			if (smallRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 3);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, ROOK, color, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, ROOK, color, square);
			}
		}
	}

	//Este método verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setBigRook(BitBoard& bitBoard, const uChar& piece, const uChar& color, const Tbitmap origin, const Tbitmap destiny) {
		if (piece == KING) {
			const uint64_t kingBitmap = INITIAL_POSITION[piece][color];
			bigRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap >> 2, destiny);

			if (bigRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 4);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, ROOK, color, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, ROOK, color, square);
			}
		}
	}

public:
	explicit MoveMaker(const uint8_t& origin, const uint8_t& destiny) :
		origin(origin),
		destiny(destiny),
		capture(NONE_PIECE),
		enPassantCapture(false),
		pawnPromotion(enPassantCapture),
		smallRook(pawnPromotion),
		bigRook(smallRook) {}
	~MoveMaker() {}

	void makeMove(BitBoard& bitBoard) {
		const uint64_t originBitmap = bitBoardOperations::getBitmapFromSquare(origin);
		const uint64_t destinyBitmap = bitBoardOperations::getBitmapFromSquare(destiny);
		uChar piece = bitBoardOperations::getPieceFromSquare(bitBoard, origin);
		uChar color = bitBoardOperations::getPieceColor(piece);
		piece = bitBoardOperations::getPieceName(piece);

		setCapture(bitBoard, destiny);
		setEnPassantCapture(bitBoard, piece, color, originBitmap, destinyBitmap);
		setEnPassantMove(bitBoard, piece, color, originBitmap, destinyBitmap);
		setPawnPromotion(bitBoard, piece, color, destinyBitmap);

		bitBoardOperations::unsetPieceOnSquare(bitBoard, piece, color, origin);
		if (capture != NONE_PIECE)
			bitBoardOperations::unsetPieceOnSquare(bitBoard, bitBoardOperations::getPieceName(capture), bitBoardOperations::getPieceColor(capture), destiny);
		bitBoardOperations::setPieceOnSquare(bitBoard, piece, color, destiny);
		//bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, originBitmap);

		setSmallRook(bitBoard, piece, color, originBitmap, destinyBitmap);
		setBigRook(bitBoard, piece, color, originBitmap, destinyBitmap);
	}

	const uChar& getOrigin() const {
		return origin;
	}

	const uChar& getDestiny() const {
		return destiny;
	}

	const uChar& getCapture() const {
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