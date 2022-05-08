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
                    printf("Wrong alphabet in the FEN string\n");
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
            printf("WEIRD CHARACTER IN FEN STRING\n");
            return false;
        }
    }

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

    int r, f, enPassantSquare;
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
                enPassantSquare = 8 * r + f;
                board->setEnPassantSquare(enPassantSquare);
            }
            i += 1;
        }
    }

    // skip the fourth space
    i += 1;

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
    printf("halfmove: %d\n", halfMove);
    
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
    printf("fullmove: %d\n\n", fullMove);


    return true;
}

void runGameLoop(GLFWwindow* window)
{
    // Our state
    bool show_demo_window = false;
    //bool show_another_window = false;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    const ImVec2 BOARD_TILE = ImVec2(100.0f, 100.0f);
    const ImVec2 uv0 = ImVec2(0.0f, 0.0f);
    const ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    const ImVec4 lightTile = ImVec4(0.9f, 0.8f, 0.7f, 1.0f);
    const ImVec4 darkTile = ImVec4(0.8f, 0.5f, 0.3f, 1.0f);
    const ImVec4 noTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

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



    //selected[square] ^= 1;
    //flipBit(board1, square);

    //board.printBitboard(board.getPawnAttackBitboard(1, square));
    //board.printBitboard(board.getKnightAttackBitboard(square));
    //board.printBitboard(board.getBishopMaskBitboard(square));
    //board.printBitboard(board.getBishopAttackBitboardRuntime(board1, square));
    //board.printBitboard(board.getRookMaskBitboard(square));
    //board.printBitboard(board.getRookAttackBitboardRuntime(board1, square));
    //board.printBitboard(board.getKingAttackBitboard(square));
    //printf("%u\n", (int)countBits(board1));
    //unsigned long index = 0UL;
    //getLSB(index, board1);
    //printf("%d\n", index);

    //U64 occ = 0ULL;
    //setBit(occ, board.c5);
    //board.printBitboard(occ);
    //board.printBitboard(board.getBishopAttackBitboard(occ, square));
    //board.printBitboard(board.getQueenAttackBitboard(occ, board.e5));

    /*printf("WHITE PIECES ON BOARD\n\n");
    board.printBitboard(board.getOccupiedBitboard(0));
    printf("BLACK PIECES ON BOARD\n\n");
    board.printBitboard(board.getOccupiedBitboard(1));
    printf("ALL PIECES ON BOARD\n\n");
    board.printBitboard(board.getOccupiedBitboard(2));*/

    Board board;
    bool clicked = false;
    int fromSquare = -1;
    int toSquare = -1;

    bool isStart = 1;
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
            printf("Entered: %s\n\n", input);

            // reset board
            board.resetBoard();

            // parse FEN
            if (!parseFEN(&board, input))
                printf("ERROR: INCORRECT FEN STRING\n");

            // clear input text field
            strcpy(input, "");
        }

        // show who's turn it is right now
        ImGui::Text("TURN: %s", board.getSide() ? "BLACK" : "WHITE");

        ImGui::Separator();

        float rankPosY;
        float filePosX;
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

            // empty tile
            if (((board.getOccupiedBitboard(board.both) >> square) & 1ULL) == 0)
            {
                //ImGui::Image((void*)(intptr_t)blankTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blankTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                    // first click: select the piece on this square
                    //if (!clicked)
                    //{
                    //    fromSquare = square;
                    //}
                    //// second click: move the piece to this square
                    //else
                    //{
                    //    // check if this is a valid move
                    //    toSquare = square;
                    //}
                    //clicked = !clicked;
                }
                ImGui::PopID();
                continue;
            }
                
            // draw piece
            if ((board.getWhitePawns() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whitePawnImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whitePawnImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    if (board.getSide() == board.white)
                    {
                        // first click: select the piece on this square and generate pawn moves
                        if (!clicked)
                        {
                            fromSquare = square;
                            // compute possible moves or captures

                        }
                        // second click: move the piece to this square
                        else
                        {
                            toSquare = square;

                            // moved the piece, flip the side
                            board.flipSide();
                        }
                        clicked = !clicked;
                    }
                }
            }
            else if ((board.getBlackPawns() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackPawnImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackPawnImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    //if (board.getSide() == board.black)
                    //{
                    //    // only compute when it's its turn
                    //    printf("Square: %d\n", square);
                    //    // getting the selected square tile
                    //    // first click: select the piece on this square
                    //    if (!clicked)
                    //    {
                    //        fromSquare = square;
                    //        // compute possible moves
                    //    }
                    //    // second click: move the piece to this square
                    //    else
                    //    {
                    //        toSquare = square;

                    //        // moved the piece, flip the side
                    //        board.flipSide();
                    //    }
                    //    clicked = !clicked;
                    //    board.flipSide();
                    //}
                }
            }
            else if ((board.getWhiteKnights() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whiteKnightImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whiteKnightImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getBlackKnights() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackKnightImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackKnightImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getWhiteBishops() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whiteBishopImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whiteBishopImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getBlackBishops() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackBishopImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackBishopImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getWhiteRooks() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whiteRookImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whiteRookImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getBlackRooks() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackRookImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackRookImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getWhiteQueens() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whiteQueenImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whiteQueenImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getBlackQueens() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackQueenImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackQueenImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getWhiteKing() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)whiteKingImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)whiteKingImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
                }
            }
            else if ((board.getBlackKing() >> square) & 1ULL)
            {
                //ImGui::Image((void*)(intptr_t)blackKingImageTexture, BOARD_TILE);
                if (ImGui::ImageButton((void*)(intptr_t)blackKingImageTexture, BOARD_TILE, uv0, uv1, 0, bg, noTint))
                {
                    // getting the selected square tile
                    printf("Square: %d\n", square);
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
