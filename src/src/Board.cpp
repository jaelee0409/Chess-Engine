#include "Board.h"

U64 Board::getPawnAttackBitboard(int side, int square)
{
    U64 attackBitboard = 0ULL;
    U64 bitboard = 0ULL;
    setBit(bitboard, square);

    // white turn
    if (!side)
    {
        // if the white pawn is not on A file, up left is possible
        if (bitboard & notAfile)
            attackBitboard |= (bitboard >> 9);
        // if the white pawn is not on H file, up right is possible
        if (bitboard & notHfile)
            attackBitboard |= (bitboard >> 7);
    }
    // black turn
    else
    {
        // if the black pawn is not on A file, down left is possible
        if (bitboard & notAfile)
            attackBitboard |= (bitboard << 7);
        // if the black pawn is not on H file, down right is possible
        if (bitboard & notHfile)
            attackBitboard |= (bitboard << 9);
    }

    return attackBitboard;
}

U64 Board::getPawnMoveBitboard(int side, int square)
{
    U64 moveBitboard = 0ULL;
    // white turn
    if (!side)
    {
    }
    // black turn
    else
    {

    }

    return moveBitboard;
}

U64 Board::getKnightAttackBitboard(int square)
{
    U64 attackBitboard = 0ULL;
    U64 bitboard = 0ULL;
    setBit(bitboard, square);

    // if the knight is not on A file, up up left and down down left is possible
    if (bitboard & notAfile)
    {
        attackBitboard |= (bitboard >> 17);
        attackBitboard |= (bitboard << 15);
    }
    // if the knight is not on A or B file, up left left and down left left is possible
    if (bitboard & notABfile)
    {
        attackBitboard |= (bitboard >> 10);
        attackBitboard |= (bitboard << 6);
    }
    // if the knight is not on G or H file, up right right and down right right is possible
    if (bitboard & notGHfile)
    {
        attackBitboard |= (bitboard >> 6);
        attackBitboard |= (bitboard << 10);
    }
    // if the knight is not on H file, up up right and down down right is possible
    if (bitboard & notHfile)
    {
        attackBitboard |= (bitboard >> 15);
        attackBitboard |= (bitboard << 17);
    }

    return attackBitboard;
}

U64 Board::getBishopMaskBitboard(int square)
{
    U64 maskBitboard = 0ULL;

    int rank, file;
    int targetRank = square / 8;
    int targetFile = square % 8;

    // up left diagonal
    for (rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; --rank, --file)
        maskBitboard |= (1ULL << (rank * 8 + file));
    // up right diagonal
    for (rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; --rank, ++file)
        maskBitboard |= (1ULL << (rank * 8 + file));
    // down left diagonal
    for (rank = targetRank + 1, file = targetFile - 1; rank <= 6 && file >= 1; ++rank, --file)
        maskBitboard |= (1ULL << (rank * 8 + file));
    // down right diagonal
    for (rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; ++rank, ++file)
        maskBitboard |= (1ULL << (rank * 8 + file));

    return maskBitboard;
}

U64 Board::getBishopAttackBitboardRuntime(U64 board, int square)
{
    U64 attackBitboard = 0ULL;

    int rank, file;
    int targetRank = square / 8;
    int targetFile = square % 8;

    // up left diagonal
    for (rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; --rank, --file)
    {
        attackBitboard |= (1ULL << (rank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + file)) & board)
            break;
    }
    // up right diagonal
    for (rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file <= 7; --rank, ++file)
    {
        attackBitboard |= (1ULL << (rank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + file)) & board)
            break;
    }
    // down left diagonal
    for (rank = targetRank + 1, file = targetFile - 1; rank <= 7 && file >= 0; ++rank, --file)
    {
        attackBitboard |= (1ULL << (rank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + file)) & board)
            break;
    }
    // down right diagonal
    for (rank = targetRank + 1, file = targetFile + 1; rank <= 7 && file <= 7; ++rank, ++file)
    {
        attackBitboard |= (1ULL << (rank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + file)) & board)
            break;
    }

    return attackBitboard;
}

U64 Board::getBishopAttackBitboard(U64 occ, int square)
{
    occ &= bishopAttackMask[square];
    occ *= bishopMagicNumbers[square];
    occ >>= (64 - bishopRelevantBitCount[square]);

    return bishopAttackTable[square][occ];
}

U64 Board::getRookMaskBitboard(int square)
{
    U64 maskBitboard = 0ULL;

    int rank, file;
    int targetRank = square / 8;
    int targetFile = square % 8;

    // up
    for (rank = targetRank - 1; rank >= 1; --rank)
        maskBitboard |= (1ULL << (rank * 8 + targetFile));
    // down
    for (rank = targetRank + 1; rank <= 6; ++rank)
        maskBitboard |= (1ULL << (rank * 8 + targetFile));
    // left
    for (file = targetFile - 1; file >= 1; --file)
        maskBitboard |= (1ULL << (targetRank * 8 + file));
    // right
    for (file = targetFile + 1; file <= 6; ++file)
        maskBitboard |= (1ULL << (targetRank * 8 + file));

    return maskBitboard;
}

U64 Board::getRookAttackBitboardRuntime(U64 board, int square)
{
    U64 attackBitboard = 0ULL;

    int rank, file;
    int targetRank = square / 8;
    int targetFile = square % 8;

    // up
    for (rank = targetRank - 1; rank >= 0; --rank)
    {
        attackBitboard |= (1ULL << (rank * 8 + targetFile));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + targetFile)) & board)
            break;
    }
    // down
    for (rank = targetRank + 1; rank <= 7; ++rank)
    {
        attackBitboard |= (1ULL << (rank * 8 + targetFile));
        // a piece is in the way, stop looking
        if ((1ULL << (rank * 8 + targetFile)) & board)
            break;
    }
    // left
    for (file = targetFile - 1; file >= 0; --file)
    {
        attackBitboard |= (1ULL << (targetRank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (targetRank * 8 + file)) & board)
            break;
    }
    // right
    for (file = targetFile + 1; file <= 7; ++file)
    {
        attackBitboard |= (1ULL << (targetRank * 8 + file));
        // a piece is in the way, stop looking
        if ((1ULL << (targetRank * 8 + file)) & board)
            break;
    }

    return attackBitboard;
}

U64 Board::getRookAttackBitboard(U64 occ, int square)
{
    occ &= rookAttackMask[square];
    occ *= rookMagicNumbers[square];
    occ >>= (64 - rookRelevantBitCount[square]);

    return rookAttackTable[square][occ];
}

U64 Board::getQueenAttackBitboard(U64 occ, int square)
{
    U64 queenAttackBitboard = 0ULL;

    U64 bishopOcc = occ;
    U64 rookOcc = occ;

    // add bishop attacks with current board occupancy
    bishopOcc &= bishopAttackMask[square];
    bishopOcc *= bishopMagicNumbers[square];
    bishopOcc >>= (64 - bishopRelevantBitCount[square]);
    queenAttackBitboard = bishopAttackTable[square][bishopOcc];

    // add rook attacks with current board occupancy
    rookOcc &= rookAttackMask[square];
    rookOcc *= rookMagicNumbers[square];
    rookOcc >>= (64 - rookRelevantBitCount[square]);
    queenAttackBitboard |= rookAttackTable[square][rookOcc];

    return queenAttackBitboard;
}

U64 Board::getKingAttackBitboard(int square)
{
    U64 attackBitboard = 0ULL;
    U64 bitboard = 0ULL;
    setBit(bitboard, square);

    // if the king is not on A file, up left and left and down left is possible
    if (bitboard & notAfile)
    {
        attackBitboard |= (bitboard >> 9);
        attackBitboard |= (bitboard >> 1);
        attackBitboard |= (bitboard << 7);
    }
    // if the king is not on H file, up right and right and down right is possible
    if (bitboard & notHfile)
    {
        attackBitboard |= (bitboard >> 7);
        attackBitboard |= (bitboard << 1);
        attackBitboard |= (bitboard << 9);
    }
    // up and down is possible
    attackBitboard |= (bitboard >> 8);
    attackBitboard |= (bitboard << 8);

    return attackBitboard;
}

void Board::initSliderAttacks(bool isBishop)
{
    // init slider pieces' masks
    for (int square = 0; square < 64; ++square)
    {
        bishopAttackMask[square] = getBishopMaskBitboard(square);

        rookAttackMask[square] = getRookMaskBitboard(square);

        U64 currentMask = isBishop ? bishopAttackMask[square] : rookAttackMask[square];
        int bitCount = countBits(currentMask);

        int occupancyIndex = (1 << bitCount);

        for (int i = 0; i < occupancyIndex; ++i)
        {
            // init current occ
            U64 occ = setOccupancy(currentMask, i, bitCount);
            int magicIndex;

            if (isBishop)
            {
                magicIndex = (occ * bishopMagicNumbers[square]) >> (64 - bishopRelevantBitCount[square]);
                bishopAttackTable[square][magicIndex] = getBishopAttackBitboardRuntime(occ, square);
            }
            // rook
            else
            {
                magicIndex = (occ * rookMagicNumbers[square]) >> (64 - rookRelevantBitCount[square]);
                rookAttackTable[square][magicIndex] = getRookAttackBitboardRuntime(occ, square);
            }
        }
    }
}

// move generator methods

bool Board::isSquareAttacked(int side, int square)
{
    // white
    if (side == white)
    {
        if (pawnAttackTable[black][square] & m_pieces[whitePawn])
            return true;
        else if (knightAttackTable[square] & m_pieces[whiteKnight])
            return true;
        else if (getBishopAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[whiteBishop])
            return true;
        else if (getRookAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[whiteRook])
            return true;
        else if (getQueenAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[whiteQueen])
            return true;
        else if (kingAttackTable[square] & m_pieces[whiteKing])
            return true;
    }
    // black
    else
    {
        if (pawnAttackTable[white][square] & m_pieces[blackPawn])
            return true;
        else if (knightAttackTable[square] & m_pieces[blackKnight])
            return true;
        else if (getBishopAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[blackBishop])
            return true;
        else if (getRookAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[blackRook])
            return true;
        else if (getQueenAttackBitboard(getOccupiedBitboard(both), square) & m_pieces[blackQueen])
            return true;
        else if (kingAttackTable[square] & m_pieces[blackKing])
            return true;
    }

    return false;
}

// magic number methods
unsigned int Board::getRandomU32()
{
    unsigned int number = m_random;

    number ^= (number << 13);
    number ^= (number >> 17);
    number ^= (number << 5);

    m_random = number;
    return number;
}

// Tord Romstad's implementation
U64 Board::getRandomU64()
{
    U64 n1, n2, n3, n4;

    n1 = (U64)(getRandomU32()) & 0xFFFF;
    n2 = (U64)(getRandomU32()) & 0xFFFF;
    n3 = (U64)(getRandomU32()) & 0xFFFF;
    n4 = (U64)(getRandomU32()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 Board::generateMagicNumber()
{
    return getRandomU64() & getRandomU64() & getRandomU64();
}

U64 Board::findMagicNumber(int square, int maskBitCount, bool isBishop)
{
    U64 occupancies[4096];
    U64 attackBitboard[4096];
    U64 usedAttackBitboard[4096];

    // init mask bitboard
    U64 maskBitboard = isBishop ? getBishopMaskBitboard(square) : getRookMaskBitboard(square);

    int occupancyIndex = 1 << maskBitCount;

    for (int i = 0; i < occupancyIndex; ++i)
    {
        // init occupancies
        occupancies[i] = setOccupancy(maskBitboard, i, maskBitCount);

        // init attack bitboard
        attackBitboard[i] = isBishop ? getBishopAttackBitboardRuntime(occupancies[i], square) : getRookAttackBitboardRuntime(occupancies[i], square);
    }

    // test magic numbers
    for (int random = 0; random < 100000000; ++random)
    {
        U64 magicNumber = generateMagicNumber();

        if (countBits((maskBitboard * magicNumber) & 0xFF00000000000000) < 6) continue;

        int index, fail;
        memset(usedAttackBitboard, 0ULL, sizeof(usedAttackBitboard));
        for (index = 0, fail = 0; !fail && index < occupancyIndex; ++index)
        {
            // init magic index
            int magicIndex = (int)((occupancies[index] * magicNumber) >> (64 - maskBitCount));

            if (usedAttackBitboard[magicIndex] == 0ULL)
            {
                // magic index works
                usedAttackBitboard[magicIndex] = attackBitboard[index];
            }
            else if (usedAttackBitboard[magicIndex] != attackBitboard[index])
            {
                // magic index doesn't work
                fail = 1;
            }

            if (!fail)
            {
                return magicNumber;
            }
                
        }

        printf("Magic Number Failed\n");
        return 0ULL;
    }
}

void Board::initMagicNumbers()
{
    for (int square = 0; square < 64; ++square)
    {
        //rookMagicNumbers[square] = findMagicNumber(square, rookRelevantBitCount[square], 0);
    }
    for (int square = 0; square < 64; ++square)
    {
        //bishopMagicNumbers[square] = findMagicNumber(square, bishopRelevantBitCount[square], 1);
    }
}

U64 Board::setOccupancy(U64 maskBitboard, int index, int maskBitCount)
{
    U64 occupancy = 0ULL;

    for (int count = 0; count < maskBitCount; ++count)
    {
        unsigned long square;
        getLSB(square, maskBitboard);
        popBit(maskBitboard, square);

        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

void Board::printBitboard(U64 bitboard)
{
    printf("\n");
    
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int square = i * 8 + j;

            // print ranks
            if (!j)
                printf("%d  ", 8 - i);

            printf("%d ", getBit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }

    printf("\n   a b c d e f g h\n\n");
}

U64 Board::getPieceBitboard(int pt) const
{
    return m_pieces[pt];
}

U64 Board::getEmptyBitboard() const
{
    return m_emptyBitboard;
}

void Board::setOccupiedBitboard(int side, U64 bitboard)
{
    m_occupiedBitboard[side] |= bitboard;
}

void Board::setOccupiedBitboardSquare(int side, int square)
{
    setBit(m_occupiedBitboard[side], square);
}

void Board::clearOccupiedBitboardSquare(int side, int square)
{
    popBit(m_occupiedBitboard[side], square);
}

void Board::updateOccupiedBitboards()
{
    resetOccupiedBitboards();

    for (int i = 0; i < 12; ++i)
    {
        // both
        setOccupiedBitboard(both, m_pieces[i]);
        // white pieces
        if (i % 2 == 0)
        {
            setOccupiedBitboard(white, m_pieces[i]);
        }
        // black pieces
        else
        {
            setOccupiedBitboard(black, m_pieces[i]);
        }
    }
}

void Board::resetOccupiedBitboards()
{
    for (int i = 0; i < 3; ++i)
        m_occupiedBitboard[i] = 0ULL;
}

void Board::resetBoard()
{
    for (int i = 0; i < 12; ++i)
        m_pieces[i] = 0ULL;

    resetOccupiedBitboards();

    m_emptyBitboard = 0ULL;

    m_side = white;
    m_enPassant = noSquare;
    m_castle = none;
}
