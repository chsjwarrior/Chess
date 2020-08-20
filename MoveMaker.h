#pragma once
class MoveMaker {
private:
	const uint64_t EN_PASSANT_RANK[2] = {bitBoardOperations::RANK_5, bitBoardOperations::RANK_4};
	const uint64_t PANW_PROMOTION_RANK[2] = {bitBoardOperations::RANK_1, bitBoardOperations::RANK_8};

	const uint8_t origin;
	const uint8_t destiny;
	std::optional<Piece> capture;
	bool enPassantCapture, pawnPromotion, smallRook, bigRook;

	void setCapture(const BitBoard& bitBoard, const uint8_t& destiny) {
		auto piece = bitBoardOperations::getPieceFromSquare(bitBoard, destiny);
		if (piece.has_value())
			capture.emplace(piece->NAME, piece->COLOR);
	}

	//Este m�todo verifica se o pe�o vai realizar uma captura em en passant
	void setEnPassantCapture(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.NAME != Piece::Name::PAWN)
			return;

		const uint64_t enPassant = bitBoardOperations::getIntersections(bitBoard.flags, EN_PASSANT_RANK[piece.otherColor()]);
		if (enPassant != 0) {
			if (piece.COLOR == Piece::Color::WHITE) {
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
				bitBoardOperations::unsetPieceOnSquare(bitBoard, piece.NAME, piece.otherColor(),
													   bitBoardOperations::getSquareFromBitmap(enPassant));
		}
	}

	/*
	 * Este m�todo verifica se o pe�o move duas casas para frente onde ele se coloca
	 * na posi��o de enPassant.
	 */
	void setEnPassantMove(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, EN_PASSANT_RANK[piece.otherColor()]);
		if (piece.NAME == Piece::Name::PAWN)
			if (bitBoardOperations::hasIntersection(origin, bitBoardOperations::INITIAL_POSITION[piece.NAME][piece.COLOR]))
				bitBoard.flags = bitBoardOperations::getUnion(bitBoard.flags, bitBoardOperations::getIntersections(destiny, EN_PASSANT_RANK[piece.COLOR]));
	}

	// Este m�todo verifca se ocorreu uma promo��o do pe�o
	void setPawnPromotion(const BitBoard& bitBoard, const Piece& piece, const uint64_t destiny) {
		if (piece.NAME == Piece::Name::PAWN)
			pawnPromotion = bitBoardOperations::hasIntersection(destiny, PANW_PROMOTION_RANK[piece.COLOR]);
	}

	//Este m�todo verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setSmallRook(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.NAME == Piece::Name::KING) {
			const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[piece.NAME][piece.COLOR];
			smallRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap << 2, destiny);

			if (smallRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 3);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, Piece::Name::ROOK, piece.COLOR, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap << 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, Piece::Name::ROOK, piece.COLOR, square);
			}
		}
	}

	//Este m�todo verifica se o rei vai mover duas casas para um lado e efetua os	movimento do roque
	void setBigRook(BitBoard& bitBoard, const Piece& piece, const uint64_t origin, const uint64_t destiny) {
		if (piece.NAME == Piece::Name::KING) {
			const uint64_t kingBitmap = bitBoardOperations::INITIAL_POSITION[piece.NAME][piece.COLOR];
			bigRook = bitBoardOperations::hasIntersection(kingBitmap, origin)
				&& bitBoardOperations::hasIntersection(kingBitmap >> 2, destiny);

			if (bigRook) {
				uint8_t square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 4);
				bitBoardOperations::unsetPieceOnSquare(bitBoard, Piece::Name::ROOK, piece.COLOR, square);
				square = bitBoardOperations::getSquareFromBitmap(kingBitmap >> 1);
				bitBoardOperations::setPieceOnSquare(bitBoard, Piece::Name::ROOK, piece.COLOR, square);
			}
		}
	}

public:
	explicit MoveMaker(const uint8_t& origin, const uint8_t& destiny) :
		origin(origin),
		destiny(destiny),
		capture(std::nullopt),
		enPassantCapture(false),
		pawnPromotion(enPassantCapture),
		smallRook(pawnPromotion),
		bigRook(smallRook) {}
	~MoveMaker() { capture.reset(); }

	void makeMove(BitBoard& bitBoard) {
		const uint64_t originBitmap = bitBoardOperations::getBitmapFromSquare(origin);
		const uint64_t destinyBitmap = bitBoardOperations::getBitmapFromSquare(destiny);
		std::optional<Piece> piece = bitBoardOperations::getPieceFromSquare(bitBoard, origin);

		setCapture(bitBoard, destiny);
		setEnPassantCapture(bitBoard, *piece, originBitmap, destinyBitmap);
		setEnPassantMove(bitBoard, *piece, originBitmap, destinyBitmap);
		setPawnPromotion(bitBoard, *piece, destinyBitmap);

		bitBoardOperations::unsetPieceOnSquare(bitBoard, piece->NAME, piece->COLOR, origin);
		if (capture.has_value())
			bitBoardOperations::unsetPieceOnSquare(bitBoard, capture->NAME, capture->COLOR, destiny);
		bitBoardOperations::setPieceOnSquare(bitBoard, piece->NAME, piece->COLOR, destiny);
		//bitBoard.flags = bitBoardOperations::unsetIntersections(bitBoard.flags, originBitmap);

		setSmallRook(bitBoard, *piece, originBitmap, destinyBitmap);
		setBigRook(bitBoard, *piece, originBitmap, destinyBitmap);
	}

	const uint8_t& getOrigin() const {
		return origin;
	}

	const uint8_t& getDestiny() const {
		return destiny;
	}

	const Piece& getCapture() const {
		return capture.value();
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