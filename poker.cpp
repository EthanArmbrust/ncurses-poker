#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <curses.h>
#include <locale.h>
#include <wchar.h>

using namespace std;


enum Suit{Spades, Hearts, Diamonds, Clubs};

#define JACK 9
#define QUEEN 10
#define KING 11
#define ACE 12

struct Card{
	int value;
	Suit suit;
};

struct Player{
	vector<Card> hand;
};

string print_suit_short(Suit s);
string print_suit(Suit s);
wchar_t print_suit_w(Suit s);
string print_value(int val);
vector<wstring> draw_card(Card c);

int main(){
	

	//create deck
	vector<Card> deck;
	for(int s = Spades; s <= Clubs; s++){
		for(int i = 0; i < 13; i++){
			Suit suit = static_cast<Suit>(s);
			Card c = {i, suit};
			deck.push_back(c);
		}
	}

	//shuffle deck
	auto rng = default_random_engine {};
	shuffle(deck.begin(), deck.end(), rng);

	//deal to 4 players
	vector<Player> players(4);

	for(Player &p : players){
		p.hand.push_back(deck.back());
		deck.pop_back();
		p.hand.push_back(deck.back());
		deck.pop_back();
	}

	for(int i = 1; i <=4; i++){
		for(Card &c : players[i-1].hand){
		}
	}

	//create screen
	setlocale(LC_ALL, "");
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int x,y;
	getmaxyx(stdscr, y, x);

	//test unicode
	wchar_t ws1[] = {L'\u2660', L'\u2663', L'\u2665', L'\u2666'};
	
	mvaddwstr(0,0,ws1);
	mvaddstr(1,0,to_string(x).c_str());
	mvaddstr(2,0,to_string(y).c_str());

	wstring top = L"\u250C\u2500\u2500\u2500\u2510";
	wstring mid2 = L"\u2502   \u2502";
	auto card_draw = draw_card(players[0].hand[0]);

	for(int i = 0; i < card_draw.size(); i++){
		mvaddwstr(5 + i, 0, card_draw[i].c_str());
	}	
	getch();	
	endwin();
	return 0;
}

string print_suit(Suit s){
	switch(s){
		case Spades:
			return "Spades";
		case Hearts:
			return "Hearts";
		case Diamonds:
			return "Diamonds";
		case Clubs:
			return "Clubs";
		default:
			return "DEFAULT";
	}
}

string print_suit_short(Suit s){
	switch(s){
		case Spades:
			return "S";
		case Hearts:
			return "H";
		case Diamonds:
			return "D";
		case Clubs:
			return "C";
		default:
			return "DEFAULT";
	}
}

wchar_t print_suit_w(Suit s){
	switch(s){
		case Spades:
			return L'\u2660';
		case Hearts:
			return L'\u2665';
		case Diamonds:
			return L'\u2666';
		case Clubs:
			return L'\u2663';
		default:
			return L'\u2660';
	}
}

string print_value(int val){
	switch(val){
		case JACK:
			return "J";
		case QUEEN:
			return "Q";
		case KING:
			return "K";
		case ACE:
			return "A";
		default:
			return to_string(val + 2);
	}
}
vector<wstring> draw_card(Card c){
	vector<wstring> output;

	wstring top = L"\u250C\u2500\u2500\u2500\u2510";
	wstring mid1 = L"\u2502   \u2502";
	wstring mid2 = mid1; 
	wstring mid3 = mid1; 
	wstring bottom = L"\u2514\u2500\u2500\u2500\u2518";
	mid1[1] = print_suit_w(c.suit);
	mid2[3] = print_suit_w(c.suit);
	mid1[3] = print_value(c.value)[0];
	mid2[1] = print_value(c.value)[0];
	output.push_back(top);
	output.push_back(mid1);
	output.push_back(mid2);
	//output.push_back(mid3);
	output.push_back(bottom);
	return output;
}

