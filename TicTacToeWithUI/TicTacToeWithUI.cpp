

#include <iostream>
#include <vector>
#include <utility>
#include "raylib.h"

using namespace std;


#define LOG(x) cout << x

#define number_of_columns 3



#define ROWS 3 
#define COLS 3

char PlayerTurn;

Vector2 MousePos;

struct Cell {
    int x;
    int y;
    int width;
    int height;
    pair<int, int> InVirtualBoardPosition;
    char Input = '0';
    Color CellColor = BLACK;

    const char* label = "";


    bool IsClecked = false;


public:
    Cell(int xpos, int ypos, int wid, int hei, const pair<int, int>& position) : x(xpos), y(ypos), width(wid), height(hei), InVirtualBoardPosition(position) {}

    bool HoveredOn() {
        if ((MousePos.x >= x) && (MousePos.x <= x + width)) {
            if ((MousePos.y >= y) && (MousePos.y <= y + height)) {
                return true;
            }
        }
        return false;
    }

    void show() {
        DrawRectangle(x, y, width, height, CellColor);
        DrawText(label, (x + width / 2) - (width / 3), (y + height / 2) - (width / 2), width, RAYWHITE); //the most hard coded thing i have ever coded (: 

        if (HoveredOn()) {
            CellColor = GRAY;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                IsClecked = true;
            }
        }
        else {
            CellColor = BLACK;
        }

    }


    bool CellIsClecked() {
        return IsClecked;
    }

};

struct Board {
    vector<vector<char>> board_buffer = {
        {'0', '0', '0'},
        {'0', '0', '0'},
        {'0', '0', '0'}
    };
};




struct UIBoard {
    static vector<Cell> box;
};

vector<Cell> UIBoard::box = {
    {50, 50, 50, 50, {0, 0} },
    {120, 50, 50, 50, {0,1} },
    {190, 50, 50, 50, {0,2} },

    {50, 120, 50, 50, {1, 0} },
    {120, 120, 50, 50, {1,1} },
    {190, 120, 50, 50, {1,2} },

    {50, 190, 50, 50, {2, 0} },
    {120, 190, 50, 50, {2,1} },
    {190, 190, 50, 50, {2,2} },
};

void DisplayUIBoard(UIBoard& board) {
    for (Cell& cell : board.box) {
        cell.show();
    }
}

void ApplyMoveOnBoard(Board& board, Cell& cell) {
    board.board_buffer[cell.InVirtualBoardPosition.first][cell.InVirtualBoardPosition.second] = cell.Input;
}

bool checkValideMove(Board& board, Cell& cell) {
    if (board.board_buffer[cell.InVirtualBoardPosition.first][cell.InVirtualBoardPosition.second] == '0') return true;
    return false;
}

char SetPlayerTurn() {
    static int counter = -1;
    counter++;
    if (counter % 2 == 0) return 'X';
    return 'O';
}



void CleckBoxTOInput(UIBoard& board, Board& vBoard) {
    for (Cell& cell : board.box) {
        if (cell.CellIsClecked()) {
            if (cell.Input == '0') {
                PlayerTurn = SetPlayerTurn();
                if (PlayerTurn == 'X') cell.label = "X";
                else if (PlayerTurn == 'O') cell.label = "O";
                cell.Input = PlayerTurn;
            }

            if (checkValideMove(vBoard, cell)) {
                ApplyMoveOnBoard(vBoard, cell);
            }
        }

    }
}


void displayBoard(const Board& board) {
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            LOG(board.board_buffer[i][j]); LOG(' ');
        }
        LOG('\n');
    }
}






char checkWin(const Board& board) {
#define arr board.board_buffer

    //rows (-)
    for (int r = 0; r < 3; ++r) {
        if (arr[r][0] != '0') {
            if (arr[r][0] == arr[r][1] && arr[r][1] == arr[r][2]) return arr[r][0];
        }
    }

    // columns (|)
    for (int c = 0; c < 3; ++c)
        if (arr[0][c] != '0' && arr[0][c] == arr[1][c] && arr[1][c] == arr[2][c])
            return arr[0][c];

    //diagonals (\)
    if (arr[0][0] != '0' && arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2])
        return arr[0][0];

    //diagonals (/)
    if (arr[0][2] != '0' && arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0])
        return arr[0][2];

    return '0';
}


bool checkDraw(const Board& board) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (board.board_buffer[i][j] == '0') return false;
        }
    }
    return true;
}





int main()
{

    const int WIN_WIDTH = 500;
    const int WIN_HEIGHT = 480;



    Board board;
    UIBoard GraphicalBoard;


    //displayBoard(board);
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "tic tac toe");
    while (!WindowShouldClose()) {
        MousePos = GetMousePosition();
        BeginDrawing();
        if (checkWin(board) != '0') {
            DrawRectangle(0, 300, WIN_WIDTH, WIN_HEIGHT, BLACK);
            DrawText("YOU WiN!", 0, 300, 50, WHITE);
        }
        if (checkDraw(board)) {
            DrawRectangle(0, 300, WIN_WIDTH, WIN_HEIGHT, GRAY);
            DrawText("GAME IS ENDS in DRAW!", 0, 300, 50, WHITE);
        }
        ClearBackground(WHITE);
        DisplayUIBoard(GraphicalBoard);
        CleckBoxTOInput(GraphicalBoard, board);

        EndDrawing();
    }

    displayBoard(board);
    //CloseWindow();
}
