#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
/*
 File: FinalVerison.cpp
 Name: Hanbal Yusuf
 Created: 7/27/2024
 Purpose: Blackjack Game
 */


// Card structure
struct Card {
    int rank;  // 1-13 (1=Ace, 11=Jack, 12=Queen, 13=King)
    char suit; // 'H', 'D', 'C', 'S'
};

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

int main(int argc, char *argv[]) {
    // Initialize random seed
    srand(time(0));

    // Game counters
    int playerWins = 0;
    int dealerWins = 0;
    int totalGames = 0;

    // String representing the suits and ranks
    string suits = "HDCS";
    string ranks = "A23456789TJQK";

    int playerScore = 0; // Declare playerScore outside the loop
    // Play multiple rounds
    char playAgain;
    do {
        // Initialize player and dealer hands and scores
        Card playerHand1, playerHand2, dealerHand1, dealerHand2, newCard;
        playerScore = 0; // Reset playerScore for each round
        int dealerScore = 0;
        char choice;

        // Shuffle and assign initial cards to players
        playerHand1.rank = rand() % 13 + 1;
        playerHand1.suit = suits[rand() % 4];
        playerHand2.rank = rand() % 13 + 1;
        playerHand2.suit = suits[rand() % 4];
        dealerHand1.rank = rand() % 13 + 1;
        dealerHand1.suit = suits[rand() % 4];
        dealerHand2.rank = rand() % 13 + 1;
        dealerHand2.suit = suits[rand() % 4];

        // Calculate initial scores
        playerScore = getCardValue(playerHand1, 0) + getCardValue(playerHand2, 0);
        dealerScore = getCardValue(dealerHand1, 0) + getCardValue(dealerHand2, 0);

        // Display initial hands
        cout << "Player's hand: " << playerHand1.rank << playerHand1.suit << " " << playerHand2.rank << playerHand2.suit;
        cout << " (Score: " << fixed << setprecision(2) << playerScore << ")" << endl;
        cout << "Dealer's hand: " << dealerHand1.rank << dealerHand1.suit << " ??" << endl;

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
                newCard.suit = suits[rand() % 4];
                playerScore += getCardValue(newCard, playerScore);
                cout << "Player drew: " << newCard.rank << newCard.suit << " (Score: " << fixed << setprecision(2) << playerScore << ")" << endl;
            } else {
                break;
            }
        }

        // Dealer's turn
        cout << "Dealer's hand: " << dealerHand1.rank << dealerHand1.suit << " " << dealerHand2.rank << dealerHand2.suit;
        cout << " (Score: " << fixed << setprecision(2) << dealerScore << ")" << endl;

        while (dealerScore < 17) {
            // Draw a new card
            newCard.rank = rand() % 13 + 1;
            newCard.suit = suits[rand() % 4];
            dealerScore += getCardValue(newCard, dealerScore);
            cout << "Dealer drew: " << newCard.rank << newCard.suit << " (Score: " << fixed << setprecision(2) << dealerScore << ")" << endl;
11        }

        // Determine the winner
        ofstream outfile("blackjack_result.txt");
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
        outfile.close();

        totalGames++;

        // Ask if the player wants to play again
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } while (playAgain == 'y');

    // Calculate and display average wins
    float playerAvgWins = static_cast<float>(playerWins) / totalGames;
    float dealerAvgWins = static_cast<float>(dealerWins) / totalGames;

    cout << "\nTotal games played: " << totalGames << endl;
    cout << "Player wins: " << playerWins << " (Average wins: " << playerAvgWins << ")" << endl;
    cout << "Dealer wins: " << dealerWins << " (Average wins: " << dealerAvgWins << ")" << endl;

    // Example of cmath usage
    cout << "Absolute value of player's final score: " << abs(playerScore) << endl;

    return 0;
}
