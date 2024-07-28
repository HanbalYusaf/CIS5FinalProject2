#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
using namespace std;

/*
 File: Project2Final
 Name: Hanbal Yusuf
 Created: 7/27/2024
 Purpose: Blackjack Game
 */

// Card structure
struct Card {
    int rank;  // 1-13 (1=Ace, 11=Jack, 12=Queen, 13=King)
    char suit; // 'H', 'D', 'C', 'S'
};

// Function prototypes
int getCardValue(Card card, int currentScore);
void displayResults(int playerScore, int dealerScore, int &playerWins, int &dealerWins, ofstream &outfile);
void logGameDetails(ofstream &outfile, Card playerHand1, Card playerHand2, Card dealerHand1, Card dealerHand2, int playerScore, int dealerScore, int playerWins, int dealerWins);
void logFinalResults(ofstream &outfile, int playerWins, int dealerWins);
char getValidInput(string prompt);

const char SUITS[] = {'H', 'D', 'C', 'S'};
const char RANKS[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

int main() {
    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // Game counters
    int playerWins = 0;
    int dealerWins = 0;
    int totalGames = 0;

    // Variable to store final player score for example of cmath usage
    int finalPlayerScore = 0;

    // Open the file in write mode to overwrite previous content
    ofstream outfile("blackjack_result.txt"); // Open in write mode to overwrite file

    // Play multiple rounds
    char playAgain;
    do {
        // Initialize player and dealer hands and scores
        Card playerHand1, playerHand2, dealerHand1, dealerHand2, newCard;
        int playerScore = 0;
        int dealerScore = 0;
        char choice;

        // Shuffle and assign initial cards to players
        playerHand1.rank = rand() % 13 + 1;
        playerHand1.suit = SUITS[rand() % 4];
        playerHand2.rank = rand() % 13 + 1;
        playerHand2.suit = SUITS[rand() % 4];
        dealerHand1.rank = rand() % 13 + 1;
        dealerHand1.suit = SUITS[rand() % 4];
        dealerHand2.rank = rand() % 13 + 1;
        dealerHand2.suit = SUITS[rand() % 4];

        // Calculate initial scores
        playerScore = getCardValue(playerHand1, 0) + getCardValue(playerHand2, 0);
        dealerScore = getCardValue(dealerHand1, 0) + getCardValue(dealerHand2, 0);

        // Display initial hands
        cout << "Player's hand: " << RANKS[playerHand1.rank - 1] << playerHand1.suit << " " << RANKS[playerHand2.rank - 1] << playerHand2.suit;
        cout << " (Score: " << fixed << setprecision(2) << playerScore << ")" << endl;
        cout << "Dealer's hand: " << RANKS[dealerHand1.rank - 1] << dealerHand1.suit << " ??" << endl;

        // Player's turn
        while (playerScore < 21) {
            bool validInput = false;
            while (!validInput) {
                cout << "Do you want to hit (h) or stand (s)? ";
                cin >> choice;
                if (choice == 'h' || choice == 's') {
                    validInput = true;
                } else {
                    cout << "Invalid input. Please enter 'h' to hit or 's' to stand." << endl;
                }
            }

            if (choice == 'h') {
                // Draw a new card
                newCard.rank = rand() % 13 + 1;
                newCard.suit = SUITS[rand() % 4];
                playerScore += getCardValue(newCard, playerScore);
                cout << "Player drew: " << RANKS[newCard.rank - 1] << newCard.suit << " (Score: " << fixed << setprecision(2) << playerScore << ")" << endl;
            } else {
                break;
            }
        }

        // Dealer's turn
        cout << "Dealer's hand: " << RANKS[dealerHand1.rank - 1] << dealerHand1.suit << " " << RANKS[dealerHand2.rank - 1] << dealerHand2.suit;
        cout << " (Score: " << fixed << setprecision(2) << dealerScore << ")" << endl;

        while (dealerScore < 17) {
            // Draw a new card
            newCard.rank = rand() % 13 + 1;
            newCard.suit = SUITS[rand() % 4];
            dealerScore += getCardValue(newCard, dealerScore);
            cout << "Dealer drew: " << RANKS[newCard.rank - 1] << newCard.suit << " (Score: " << fixed << setprecision(2) << dealerScore << ")" << endl;
        }

        // Determine the winner and display results
        logGameDetails(outfile, playerHand1, playerHand2, dealerHand1, dealerHand2, playerScore, dealerScore, playerWins, dealerWins);
        displayResults(playerScore, dealerScore, playerWins, dealerWins, outfile);

        totalGames++;

        // Update final player score for example of cmath usage
        finalPlayerScore = playerScore;

        // Ask if the player wants to play again
        playAgain = getValidInput("Do you want to play again? (y/n): ");
    } while (playAgain == 'y');

    // Log the final results
    logFinalResults(outfile, playerWins, dealerWins);

    // Close the output file
    outfile.close();

    // Calculate and display average wins
    float playerAvgWins = static_cast<float>(playerWins) / totalGames;
    float dealerAvgWins = static_cast<float>(dealerWins) / totalGames;

    cout << "\nTotal games played: " << totalGames << endl;
    cout << "Player wins: " << playerWins << " (Average wins: " << playerAvgWins << ")" << endl;
    cout << "Dealer wins: " << dealerWins << " (Average wins: " << dealerAvgWins << ")" << endl;

    // Calculate and display sine of wins
    cout << "Player wins sin: " << sin(static_cast<float>(playerWins)) << endl;
    cout << "Dealer wins sin: " << sin(static_cast<float>(dealerWins)) << endl;

    // Example of cmath usage
    cout << "Absolute value of player's final score: " << abs(finalPlayerScore) << endl;

    return 0;
}

// Function to get the value of a card
int getCardValue(Card card, int currentScore) {
    if (card.rank > 10) {
        return 10;
    } else if (card.rank == 1) {
        return (currentScore + 11 > 21) ? 1 : 11; // Handle Ace as 1 or 11
    } else {
        return card.rank;
    }
}

// Function to display results and update counters
void displayResults(int playerScore, int dealerScore, int &playerWins, int &dealerWins, ofstream &outfile) {
    if (playerScore > 21) {
        cout << "Player busts! Dealer wins." << endl;
        outfile << "Player busts! Dealer wins." << endl;
        dealerWins++;
    } else if (dealerScore > 21) {
        cout << "Dealer busts! Player wins." << endl;
        outfile << "Dealer busts! Player wins." << endl;
        playerWins++;
    } else if (playerScore > dealerScore) {
        cout << "Player wins!" << endl;
        outfile << "Player wins!" << endl;
        playerWins++;
    } else if (dealerScore > playerScore) {
        cout << "Dealer wins!" << endl;
        outfile << "Dealer wins!" << endl;
        dealerWins++;
    } else {
        cout << "It's a tie!" << endl;
        outfile << "It's a tie!" << endl;
    }
}

// Function to log detailed game results
void logGameDetails(ofstream &outfile, Card playerHand1, Card playerHand2, Card dealerHand1, Card dealerHand2, int playerScore, int dealerScore, int playerWins, int dealerWins) {
    outfile << "Player's hand: " << RANKS[playerHand1.rank - 1] << playerHand1.suit << " " << RANKS[playerHand2.rank - 1] << playerHand2.suit;
    outfile << " (Score: " << fixed << setprecision(2) << playerScore << ")" << endl;
    outfile << "Dealer's hand: " << RANKS[dealerHand1.rank - 1] << dealerHand1.suit << " " << RANKS[dealerHand2.rank - 1] << dealerHand2.suit;
    outfile << " (Score: " << fixed << setprecision(2) << dealerScore << ")" << endl;
    outfile << "Player wins sin: " << sin(static_cast<float>(playerWins)) << endl;
    outfile << "Dealer wins sin: " << sin(static_cast<float>(dealerWins)) << endl;
    outfile << "Total player wins: " << playerWins << endl;
    outfile << "Total dealer wins: " << dealerWins << endl;
}

// Function to log final results
void logFinalResults(ofstream &outfile, int playerWins, int dealerWins) {
    if (playerWins > dealerWins) {
        outfile << "PLAYER HAS MORE WINS" << endl;
    } else if (dealerWins > playerWins) {
        outfile << "DEALER HAS MORE WINS" << endl;
    } else {
        outfile << "BOTH HAVE AN EQUAL AMOUNT OF WINS" << endl;
    }
    outfile << "Player wins sin: " << sin(static_cast<float>(playerWins)) << endl;
    outfile << "Dealer wins sin: " << sin(static_cast<float>(dealerWins)) << endl;
}
 
// Function to get valid input
char getValidInput(string prompt) {
    char input;
    bool valid = false;
    while (!valid) {
        cout << prompt;
        cin >> input;
        if (input == 'y' || input == 'n') {
            valid = true;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    }
    return input;
}
