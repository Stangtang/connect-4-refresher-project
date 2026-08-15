#include <iostream>

#define ROWS 6
#define COLS 7

#define ANSI_RED ("\033[91m")
#define ANSI_YELLOW ("\033[93m")
#define ANSI_RESET ("\033[0m")

enum class Color {
    None, // 0
    Red, // 1
    Yellow // 2
};

void print_board(const Color (&board)[ROWS][COLS]) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            switch (cell) {
            case Color::None:
                std::cout << "- ";
                continue;
            case Color::Red:
                std::cout << ANSI_RED;
                break;
            case Color::Yellow:
                std::cout << ANSI_YELLOW;
                break;
            }
            std::cout << "O " << ANSI_RESET;
        }
        std::cout << '\n';
    }

    for (unsigned int i = 1; i <= COLS; i++) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

bool verify_input(const unsigned int& input, const Color(&board)[ROWS][COLS]) {
    if (input < 1 || input > COLS) {
        std::cout << "Value out of bounds. Try again.\n";
        return false;
    }

    if (board[0][input - 1] != Color::None) {
        std::cout << "Column full. Try again.\n";
        return false;
    }

    return true;
}

unsigned int get_input(const Color& curr_player, const Color(&board)[ROWS][COLS]) {
    std::cout << "CURRENT PLAYER: ";
    switch (curr_player) {
    case Color::Red:
        std::cout << ANSI_RED << "RED";
        break;
    case Color::Yellow:
        std::cout << ANSI_YELLOW << "YELLOW";
        break;
    }
    std::cout << '\n' << ANSI_RESET;

    unsigned int input;
    bool is_input_valid = false;
    do {
        std::cout << "Input Column 1-" << COLS << ": ";
        std::cin >> input;
        is_input_valid = verify_input(input, board);
    } while (!is_input_valid);

    return input;
}

void update_board(const unsigned int& input, const Color& curr_player, Color (&board)[ROWS][COLS], unsigned int& updated_row) {
    for (unsigned int row = ROWS - 1; row >= 0; row--) {
        if (board[row][input - 1] != Color::None) {
            continue;
        }
        board[row][input - 1] = curr_player;
        updated_row = row;
        return;
    }
}

void switch_player(Color& player) {
    player = (player == Color::Red) ? Color::Yellow : Color::Red;
}

bool has_won(const Color& player, Color(&board)[ROWS][COLS], const unsigned int& last_move_row, const unsigned int& last_move_col) {
    const int directions[4][2] = {
        {0, 1},   // horizontal right
        {1, 0},   // vertical down
        {1, 1},   // diagonal down-right
        {1, -1}   // diagonal down-left
    };
    for (const auto& direction : directions) {
        int delta_row = direction[0];
        int delta_col = direction[1];

        unsigned int consecutive_pieces = 1;

        int row = last_move_row + delta_row;
        int col = last_move_col + delta_col;
        while (row < ROWS && col < COLS && board[row][col] == player) {
            consecutive_pieces++;
            row += delta_row;
            col += delta_col;
        }

        row = last_move_row - delta_row;
        col = last_move_col - delta_col;
        while (row >= 0 && col >= 0 && board[row][col] == player) {
            consecutive_pieces++;
            row -= delta_row;
            col -= delta_col;
        }

        if (consecutive_pieces >= 4) {
            return true;
        }
    }

    return false;
}

#ifdef _WIN32
#include <conio.h>
void wait_until_key_pressed() {
    _getch();
}
#else
#include <termios.h>
#include <unistd.h>

void wait_until_key_pressed() {
    termios oldt{}, newt{};

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
#endif

int main()
{
    Color board[ROWS][COLS] = {};
    Color curr_player = Color::Yellow;

    unsigned int max_turns = ROWS * COLS;
    while (max_turns--) {
        switch_player(curr_player);

        print_board(board);
        std::cout << '\n';

        unsigned int input = get_input(curr_player, board);
        std::cout << '\n';

        unsigned int input_row;
        unsigned int input_col = input - 1;
        update_board(input, curr_player, board, input_row);
        if (has_won(curr_player, board, input_row, input_col)) {
            print_board(board);
            std::cout << '\n';
            switch (curr_player) {
            case Color::Red:
                std::cout << ANSI_RED << "RED";
                break;
            case Color::Yellow:
                std::cout << ANSI_YELLOW << "YELLOW";
                break;
            }
            std::cout << ANSI_RESET << " WON!\nPress any key to exit.";
            wait_until_key_pressed();
            return 0;
        }
    }

    std::cout << "NOONE WON!\nPress any key to exit.";
    wait_until_key_pressed();
    return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
