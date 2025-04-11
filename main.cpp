#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */
#endif

using namespace std;

const int SIZE = 4;
int board[SIZE][SIZE];
int score = 0;          // Current score
vector<int> highScores; // Historical high scores

// Function declarations
void initializeBoard();
void generateNewNumber();
void displayBoard();
char processInput();
bool isWin();
bool isGameOver();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
void saveScore();
void displayMenu();
void processMenuInput();
void playGame();
void viewHighScores();

// Initialize the game board
void initializeBoard() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      board[i][j] = 0;
    }
  }
}

// Generate a new number (2 or 4) at a random empty position
void generateNewNumber() {
  int emptyCount = 0;
  int emptyIndices[SIZE * SIZE][2];

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 0) {
        emptyIndices[emptyCount][0] = i;
        emptyIndices[emptyCount][1] = j;
        emptyCount++;
      }
    }
  }

  if (emptyCount > 0) {
    int index = rand() % emptyCount;
    int x = emptyIndices[index][0];
    int y = emptyIndices[index][1];
    board[x][y] = (rand() % 2 + 1) * 2;
  }
}

// Display the game board and current score
void displayBoard() {
  system("clear");

  cout << "=================================" << endl;
  cout << "            2048 Game            " << endl;
  cout << "=================================" << endl;

  for (int i = 0; i < SIZE; i++) {
    cout << "+-------+-------+-------+-------+" << endl;
    for (int j = 0; j < SIZE; j++) {
      cout << "|";
      if (board[i][j] != 0) {
        int num = board[i][j];
        string color;
        switch (num) {
        case 2:
          color = RED;
          break;
        case 4:
          color = GREEN;
          break;
        case 8:
          color = YELLOW;
          break;
        case 16:
          color = BLUE;
          break;
        case 32:
          color = MAGENTA;
          break;
        case 64:
          color = CYAN;
          break;
        default:
          color = WHITE;
          break;
        }
        cout << color << "   " << num << "   " << RESET;
      } else {
        cout << "       ";
      }
    }
    cout << "|" << endl;
  }
  cout << "+-------+-------+-------+-------+" << endl;

  cout << "=================================" << endl;
  cout << "   Score: " << score << endl;
  cout << "   High Scores:" << endl;
  for (int i = 0; i < highScores.size(); i++) {
    cout << "   - " << highScores[i] << endl;
  }
  cout << "   Use W/A/S/D to move tiles     " << endl;
  cout << "        Press Q to quit          " << endl;
  cout << "=================================" << endl;
}

// Process user input
char processInput() {
  char input;
#ifdef _WIN32
  input = _getch();
#else
  system("stty raw");
  input = getchar();
  system("stty cooked");
#endif
  return input;
}

// Check if the player has won
bool isWin() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 2048) {
        return true;
      }
    }
  }
  return false;
}

// Check if the game is over
bool isGameOver() {
  // Check if there are still empty positions
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 0) {
        return false;
      }
    }
  }

  // Check if there are adjacent tiles with the same number
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if ((i < SIZE - 1 && board[i][j] == board[i + 1][j]) ||
          (j < SIZE - 1 && board[i][j] == board[i][j + 1])) {
        return false;
      }
    }
  }

  return true;
}

// Move tiles up
void moveUp() {
  for (int j = 0; j < SIZE; j++) {
    for (int i = 1; i < SIZE; i++) {
      if (board[i][j] != 0) {
        int k = i;
        while (k > 0 && board[k - 1][j] == 0) {
          board[k - 1][j] = board[k][j];
          board[k][j] = 0;
          k--;
        }
        if (k > 0 && board[k - 1][j] == board[k][j]) {
          board[k - 1][j] *= 2;
          score += board[k - 1][j];
          board[k][j] = 0;
        }
      }
    }
  }
}

// Move tiles down
void moveDown() {
  for (int j = 0; j < SIZE; j++) {
    for (int i = SIZE - 2; i >= 0; i--) {
      if (board[i][j] != 0) {
        int k = i;
        while (k < SIZE - 1 && board[k + 1][j] == 0) {
          board[k + 1][j] = board[k][j];
          board[k][j] = 0;
          k++;
        }
        if (k < SIZE - 1 && board[k + 1][j] == board[k][j]) {
          board[k + 1][j] *= 2;
          score += board[k + 1][j];
          board[k][j] = 0;
        }
      }
    }
  }
}

// Move tiles left
void moveLeft() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 1; j < SIZE; j++) {
      if (board[i][j] != 0) {
        int k = j;
        while (k > 0 && board[i][k - 1] == 0) {
          board[i][k - 1] = board[i][k];
          board[i][k] = 0;
          k--;
        }
        if (k > 0 && board[i][k - 1] == board[i][k]) {
          board[i][k - 1] *= 2;
          score += board[i][k - 1];
          board[i][k] = 0;
        }
      }
    }
  }
}

// Move tiles right
void moveRight() {
  for (int i = 0; i < SIZE; i++) {
    for (int j = SIZE - 2; j >= 0; j--) {
      if (board[i][j] != 0) {
        int k = j;
        while (k < SIZE - 1 && board[i][k + 1] == 0) {
          board[i][k + 1] = board[i][k];
          board[i][k] = 0;
          k++;
        }
        if (k < SIZE - 1 && board[i][k + 1] == board[i][k]) {
          board[i][k + 1] *= 2;
          score += board[i][k + 1];
          board[i][k] = 0;
        }
      }
    }
  }
}

// Save the current score to the high scores and update the leaderboard
void saveScore() {
  highScores.push_back(score);
  sort(highScores.begin(), highScores.end(), greater<int>());
}

// Display the main menu
void displayMenu() {
  system("clear");

  cout << "=================================" << endl;
  cout << "          2048 Game Menu         " << endl;
  cout << "=================================" << endl;
  cout << "   1. Start Game" << endl;
  cout << "   2. View High Scores" << endl;
  cout << "   3. Quit" << endl;
  cout << "=================================" << endl;
  cout << "   Enter your choice: ";
}

// Process the main menu input
void processMenuInput() {
  char input;
#ifdef _WIN32
  input = _getch();
#else
  system("stty raw");
  input = getchar();
  system("stty cooked");
#endif

  switch (input) {
  case '1':
    playGame();
    break;
  case '2':
    viewHighScores();
    break;
  case '3':
    saveScore();
    exit(0);
  default:
    break;
  }
}

// Run the game
void playGame() {
  score = 0;
  initializeBoard();
  generateNewNumber();
  generateNewNumber();
  displayBoard();

  while (true) {
    char input = processInput();
    if (input == '0') {
      saveScore();
      break;
    }

    bool moved = false;

    switch (input) {
    case 'W':
    case 'w':
      moveUp();
      moved = true;
      break;
    case 'S':
    case 's':
      moveDown();
      moved = true;
      break;
    case 'A':
    case 'a':
      moveLeft();
      moved = true;
      break;
    case 'D':
    case 'd':
      moveRight();
      moved = true;
      break;
    }

    if (moved) {
      generateNewNumber();
      displayBoard();

      if (isWin()) {
        cout << "Congratulations! You win!" << endl;
        saveScore();
        break;
      }

      if (isGameOver()) {
        cout << "Game over! You lose!" << endl;
        saveScore();
        break;
      }
    }
  }
}

// View the high scores
void viewHighScores() {
  system("clear");

  cout << "=================================" << endl;
  cout << "          High Scores            " << endl;
  cout << "=================================" << endl;
  for (int i = 0; i < highScores.size(); i++) {
    cout << "   " << i + 1 << ". " << highScores[i] << endl;
  }
  cout << "=================================" << endl;
  cout << "   Press any key to continue...  " << endl;

  processInput();
}

int main() {
  srand(time(0));

  while (true) {
    displayMenu();
    processMenuInput();
  }

  return 0;
}
