#include "GameLoop.h"
#include "Board.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool parseFEN(Board* board, char *fen)
{
    // init piece placement
    int i = 0;
    for (int square = 0; square < 64; ++i)
    {
        // if current character is an alphabet, set pieces accordingly
        if ((fen[i] >= 'a' && fen[i] <= 'z') || (fen[i] >= 'A' && fen[i] <= 'Z'))
        {
            switch (fen[i])
            {
                case 'p':
                    // black rook
                    board->setBlackPawns(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'n':
                    // black knight
                    board->setBlackKnights(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'b':
                    // black bishop
                    board->setBlackBishops(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'r':
                    // black rook
                    board->setBlackRooks(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'q':
                    // black queen
                    board->setBlackQueens(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'k':
                    // black king
                    board->setBlackKing(square);
                    board->setOccupiedBitboardSquare(board->black, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'P':
                    // white pawn
                    board->setWhitePawns(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'N':
                    // white knight
                    board->setWhiteKnights(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'B':
                    // white bishop
                    board->setWhiteBishops(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'R':
                    // white rook
                    board->setWhiteRooks(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'Q':
                    // white queen
                    board->setWhiteQueens(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                case 'K':
                    // white king
                    board->setWhiteKing(square);
                    board->setOccupiedBitboardSquare(board->white, square);
                    board->setOccupiedBitboardSquare(board->both, square);
                    square += 1;
                    break;
                default:
                    printf("Wrong alphabet in the FEN string\n"); // DEBUG
                    return false;
            }
        }
        else if (fen[i] >= '0' && fen[i] <= '9')
        {
            int numEmpty = fen[i] - '0';
            square += numEmpty;
        }
        else if (fen[i] == '/')
        {
            continue;
        }
        else
        {
            printf("WEIRD CHARACTER IN FEN STRING\n"); // DEBUG
            return false;
        }
    }
    board->updateEmptyBitboards();

    // skip the first space
    i += 1;

    // init side
    if (fen[i] == 'w')
        board->setSide(board->white);
    else if (fen[i] == 'b')
        board->setSide(board->black);
    else
        return false;
    i += 1;

    // skip the second space
    i += 1;

    int castleFlags = board->none;
    // init castling rights
    while (fen[i] != ' ')
    {
        switch (fen[i])
        {
            case 'K':
                castleFlags |= board->whiteKingSide;
                break;
            case 'Q':
                castleFlags |= board->whiteQueenSide;
                break;
            case 'k':
                castleFlags |= board->blackKingSide;
                break;
            case 'q':
                castleFlags |= board->blackQueenSide;
                break;
            case '-':
                break;
            default:
                return false;
        }
        i += 1;
    }
    board->setCastlingRights(castleFlags);

    // skip the third space
    i += 1;

    int r = -1, f = -1;
    bool first = true;

    // init en passant target square
    while (fen[i] != ' ')
    {
        if (fen[i] == '-')
        {
            board->setEnPassantSquare(board->noSquare);
            i += 1;
            break;
        }
        else
        {
            if (first)
            {
                f = fen[i] - 'a';
                first = false;
            }
            else
            {
                r = fen[i] - '0';
                r = 8 - r;
                board->setEnPassantSquare(8 * r + f);
            }
            i += 1;
        }
    }

    // skip the fourth space
    i += 1;

    // TODO: Halfmove clock and Fullmove counter

    // init halfmove clock (up to 3 digit number)
    int halfMove = 0;
    int numDigit = 0;
    int firstDigit = 0;
    int secondDigit = 0;
    int thirdDigit = 0;
    while (fen[i] != ' ')
    {
        numDigit++;
        if (numDigit == 1)
            firstDigit = fen[i] - '0';
        else if (numDigit == 2)
            secondDigit = fen[i] - '0';
        else if (numDigit == 3)
            thirdDigit = fen[i] - '0';

        i += 1;
    }
    if (numDigit == 1)
        halfMove = firstDigit;
    else if (numDigit == 2)
        halfMove = firstDigit * 10 + secondDigit * 1;
    else if (numDigit == 3)
        halfMove = firstDigit * 100 + secondDigit * 10 + thirdDigit * 1;
    printf("halfmove: %d\n", halfMove); // DEBUG
    
    // skip the fifth space
    i += 1;

    // init fullmove counter (up to 3 digit number)
    int fullMove = 0;
    numDigit = 0;
    while (fen[i] != ' ' && fen[i] != '\0')
    {
        numDigit++;
        if (numDigit == 1)
            firstDigit = fen[i] - '0';
        else if (numDigit == 2)
            secondDigit = fen[i] - '0';
        else if (numDigit == 3)
            thirdDigit = fen[i] - '0';

        i += 1;
    }
    if (numDigit == 1)
        fullMove = firstDigit;
    else if (numDigit == 2)
        fullMove = firstDigit * 10 + secondDigit * 1;
    else if (numDigit == 3)
        fullMove = firstDigit * 100 + secondDigit * 10 + thirdDigit * 1;
    printf("fullmove: %d\n\n", fullMove); // DEBUG


    return true;
}

void runGameLoop(GLFWwindow* window)
{
    // Our state
    bool show_demo_window = false;
    //bool show_another_window = false;

    // init styles
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const ImVec2 BOARD_TILE = ImVec2(100.0f, 100.0f);
    const ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    const ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    const ImVec4 lightTile = ImVec4(0.9f, 0.8f, 0.7f, 1.0f);
    const ImVec4 darkTile = ImVec4(0.8f, 0.5f, 0.3f, 1.0f);
    const ImVec4 possibleCaptureTile = ImVec4(0.7f, 0.1f, 0.1f, 1.0f);
    const ImVec4 possibleMoveTile = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
    const ImVec4 selectedTile = ImVec4(0.77f, 0.63f, 0.32f, 1.0f);
    const ImVec4 noTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // load images
    int pieceImageWidth = 0;
    int pieceImageHeight = 0;
    GLuint whitePawnImageTexture = 0;
    GLuint blackPawnImageTexture = 0;
    GLuint whiteKnightImageTexture = 0;
    GLuint blackKnightImageTexture = 0;
    GLuint whiteBishopImageTexture = 0;
    GLuint blackBishopImageTexture = 0;
    GLuint whiteRookImageTexture = 0;
    GLuint blackRookImageTexture = 0;
    GLuint whiteQueenImageTexture = 0;
    GLuint blackQueenImageTexture = 0;
    GLuint whiteKingImageTexture = 0;
    GLuint blackKingImageTexture = 0;
    GLuint blankTexture = 0;
    bool ret = LoadTextureFromFile("../assets/whitePawn.png", &whitePawnImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackPawn.png", &blackPawnImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/whiteKnight.png", &whiteKnightImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackKnight.png", &blackKnightImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/whiteBishop.png", &whiteBishopImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackBishop.png", &blackBishopImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/whiteRook.png", &whiteRookImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackRook.png", &blackRookImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/whiteQueen.png", &whiteQueenImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackQueen.png", &blackQueenImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/whiteKing.png", &whiteKingImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blackKing.png", &blackKingImageTexture, &pieceImageWidth, &pieceImageHeight);
    ret &= LoadTextureFromFile("../assets/blank.png", &blankTexture, &pieceImageWidth, &pieceImageHeight);
    IM_ASSERT(ret);

    // init game state
    Board board;
    bool clickedOnPiece = false;
    int fromSquare = -1;
    //int toSquare = -1;
    Board::PieceTypes pieceSelected = board.whitePawn;
    U64 possibleMoves = 0ULL;
    U64 possibleCaptures = 0ULL;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwWaitEvents();

        // start the Dear ImGui frame only when mouse button is pressed or if it's the starting frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(900, 905));
        ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // FEN string parser input text field
        static char input[91] = "";
        if (ImGui::InputText("FEN string", input, 91, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            //char* fen = input;
            printf("Entered: %s\n\n", input); // DEBUG

            // reset board
            board.resetBoard();

            // parse FEN
            if (!parseFEN(&board, input))
                printf("ERROR: INCORRECT FEN STRING\n"); // DEBUG

            // clear input text field
            strcpy(input, "");
        }

        ImGui::SameLine();
        if (ImGui::Button("DEBUG"))
        {
            // DEBUG
            printf("CURRENT EN PASSANT SQUARE %d\n", board.getEnPassantSquare());
        }

        // show who's turn it is right now
        ImGui::Text("TURN: %s", board.getSide() ? "BLACK" : "WHITE");

        ImGui::Separator();

        float rankPosY = 0.0f;
        float filePosX = 0.0f;
        for (int square = 0, pattern = 0; square < 64; ++square)
        {
            ImGui::PushID(square);

            // show ranks
            if (square % 8 == 0)
            {
                rankPosY = ImGui::GetCursorPosY();
                ImVec2 rankPos = ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 40.0f);
                ImGui::SetCursorPos(rankPos);
                ImGui::Text("%d", 8 - (square / 8));
                ImGui::SameLine();
                filePosX = ImGui::GetCursorPosX();
            }
            
            // 8 files in a rank
            if ((square % 8) > 0)
            {
                pattern++;
                ImGui::SameLine();
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), rankPosY));

            ImVec4 bg;
            // draw tile
            if (pattern % 2 == 0)
                bg = lightTile;
            else
                bg = darkTile;

            if ((possibleMoves >> square) & 1ULL)
                bg = possibleMoveTile;
            else if ((possibleCaptures >> square) & 1ULL)
                bg = possibleCaptureTile;
            else if (fromSquare == square)
                bg = selectedTile;

            // clicked on an empty square
            if (((board.getOccupiedBitboard(board.both) >> square) & 1ULL) == 0)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blankTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // if no piece has been clicked
                    if (!clickedOnPiece)
                    {
                        printf("DO NOTHING\n"); // DEBUG
                    }
                    // if empty square is clicked after clicking on a piece first, validate the move
                    else
                    {
                        // check if this is one of the possible moves
                        if ((possibleMoves >> square) & 1ULL)
                        {
                            // move the piece to an empty square
                            printf("MOVED A PIECE TO AN EMPTY SQUARE %d %d\n", pieceSelected, square); // DEBUG
                            board.setPieceBitboard(pieceSelected, square);
                            board.popPieceBitboard(pieceSelected, fromSquare);
                            board.updateOccupiedBitboards();
                            board.updateEmptyBitboards();

                            // switch side
                            board.flipSide();

                            // set en passant square
                            if (pieceSelected == board.whitePawn)
                            {
                                if (fromSquare - square == 16)
                                {
                                    printf("SETTING EN PASSANT SQUARE\n"); // DEBUG
                                    board.setEnPassantSquare(square + 8);
                                }
                                // pawn promotion
                                else
                                {
                                    if (fromSquare - square == 8 && square >= board.a8 && square <= board.h8)
                                    {
                                        printf("WHITE PAWN MOVE PROMOTION\n"); // DEBUG
                                        board.popPieceBitboard(board.whitePawn, square);
                                        board.setPieceBitboard(board.whiteQueen, square);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                    }
                                    board.setEnPassantSquare(board.noSquare);
                                }
                            }
                            else if (pieceSelected == board.blackPawn)
                            {
                                if (square - fromSquare == 16)
                                {
                                    printf("SETTING EN PASSANT SQUARE\n"); // DEBUG
                                    board.setEnPassantSquare(square - 8);
                                }
                                else
                                {
                                    if (square - fromSquare == 8 && square >= board.a1 && square <= board.h1)
                                    {
                                        printf("BLACK PAWN MOVE PROMOTION\n"); // DEBUG
                                        board.popPieceBitboard(board.blackPawn, square);
                                        board.setPieceBitboard(board.blackQueen, square);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                    }
                                    board.setEnPassantSquare(board.noSquare);
                                }
                            }
                            // white castling
                            else if (pieceSelected == board.whiteKing)
                            {
                                if (square == board.g1 && fromSquare == board.e1)
                                {
                                    if (board.getCastlingRights() & board.whiteKingSide)
                                    {
                                        printf("WHITE KING SIDE CASTLE AVAILABLE\n"); // DEBUG
                                        board.popPieceBitboard(board.whiteRook, board.h1);
                                        board.setPieceBitboard(board.whiteRook, board.f1);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                        board.setEnPassantSquare(board.noSquare);
                                    }
                                }
                                else if (square == board.c1 && fromSquare == board.e1)
                                {
                                    if (board.getCastlingRights() & board.whiteQueenSide)
                                    {
                                        printf("WHITE QUEEN SIDE CASTLE AVAILABLE\n"); // DEBUG
                                        board.popPieceBitboard(board.whiteRook, board.a1);
                                        board.setPieceBitboard(board.whiteRook, board.d1);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                        board.setEnPassantSquare(board.noSquare);
                                    }
                                }
                                // white king moved, white can't castle now
                                board.setCastlingRights(board.getCastlingRights() & 12);
                            }
                            // black castling
                            else if (pieceSelected == board.blackKing)
                            {
                                if (square == board.g8 && fromSquare == board.e8)
                                {
                                    if (board.getCastlingRights() & board.blackKingSide)
                                    {
                                        printf("BLACK KING SIDE CASTLE AVAILABLE\n"); // DEBUG
                                        board.popPieceBitboard(board.blackRook, board.h8);
                                        board.setPieceBitboard(board.blackRook, board.f8);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                        board.setEnPassantSquare(board.noSquare);
                                    }
                                }
                                else if (square == board.c8 && fromSquare == board.e8)
                                {
                                    if (board.getCastlingRights() & board.blackQueenSide)
                                    {
                                        printf("BLACK QUEEN SIDE CASTLE AVAILABLE\n"); // DEBUG
                                        board.popPieceBitboard(board.blackRook, board.a8);
                                        board.setPieceBitboard(board.blackRook, board.d8);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();
                                        board.setEnPassantSquare(board.noSquare);
                                    }
                                }
                                // black king moved, black can't castle now
                                board.setCastlingRights(board.getCastlingRights() & 3);
                            }
                            else
                            {
                                board.setEnPassantSquare(board.noSquare);
                            }
                        }
                        else
                        {
                            // check en passant
                            if (square == board.getEnPassantSquare())
                            {
                                if (pieceSelected == board.whitePawn)
                                {
                                    if (fromSquare - square == 7 || fromSquare - square == 9)
                                    {
                                        printf("EN PASSANT\n"); // DEBUG
                                        board.setPieceBitboard(pieceSelected, square);
                                        board.popPieceBitboard(pieceSelected, fromSquare);
                                        board.popPieceBitboard(board.blackPawn, square + 8);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();

                                        // switch side
                                        board.flipSide();
                                    }
                                }
                                else if (pieceSelected == board.blackPawn)
                                {
                                    if (square - fromSquare == 7 || square - fromSquare == 9)
                                    {
                                        printf("EN PASSANT\n"); // DEBUG
                                        board.setPieceBitboard(pieceSelected, square);
                                        board.popPieceBitboard(pieceSelected, fromSquare);
                                        board.popPieceBitboard(board.whitePawn, square - 8);
                                        board.updateOccupiedBitboards();
                                        board.updateEmptyBitboards();

                                        // switch side
                                        board.flipSide();
                                    }
                                }
                            }
                            else
                            {
                                printf("INVALID MOVE\n"); // DEBUG
                            }
                        }

                        // reset move state
                        clickedOnPiece = false;
                        possibleMoves = 0ULL;
                        possibleCaptures = 0ULL;
                        fromSquare = -1;
                    }
                }
                ImGui::PopID();
                continue;
            }

            // draw piece
            if ((board.getWhitePawns() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whitePawnImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white pawn
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whitePawn;
                            // compute possible moves and captures
                            possibleMoves = board.getPawnMoveBitboard(board.white, square);
                            possibleCaptures = pawnAttackTable[board.white][square] & board.getOccupiedBitboard(board.black);
                            if (board.getEnPassantSquare() >= 0)
                                possibleCaptures |= (pawnAttackTable[board.white][square] & (1ULL << board.getEnPassantSquare()));

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // black turn trying to capture a white pawn
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white pawn
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                printf("CAPTURING WHITE PAWN\n"); // DEBUG
                                board.popPieceBitboard(board.whitePawn, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                board.setPieceBitboard(pieceSelected, square);
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.blackKing)
                                {
                                    // black king moved, black can't castle now
                                    printf("BLACK KING MOVED TO CAPTURE A PIECE - BLACK CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 3);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackPawns() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackPawnImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black pawn
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackPawn;
                            // compute possible moves or captures
                            possibleMoves = board.getPawnMoveBitboard(board.black, square);
                            possibleCaptures = pawnAttackTable[board.black][square] & board.getOccupiedBitboard(board.white);
                            if (board.getEnPassantSquare() >= 0)
                                possibleCaptures |= (pawnAttackTable[board.black][square] & (1ULL << board.getEnPassantSquare()));

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn clicking on a black pawn
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture black pawn
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                printf("CAPTURING BLACK PAWN\n"); // DEBUG
                                board.popPieceBitboard(board.blackPawn, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                board.setPieceBitboard(pieceSelected, square);
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.whiteKing)
                                {
                                    // white king moved, white can't castle now
                                    printf("WHITE KING MOVED TO CAPTURE A PIECE - WHITE CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 12);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getWhiteKnights() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whiteKnightImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white knight
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whiteKnight;
                            // compute possible moves and captures
                            possibleMoves = knightAttackTable[square] & board.getEmptyBitboard();
                            possibleCaptures = knightAttackTable[square] & board.getOccupiedBitboard(board.black);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                        
                    }
                    // black turn trying to capture a white knight
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white knight
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.whiteKnight, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.blackPawn && (square - fromSquare == 7 || square - fromSquare == 9))
                                {
                                    printf("CAPTURING WHITE KNIGHT AND PROMOTING BLACK PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.blackQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING WHITE KNIGHT\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.blackKing)
                                {
                                    // black king moved, black can't castle now
                                    printf("BLACK KING MOVED TO CAPTURE A PIECE - BLACK CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 3);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackKnights() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackKnightImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black knight
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackKnight;
                            // compute possible moves and captures
                            possibleMoves = knightAttackTable[square] & board.getEmptyBitboard();
                            possibleCaptures = knightAttackTable[square] & board.getOccupiedBitboard(board.white);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn trying to capture a black knight
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture black knight
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.blackKnight, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.whitePawn && (fromSquare - square == 7 || fromSquare - square == 9))
                                {
                                    printf("CAPTURING BLACK KNIGHT AND PROMOTING WHITE PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.whiteQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING BLACK KNIGHT\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.whiteKing)
                                {
                                    // white king moved, white can't castle now
                                    printf("WHITE KING MOVED TO CAPTURE A PIECE - WHITE CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 12);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getWhiteBishops() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whiteBishopImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white bishop
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whiteBishop;
                            // compute possible moves and captures
                            U64 bishopMoves = board.getBishopAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = bishopMoves & board.getEmptyBitboard();
                            possibleCaptures = bishopMoves & board.getOccupiedBitboard(board.black);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // black turn trying to capture a white bishop
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white bishop
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.whiteBishop, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.blackPawn && (square - fromSquare == 7 || square - fromSquare == 9))
                                {
                                    printf("CAPTURING WHITE BISHOP AND PROMOTING BLACK PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.blackQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING WHITE BISHOP\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.blackKing)
                                {
                                    // black king moved, black can't castle now
                                    printf("BLACK KING MOVED TO CAPTURE A PIECE - BLACK CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 3);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackBishops() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackBishopImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black bishop
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackBishop;
                            // compute possible moves and captures
                            U64 bishopMoves = board.getBishopAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = bishopMoves & board.getEmptyBitboard();
                            possibleCaptures = bishopMoves & board.getOccupiedBitboard(board.white);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn trying to capture a black bishop
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture black bishop
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.blackBishop, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.whitePawn && (fromSquare - square == 7 || fromSquare - square == 9))
                                {
                                    printf("CAPTURING BLACK BISHOP AND PROMOTING WHITE PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.whiteQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING BLACK BISHOP\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.whiteKing)
                                {
                                    // white king moved, white can't castle now
                                    printf("WHITE KING MOVED TO CAPTURE A PIECE - WHITE CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 12);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getWhiteRooks() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whiteRookImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white rook
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whiteRook;
                            // compute possible moves and captures
                            U64 rookMoves = board.getRookAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = rookMoves & board.getEmptyBitboard();
                            possibleCaptures = rookMoves & board.getOccupiedBitboard(board.black);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // black turn trying to capture a white rook
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white rook
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.whiteRook, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.blackPawn && (square - fromSquare == 7 || square - fromSquare == 9))
                                {
                                    printf("CAPTURING WHITE ROOK AND PROMOTING BLACK PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.blackQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING WHITE ROOK\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.blackKing)
                                {
                                    // black king moved, black can't castle now
                                    printf("BLACK KING MOVED TO CAPTURE A PIECE - BLACK CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 3);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackRooks() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackRookImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black rook
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackRook;
                            // compute possible moves and captures
                            U64 rookMoves = board.getRookAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = rookMoves & board.getEmptyBitboard();
                            possibleCaptures = rookMoves & board.getOccupiedBitboard(board.white);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn trying to capture a black rook
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white rook
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.blackRook, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.whitePawn && (fromSquare - square == 7 || fromSquare - square == 9))
                                {
                                    printf("CAPTURING BLACK ROOK AND PROMOTING WHITE PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.whiteQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING BLACK ROOK\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.whiteKing)
                                {
                                    // white king moved, white can't castle now
                                    printf("WHITE KING MOVED TO CAPTURE A PIECE - WHITE CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 12);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getWhiteQueens() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whiteQueenImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white queen
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whiteQueen;
                            // compute possible moves and captures
                            U64 queenMoves = board.getQueenAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = queenMoves & board.getEmptyBitboard();
                            possibleCaptures = queenMoves & board.getOccupiedBitboard(board.black);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // black turn trying to capture a white queen
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white rook
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.whiteQueen, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.blackPawn && (square - fromSquare == 7 || square - fromSquare == 9))
                                {
                                    printf("CAPTURING WHITE QUEEN AND PROMOTING BLACK PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.blackQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING WHITE QUEEN\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.blackKing)
                                {
                                    // black king moved, black can't castle now
                                    printf("BLACK KING MOVED TO CAPTURE A PIECE - BLACK CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 3);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackQueens() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackQueenImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black queen
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackQueen;
                            // compute possible moves and captures
                            U64 queenMoves = board.getQueenAttackBitboard(board.getOccupiedBitboard(board.both), square);
                            possibleMoves = queenMoves & board.getEmptyBitboard();
                            possibleCaptures = queenMoves & board.getOccupiedBitboard(board.white);

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn trying to capture a black queen
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white rook
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                board.popPieceBitboard(board.blackQueen, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                if (pieceSelected == board.whitePawn && (fromSquare - square == 7 || fromSquare - square == 9))
                                {
                                    printf("CAPTURING BLACK QUEEN AND PROMOTING WHITE PAWN\n"); // DEBUG
                                    board.setPieceBitboard(board.whiteQueen, square);
                                }
                                else
                                {
                                    printf("CAPTURING BLACK QUEEN\n"); // DEBUG
                                    board.setPieceBitboard(pieceSelected, square);
                                }
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                if (pieceSelected == board.whiteKing)
                                {
                                    // white king moved, white can't castle now
                                    printf("WHITE KING MOVED TO CAPTURE A PIECE - WHITE CANNOT CASTLE NOW\n"); // DEBUG
                                    board.setCastlingRights(board.getCastlingRights() & 12);
                                }

                                // switch side
                                board.flipSide();
                                fromSquare = -1;
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getWhiteKing() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)whiteKingImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // white turn clicking on a white king
                    if (board.getSide() == board.white)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.whiteKing;
                            // compute possible moves and captures
                            U64 emptyBitboard = board.getEmptyBitboard();
                            possibleMoves = kingAttackTable[square] & emptyBitboard;
                            possibleCaptures = kingAttackTable[square] & board.getOccupiedBitboard(board.black);

                            // compute possible castling
                            if (board.getCastlingRights() & board.whiteKingSide)
                            {
                                // white king side castling
                                if ((emptyBitboard >> board.f1) & 1 && (emptyBitboard >> board.g1) & 1)
                                {
                                    // the castling squares are empty
                                    printf("WHITE KING SIDE CASTLING IS POSSIBLE\n"); // DEBUG
                                    possibleMoves |= (1ULL << board.g1);
                                }
                                else
                                {
                                    printf("WHITE KING SIDE CASTLING IS NOT POSSIBLE - A PIECE IS BLOCKING THE WAY\n"); // DEBUG
                                }
                            }

                            // compute possible castling
                            if (board.getCastlingRights() & board.whiteQueenSide)
                            {
                                // white queen side castling
                                if ((emptyBitboard >> board.b1) & 1 && (emptyBitboard >> board.c1) & 1 && (emptyBitboard >> board.d1) & 1)
                                {
                                    // the castling squares are empty
                                    printf("WHITE QUEEN SIDE CASTLING IS POSSIBLE\n"); // DEBUG
                                    possibleMoves |= (1ULL << board.c1);
                                }
                                else
                                {
                                    printf("WHITE QUEEN SIDE CASTLING IS NOT POSSIBLE - A PIECE IS BLOCKING THE WAY\n"); // DEBUG
                                }
                            }

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // black turn trying to capture a white king
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture white king
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                printf("CAPTURING WHITE KING\n"); // DEBUG
                                board.popPieceBitboard(board.whiteKing, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                board.setPieceBitboard(pieceSelected, square);
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                // switch side
                                board.flipSide();
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR BLACK\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }
            else if ((board.getBlackKing() >> square) & 1ULL)
            {
                if (ImGui::ImageButton((void*)(intptr_t)blackKingImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // black turn clicking on a black king
                    if (board.getSide() == board.black)
                    {
                        // hasn't clicked on a piece yet
                        if (!clickedOnPiece || fromSquare != square)
                        {
                            fromSquare = square;
                            pieceSelected = board.blackKing;
                            // compute possible moves and captures
                            U64 emptyBitboard = board.getEmptyBitboard();
                            possibleMoves = kingAttackTable[square] & emptyBitboard;
                            possibleCaptures = kingAttackTable[square] & board.getOccupiedBitboard(board.white);

                            // compute possible castling
                            if (board.getCastlingRights() & board.blackKingSide)
                            {
                                // black king side castling
                                if ((emptyBitboard >> board.f8) & 1 && (emptyBitboard >> board.g8) & 1)
                                {
                                    // the castling squares are empty
                                    printf("BLACK KING SIDE CASTLING IS POSSIBLE\n"); // DEBUG
                                    possibleMoves |= (1ULL << board.g8);
                                }
                                else
                                {
                                    printf("BLACK KING SIDE CASTLING IS NOT POSSIBLE - A PIECE IS BLOCKING THE WAY\n"); // DEBUG
                                }
                            }

                            // compute possible castling
                            if (board.getCastlingRights() & board.blackQueenSide)
                            {
                                // black queen side castling
                                if ((emptyBitboard >> board.b8) & 1 && (emptyBitboard >> board.c8) & 1 && (emptyBitboard >> board.d8) & 1)
                                {
                                    // the castling squares are empty
                                    printf("BLACK QUEEN SIDE CASTLING IS POSSIBLE\n"); // DEBUG
                                    possibleMoves |= (1ULL << board.c8);
                                }
                                else
                                {
                                    printf("BLACK QUEEN SIDE CASTLING IS NOT POSSIBLE - A PIECE IS BLOCKING THE WAY\n"); // DEBUG
                                }
                            }

                            clickedOnPiece = true;
                            // no possible moves or captures, reset the click state
                            if ((possibleMoves | possibleCaptures) == 0ULL)
                            {
                                printf("NO POSSIBLE MOVES OR CAPTURES\n"); // DEBUG
                                clickedOnPiece = !clickedOnPiece;
                                fromSquare = -1;
                            }
                        }
                        // can't capture your own piece
                        else
                        {
                            printf("CANCEL - RECLICKING SAME PIECE\n"); // DEBUG
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                            clickedOnPiece = !clickedOnPiece;
                        }
                    }
                    // white turn trying to capture a black king
                    else
                    {
                        if (clickedOnPiece)
                        {
                            // a piece is trying to capture black king
                            // check if it is a valid capture
                            if ((possibleCaptures >> square) & 1ULL)
                            {
                                printf("CAPTURING BLACK KING\n"); // DEBUG
                                board.popPieceBitboard(board.blackKing, square);
                                board.popPieceBitboard(pieceSelected, fromSquare);
                                board.setPieceBitboard(pieceSelected, square);
                                board.updateOccupiedBitboards();
                                board.updateEmptyBitboards();

                                // switch side
                                board.flipSide();
                            }
                            else
                            {
                                printf("CANCEL - NOT A VALID CAPTURE FOR WHITE\n"); // DEBUG
                            }

                            // reset move state
                            clickedOnPiece = false;
                            possibleMoves = 0ULL;
                            possibleCaptures = 0ULL;
                            fromSquare = -1;
                        }
                        else
                        {
                            printf("DO NOTHING\n"); // DEBUG
                        }
                    }
                }
            }

            ImGui::PopID();
        }

        ImGui::SetCursorPos(ImVec2(filePosX, ImGui::GetCursorPosY()));
        ImGui::Text("\t   a\t\t\t   b\t\t\t  c\t\t\t  d\t\t\t   e\t\t\t  f\t\t\t  g\t\t\t   h");

        ImGui::End();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        //{
        //    static float f = 0.0f;
        //    static int counter = 0;

        //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //    ImGui::Checkbox("Another Window", &show_another_window);

        //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //        counter++;
        //    ImGui::SameLine();
        //    ImGui::Text("counter = %d", counter);

        //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //    ImGui::End();
        //}

        //// 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}
