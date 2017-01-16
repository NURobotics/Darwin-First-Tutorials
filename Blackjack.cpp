#include "../../Deck.1.4.h"
#include "../../std_lib_facilities.h"
void rules();
void decksum(Deck& deck, int& sum);
void deal(Deck& dealer, Deck& deck);
void reshuffle(Deck& dealer, Deck& discard);
void ace_check(Deck& deck, int& sum);
void bidding(Deck& player, int& playersum, int& bid, int& chips);
void results(int& losses, int& earnings);
void setup(Deck& dealer, Deck& computer, Deck& player, int& playersum, int& bid, int& chips, int& insure, int& computersum, Deck& discard, char& winner);
void doubling(Deck& player, Deck& dealer, Deck& computer, int& playersum, int& computersum, int& bid, char& winner);
void stay(Deck& player, Deck& computer, Deck& dealer, int& computersum, int& playersum, char& winner);
void hit(Deck& player, Deck& dealer, int& playersum, string& playername, char& winner);
bool winning(char& winner, int& chips, int& bid, int& insure, int& losses, int& earnings, int& playersum, int& computersum);
bool insurance(Deck& computer, int& bid, int& chips, int& insure);
bool splitting(Deck& player);
int main(){
	/*Win1 win(Point(0, 0),1200,800,"Blackjack");
	win.wait_for_button();
	win.screen1(); */
	srand(time(NULL));
	vector<Deck> players;		// Empty vector of decks for the player in case of split
	Deck dealer(true);			//Use a dealer deck to deal the cards to each of the players

	//Need 2 empty decks, one for the computer and one for the player. Also need a discard deck for the cards being discarded each round

	Deck computer;				// Empty deck for the computer
	Deck mainplayer;			// Empyt deck for the player
	Deck discard;				// Discard deck for the round
	int playersum = 0;			// Player's sum used to determine who wins
	int computersum = 0;		// Computer's sum used to determine who wins
	char winner;				// Variable to determine the winner
	int bid = 0;				// Player's bid each round
	int chips = 500;			// Default amount of money the player starts with
	int insure = 0;				// Insurance amount
	int earnings = 0;			// Overall earnings for the player to see at the end of the game.
	int losses = 0;				// Overall losses for the player to see at the end of the game.
	string playername;			// Ask for the player's name
	string choice;				// A string to keep the while loop running until the user enters quit
	string hitstay;				// A string to determine if player wants to hit or stay
	dealer.shuffle();			// Start out by shuffling the deck
	players.push_back(mainplayer); //Add the player deck to the vector of decks
	cout << "Welcome to Drew's Blackjack! See if you can beat him!\n\nWhat's your name?\n";
	cin >> playername;
	cout << "\nIf you need help with the rules, enter 'rules' or enter 'quit' to exit\nEnter a key to continue " << playername << endl;
	while (cin >> choice){
		playersum = 0;								// Reset the player's and computer's sums
		computersum = 0;
		if (choice == "quit" || choice == "q"){		// If the player enters quit exit the program
			results(losses, earnings);				// Show the player the results before they exit
			keep_window_open();
			return 0;
		}
		else if (choice == "rules" || choice == "r") {			// Opens the rules function to give an overview of the rules of Blackjack
			rules();
		}
		else {
			for (int i = 0; i < players.size(); ++i){
				setup(dealer, computer, players[i], playersum, bid, chips, insure, computersum, discard, winner); //Setup function to deal the player's and computer's starting hands
				if (playersum == 21) {
					winner = 'b';
				}
				else if (insurance(computer, bid, chips, insure) == true) {			//If the computer has a natural blackjack and the player bought insurance, set the winner equal to 'i'
					if (insure > 0) { 
						winner = 'i'; 
					}
					else winner = 'c';
				}
				else if (splitting(players[i]) == true){
					cout << "You have a split available.\n";
				}
				else {
					if ((bid * 2) > chips){											//Only the display the option for doubling down if the player has enough money to do so
						cout << "Do you want to 'hit' or 'stay'?\n";
					}
					else if ((bid * 2) <= chips){
						cout << "Do you want to 'hit', 'stay', or 'double'?\n";
					}
					while (cin >> hitstay) {
						if (hitstay == "hit" || hitstay == "h") {					//If player hits then deal more cards until player stays
							hit(players[i], dealer, playersum, playername, winner);
							if (playersum == 21) {									//Player gets blackjack, player wins
								winner = 'b';
								break;
							}
							else if (playersum > 21){
								winner = 'c';
								break;
							}
						}
						else if (hitstay == "stay" || hitstay == "s") {				//If the player stays run the stay function
							stay(players[i], computer, dealer, computersum, playersum, winner);
							break;
						}
						else if (hitstay == "double" && (bid * 2) <= chips || hitstay == "d" && (bid * 2) <= chips){
							doubling(players[i], dealer, computer, playersum, computersum, bid, winner);
							break;
						}
						else if (hitstay == "quit" || hitstay == "q") {				//If player enters 'quit' at any time the program will quit
							cout << "You may not quit in the middle of the round.\n";
						}
						else {
							cout << "That's not an option.\n";						//The program will not accept non-options
						}
						cout << "Do you want to 'hit' or 'stay'?\n";				//Ask if the player wants to hit or stay after all conditions
					}
				}
				winning(winner, chips, bid, insure, losses, earnings, playersum, computersum);
				cout << "Do you want to play again " << playername << "?\n\nEnter any key to continue " << playername << "...\nIf you need help with the rules, enter 'rules' or enter 'quit' to exit\n";
				while (players[i].size() > 0){
					int j = 1;														//Add the player's cards to the discard deck after each hand
					discard.add_card_to_deck(players[i].get_card(j));// , true);
					++j;
				}
				while (computer.size() > 0){
					int i = 1;														//Add the computer's cards to the discard deck after each hand
					discard.add_card_to_deck(computer.get_card(i));// , true);
					++i;
				}
			}
		}
	}
	return 0;
}
void decksum(Deck& deck, int& sum){
	sum = 0;
	Card tmp;
	for (int i = 0; i < deck.size(); ++i){
		tmp = deck.get_card(1);
		sum += tmp.value();
		deck.add_card_to_deck(tmp); //,true);
	}
}
void deal(Deck& dealer, Deck& deck){	//Function to deal the player cards
	Card tmp;														//Temporary card used to retrieve values
	tmp = dealer.get_top_card();									//Set the temporary card equal to the top card of the dealer's deck
	deck.add_card_to_deck(tmp);// , true);										//Add the new card to the deck
}
void reshuffle(Deck& dealer, Deck& discard){						//Function to reshuffle the discard deck into the dealer deck
	while (discard.size() > 0){										//Keep adding cards from the discard deck until the discard deck size is 0
		dealer.add_card_to_deck(discard.get_top_card()); //,true);
	}
	dealer.shuffle();												//Shuffle the dealer's deck again to produce new order of cards
}
void rules() {
	cout << "The goal of blackjack is to beat the dealer's hand without going over 21.\nFace cards are worth 10. Aces are worth 1 or 11, whichever makes a better hand.\nEach player starts with two cards, one of the dealer's cards is hidden until the end.\nTo 'Hit' is to ask for another card.To 'stay' is to hold your total and end your turn.\nIf you go over 21 you bust, and the dealer wins regardless of the dealer's hand.\nIf you are dealt 21 from the start(Ace & 10), you got a blackjack.\nBlackjack usually means you win 1.5 the amount of your bet.Depends on the casino.\nDealer will hit until his / her cards total 17 or higher.\nIf both the dealer and you get Blackjack or 21, then the game results in a push.\n'Doubling down' is when you’ve been dealt your first two cards in blackjack, you have an option of doubling your initial bet — but you get only one additional card.\n";
	cout << "Enter any character to play or 'quit' to exit.\n";
}
void ace_check(Deck& deck, int& sum){ //Function that checks the player's and computer's decks to make sure they do not bust due to aces
	decksum(deck, sum);
	Card tmp;													//Temp card to store the check the values of the player's and computer's cards
	for (int i = 0; i < deck.size(); ++i){						//For loop for the deck check based on size of the deck
		if (sum > 21){											//If the deck has an ace and their sum is over 21 due to it, subtract 10 from the deck's sum
			tmp = deck.get_card(1);
			if (tmp.name() == "Ace"){
				sum -= 10;
			}
			deck.add_card_to_deck(tmp); //,true);
		}
	}
}
void bidding(Deck& player, int& playersum, int& bid, int& chips){
	if (chips == 0){																											//If the player is out of chips provide an extra 100 to keep the game going
		cout << "\nLooks like you're out of money, on the house here's $100 so you can try and win your money back.\n\n";
		chips = 100;
	}
	cout << "How much do you want to bid, $5 minimum, you have: $" << chips << endl;													//Ask how much the player wants to bid
	cin >> bid;
	while (!cin){
		cout << "Please enter numbers only.\n";
		cin.clear();
		cin.ignore();
		cin >> bid;
	}
	while (bid > chips || bid < 5){																									//Check to make sure the player has that amount to bid
		if (bid < 0){
			cout << "You can't bid a negative amount.\n";
		}
		else {
			cout << "You don't have that amount to bid, please make a bet of at least 5.\n";
		}
		cin >> bid;																											//Reread the bid if the player doesn't have that amount
	}
	cout << "You bid: $" << bid << " on this round.\n";																		//Reread the bid and amount remaining to the player so they are aware of where they stand with money
	cout << "You have: $" << chips - bid << " remaining.\n\n";
	cout << "Press enter to continue...\n";
	cin.ignore(); cin.ignore();
	cout << string(20, '\n');
}
void results(int& losses, int& earnings){																						//Function to calculate the results of the rounds played
	if (losses > earnings){																										//If the losses outweighed the earnings then show how much the player lost
		cout << "You ended up losing $" << losses - earnings << " in the long run but better luck next time!\n";
	}
	else if (earnings > losses){																								//If the player won more than they lost, show them the amount they won
		cout << "You made $" << earnings - losses << " overall, great job!\n";
	}
	else cout << "You broke even, not bad but not good either.\n";
}
void setup(Deck& dealer, Deck& computer, Deck& player, int& playersum, int& bid, int& chips, int& insure, int& computersum, Deck& discard, char& winner){
	bidding(player, playersum, bid, chips);					// Set the player's bid for the round
	if (dealer.size() <= 10){								// If there aren't enough cards to finish the round, reshuffle the deck
		reshuffle(dealer, discard);
	}
	for (int i = 0; i < 2; ++i){
		deal(dealer, player);								// Deal the player's first and second cards
		deal(dealer, computer);								// Deal the computer's first and second cards
	}
	ace_check(player, playersum);
	cout << "You have:\n";
	player.print_deck();									//Show the player his cards
	cout << "Your sum is: " << playersum << endl << endl;
	cout << "Drew has: \n1. ";
	computer.print_card(1);									//Reveal only the computer's first card
	cout << "2. And a mystery card\n";
	//splitting(player);
}
void doubling(Deck& player, Deck& dealer, Deck& computer, int& playersum, int& computersum, int& bid, char& winner){
	bid *= 2;
	deal(dealer, player);
	if (playersum > 21){
		cout << "\n\n\nYou have: \n";
		ace_check(player, playersum);						//Check for aces to avoid bust
		player.print_deck();								//If player stays then show player deck and computer deck
		cout << "Your sum is: " << playersum << endl << endl;
		winner = 'c';
	}
	else {
		stay(player, computer, dealer, computersum, playersum, winner);
	}
}
void stay(Deck& player, Deck& computer, Deck& dealer, int& computersum, int& playersum, char& winner){
	while (computersum < 17) {
		deal(dealer, computer);								//If the computer's sum is less than 17, keep dealing cards until sum is 17 or more
		ace_check(computer, computersum);					//Have the computer check each time it's dealt for aces
	}
	cout << "\n\n\nYou have: \n";
	ace_check(player, playersum);							//Check for aces to avoid bust
	player.print_deck();									//If player stays then show player deck and computer deck
	cout << "Your sum is: " << playersum << endl << endl;
	cout << "And Drew has: \n";
	computer.print_deck();									//Also show both sums
	cout << "Drew's sum is: " << computersum << endl;
	if (computersum > 21) {									//If the computer sum is over 21, player wins
		winner = 'p';
	}
	else if (computersum == 21) {							//If the computer has blackjack, computer wins
		winner = 'c';
	}
	else if (playersum > 21){
		winner = 'c';
	}
	else if (computersum == 21 && playersum == 21){
		winner = 'n';
	}
	else {
		if (playersum > computersum) {						//Player has a sum closer to 21, player wins
			winner = 'p';
		}
		else if (playersum < computersum) {					//Computer has a sum closer to 21, computer wins
			winner = 'c';
		}
		else if (playersum == computersum) {				//If sums are equal, round results in push
			winner = 'n';
		}
	}
}
void hit(Deck& player, Deck& dealer, int& playersum, string& playername, char& winner){
	deal(dealer, player);
	ace_check(player, playersum);							// Check for aces each hit to avoid bust
	cout << "\n\n\nNow you have: \n";
	player.print_deck();									//Show the player his/her deck again and new sum
	cout << "Your sum is: " << playersum << endl;
}
bool winning(char& winner, int& chips, int& bid, int& insure, int& losses, int& earnings, int& playersum, int& computersum){
	switch (winner){												//Switch statement to determine the winner and money
	case 'c':														//Case that the computer wins
		if (playersum > 21){
			cout << "Over 21 bust, you lose this time.\n";
		}
		else if (computersum == 21) {								//If the computer has blackjack, computer wins
			cout << "Drew has Blackjack, he wins\n";
		}
		else cout << "Drew wins!\n";
		chips -= bid;												//Subtract the bid from the chips the player has
		losses += bid;												//Add the amount lost to the losses variable
		break;
	case 'p':														//Case that the player wins
		if (computersum > 21) {										//If the computer sum is over 21, player wins
			cout << "You win! Drew's over 21\n";
		}
		else cout << "You win!\n";
		chips += bid;												//Add the bid to the player's chips
		earnings += bid;											//Add the bid amount to the earnings variable
		break;
	case 'b':														//Case that the player gets blackjack
		cout << "BlackJack! You win!\n";
		chips += (bid * 1.5);										//Add 1.5 times the bid to the player's chips
		earnings += (bid * 1.5);									//Add 1.5 times the bid to the player's overall earnings variable
		break;
	case 'n':														//Case that there is a push and nothing happens
		cout << "Push\n";
		chips = chips;												//Make the chips equal to themselves so that program runs without breaking
		break;
	case 'i':														//Case that insurance pays off
		cout << "Drew has BlackJack so your insurance paid off\n";
		chips -= bid;												//If insurance pays off then player loses bid
		chips -= insure;											//Player will also lose the amount put in to pay for insurance
		chips += insure * 2;										//Player will receive twice the amount of the insurance back
		break;
	default:
		chips = chips;												//Default case that keeps the program from breaking
		break;
	}
	return chips;
}
//Insurance rules - https://www.vegas.com/gaming/gaming-tips/blackjack/
bool insurance(Deck& computer, int& bid, int& chips, int& insure){	//Function to suggest insurance if an Ace is showing as the computer's first card
	string choice;
	Card tmp1;														//Temporary card to check the name of the first card
	Card tmp2;														//Temporary card to check the value of the second card
	tmp1 = computer.get_card(1);
	if (tmp1.value() == 11){
		cout << "Do you want to buy insurance for: " << bid / 2 << endl << "'Yes' or 'no'?\n";
		while (cin >> choice){
			if (choice == "Yes" && ((chips - bid) >= (bid / 2)) || choice == "yes" && ((chips - bid) >= (bid / 2))) {					//If the player buys insurance then set the value of insurance and determine if the computer has natural blackjack
				insure = bid / 2;										//Set the insurance cost equal to half of the bid
				tmp2 = computer.get_card(1);							//Set the second tmp card equal to the next card
				if (tmp2.value() == 10) {								//If the computer's second card has a value of 10
					computer.add_card_to_deck(tmp1); //,true);
					computer.add_card_to_deck(tmp2); //,true);
					computer.print_deck();
					return true;										//Return the function as true to show that the player bought insurance
				}
				else {													//If the player pays for insurance but the computer doesn't have natural blackajack, subtract the insurance
					cout << "Drew didn't have a natural blackjack. You lose your $"<<insure<<" insurance bid.\n";
					chips -= insure;
					computer.add_card_to_deck(tmp1); //,true);
					computer.add_card_to_deck(tmp2); //,true);
					return false;										//Return as false to show that the computer doesn't have blackjack
				}
				computer.add_card_to_deck(tmp2); //,true);
				break;
			}
			else if (choice == "No" || choice == "no") {				//If the player chooses not to buy insurance play the hand like normal
				tmp2 = computer.get_card(1);							//Set the second tmp card equal to the next card
				if (tmp2.value() == 10) {								//If the second card has a value of 10 then the computer has natural blackjack
					computer.add_card_to_deck(tmp1); //,true);
					computer.add_card_to_deck(tmp2); //,true);				//Add the second card back to the deck
					return true;										//Return the function false to show that the play did not buy insurance
				}
				else {
					cout << "Drew didn't have a natural blackjack.\n";
					computer.add_card_to_deck(tmp1); //,true);
					computer.add_card_to_deck(tmp2); //,true);
					return false;
				}
					computer.add_card_to_deck(tmp2); //,true);					//Add the second card back to the deck
					break;												//Break the while loop
			}
			else if ((chips - bid) <= (bid / 2)) {						//If the player doesn't have enough money to buy insurance automatically break the loop
				cout << "You don't have enough money left to purchase insurance.\n";
				break;
			}
			else cout << "That is not an option.\n";					//If anything else is entered
		}
	}
	computer.add_card_to_deck(tmp1); //,true);
}
bool splitting(Deck& player){ //Keep a constant for one of the cards like tmp[j=0] and then run through the loop of cards, if no matches put the first card back in the player's deck
	//Need a vector to handle splitting
	Vector<Card> tmp;
	string choice;
	while (tmp.size() > 0) {
		if (tmp[0].value() == tmp[1].value()) {
			cout << "Do you want to split your " << tmp[0].name() << "'s? Yes or no?\n";
			cin >> choice;
			if (choice == "Yes" || choice == "yes") {
				while (tmp.size() > 0) {
					player.add_card_to_deck(tmp[0]);
				}
				return true;
			}
			else if (choice == "No" || choice == "no") {
				while (tmp.size() > 0) {
					player.add_card_to_deck(tmp[0]);
				}
				return false;
			}
			else cout << "That's not an option.\n";
		}
		player.add_card_to_deck(tmp[0]);
	}
	cout << "No splits available.\n";
	return false;
}
//True or false after add card to deck means true - spread out the cards for the player to see or false - stack them up in a single pile
//Page 439 in programming book for help