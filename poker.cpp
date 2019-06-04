#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#ifdef __APPLE__
#include <curses.h>
#else
#include <ncursesw/curses.h>
#endif
#include <locale.h>
#include <wchar.h>
#include <cstdlib>
#include <ctime>

using namespace std;


enum Suit{Spades, Hearts, Diamonds, Clubs};

#define TEN 8
#define JACK 9
#define QUEEN 10
#define KING 11
#define ACE 12

#define CARD_WIDTH 5
#define CARD_HEIGHT 4

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

bool check_pair(vector<Card> &cards);

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
	random_device rd;
	mt19937 gen(rd());
	shuffle(deck.begin(), deck.end(), gen);

	//deal to 4 players
	vector<Player> players(4);
	vector<string> playernames;
    vector<Card> table;

	for(int i = 0; i < 4; i++){
		playernames.push_back("Player " + to_string(i+1));
	}

	for(Player &p : players){
		p.hand.push_back(deck.back());
		deck.pop_back();
		p.hand.push_back(deck.back());
		deck.pop_back();
	}

    for(int i = 0; i < 5; i++){
        table.push_back(deck.back());
        deck.pop_back();
    }


	//create screen
	setlocale(LC_ALL, "");
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

    //get terminal dimensions
	int x,y;
	getmaxyx(stdscr, y, x);

    /*
     * Player 0: Bottom
     * Player 1: Left
     * Player 2: Top
     * Player 3: Right
     */

    vector<pair<int,int>> playerpos;
    playerpos.push_back({y * 0.7, (x * 0.5) - CARD_WIDTH});
    playerpos.push_back({(y * 0.5) - (CARD_HEIGHT / 2), (x * 0.2) - (CARD_WIDTH)});
    playerpos.push_back({(y * 0.3) - CARD_HEIGHT, (x * 0.5) - CARD_WIDTH});
    playerpos.push_back({(y * 0.5) - (CARD_HEIGHT / 2), (x * 0.8) - CARD_WIDTH});

	vector<pair<int,int>> nameplates;
	for(int i = 0; i < playerpos.size(); i++){
		nameplates.push_back({playerpos[i].first + CARD_HEIGHT, playerpos[i].second});
	}


    for(unsigned j = 0; j < playerpos.size(); j++){
        auto card_draw = draw_card(players[j].hand[0]);
        auto card2_draw = draw_card(players[j].hand[1]);
		string player_name = playernames[j];

        for(unsigned i = 0; i < card_draw.size(); i++){
            mvaddwstr(playerpos[j].first + i, playerpos[j].second, card_draw[i].c_str());
        }

        for(unsigned i = 0; i < card2_draw.size(); i++){
            mvaddwstr(playerpos[j].first + i, playerpos[j].second + CARD_WIDTH, card2_draw[i].c_str());
        }
		int half = playerpos[j].second + CARD_WIDTH;
		int centered = half - (player_name.length() / 2);
		mvaddstr(nameplates[j].first, centered, player_name.c_str());
    };

    int table_y = (y * 0.5) - (CARD_HEIGHT / 2);
    int table_x = (x * 0.5) - (CARD_WIDTH * 2.5);

    for(unsigned i = 0; i < table.size(); i++){
        auto card_img = draw_card(table[i]);
        
        for(unsigned j = 0; j < card_img.size(); j++){
            mvaddwstr(table_y + j, table_x + (i * CARD_WIDTH), card_img[j].c_str());
        }
    }

	string player1_test = "Player 1 has pair? ";
	vector<Card> complete;
	complete.reserve(table.size() + players[0].hand.size());
	complete.insert(complete.end(), table.begin(), table.end());
	complete.insert(complete.end(), players[0].hand.begin(), players[0].hand.end());

	if(check_pair(complete)){
		player1_test += "Yes";
	}
	else{
		player1_test += "No";
	}

	mvaddstr(y - 1, 0, player1_test.c_str());

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
        case TEN:
            return "T";
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
	wstring bottom = L"\u2514\u2500\u2500\u2500\u2518";
	mid1[1] = print_suit_w(c.suit);
	mid2[3] = print_suit_w(c.suit);
	mid1[3] = print_value(c.value)[0];
	mid2[1] = print_value(c.value)[0];
	output.push_back(top);
	output.push_back(mid1);
	output.push_back(mid2);
	output.push_back(bottom);
	return output;
}

bool check_pair(vector<Card> &cards){
	for(int i = 0; i < cards.size(); i++){
		Card &c = cards[i];
		for(int j = i + 1; j < cards.size(); j++){
			Card &d = cards[j];
			if(c.value == d.value){
				return true;
			}
		}
	}
	return false;
}
