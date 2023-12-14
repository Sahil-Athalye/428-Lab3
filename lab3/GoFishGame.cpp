// GoFishGame.cpp
// authors: Sahil Athalye
//          Yuheng Ding
//          Reedham Kalariya
// 

#define MIN_PLAYERS 2
#define MIN_RANKS 4
#define FOUR_OF_A_KIND 4
#define MULTIPLAYER_CARDNUM 5
#define CARDS_EACH_LINE 6
#define TWO_PLAYER_CARDNUM 7
#define CLI_ARGS 3

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

#include "GoFishGame.h"

template <typename S, typename R, typename D>
GoFishGame<S, R, D>::GoFishGame(int num_players, const char* player_names[]): Game(num_players, player_names){
    books = (std::vector<CardSet<R, S>>(num_players - CLI_ARGS));
    hands = (std::vector<CardSet<R, S>>(num_players - CLI_ARGS));
    std::cout << "template constructor"<< std::endl;
    // check if the deck has at least 4 of each rank
    for (R r = static_cast<R>(0); r < R::undefined; ++r) {
        unsigned num = count_if(deck.get_start(), deck.get_end(), [r](const Card<R, S> & card) {
            return card.rank == r;
        });
        if (num < MIN_RANKS) {
            throw std::runtime_error("GoFishGame constructor: deck needs to have at least 4 of each rank");
        }
    }

    for (int i = 0; i < num_players; i++) {
        this->player_names.push_back(std::string(player_names[i]));
    }
}

template <typename S, typename R, typename D>
GoFishGame<S, R, D>::~GoFishGame() {

}

template <typename S, typename R, typename D>
int GoFishGame<S, R, D>::play() {

    deal();

    auto canCollectBooks = true;
    for(unsigned int i = 0; i<hands.size();i++){
        canCollectBooks = true;
        while(canCollectBooks){
            canCollectBooks = collect_books(i+1);
        }
    }

    int round_num = 1;
    std::set<int> finished_players;
    while(finished_players.size() < hands.size()-1){
        

        for(unsigned int i = 0; i<hands.size();i++){
            if(finished_players.find(i) != finished_players.end()){
                continue;
            }

            bool successful_turn = turn(i+1);
            if(!successful_turn){
                finished_players.insert(i);
            }
        }

        if(deck.is_empty()){
            break;
        }

        std::cout<<"IT IS ROUND #"<<round_num<<std::endl;
        round_num++;
        //TODO STEP 21: need to print players' books and winning player

    }

    return 1;
    


}

template <typename S, typename R, typename D>
void GoFishGame<S, R, D>::deal() {
    deck.shuffle();
    int numPlayers = hands.size();
    if (numPlayers < MIN_PLAYERS) {
        return;
    }
    if (numPlayers == MIN_PLAYERS) {
        for (int i = 0; i < TWO_PLAYER_CARDNUM && !deck.is_empty(); i++) {
            deck >> hands[0];
            deck >> hands[1];
        }
    }
    else{
        for(int j = 0; j < MULTIPLAYER_CARDNUM; j++){
            for (int i = 0; i < numPlayers && !deck.is_empty(); i++) {
                deck >> hands[i];
            }
        } 
    }
}

template <typename S, typename R, typename D>
bool GoFishGame<S, R, D>::collect_books(int player_num){
    CardSet<R, S> player_hand = hands[player_num-1];
    std::vector<int> rank_counts(static_cast<int>(R::undefined));
    for(auto iter = player_hand.get_start();iter!=player_hand.get_end();iter++){
        Card<R,S> curr_card = *iter;
        R curr_rank = curr_card.rank;
        int idx = static_cast<int>(curr_rank);
        rank_counts[idx]++;
    }

    for(unsigned int i = 0; i<rank_counts.size();i++){
        if(rank_counts[i] >= FOUR_OF_A_KIND){
            auto predicate = [i](Card<R, S>& curr_card)->bool{
                R the_rank = static_cast<R>(i);
                if(the_rank == curr_card.rank){
                    std::cout<<"RANK MATCH"<<std::endl;
                    return true;
                }
                else{
                    std::cout<<"RANK MISMATCH"<<std::endl;
                    return false;
                }
            };
            // std::cout<<"BEFORE COLLECT_IF"<<std::endl;
            // books[player_num-1].print(std::cout,7);
            books[player_num-1].collect_if(player_hand, predicate);
            // std::cout<<"AFTER COLLECT_IF"<<std::endl;
            // books[player_num-1].print(std::cout,7);
            return true;
        }
    }

    return false;
}

template <typename S, typename R, typename D>
bool GoFishGame<S, R, D>::turn(int player_num) {
    std::cout<<"START OF PLAYER #"<<player_num<<"'s TURN"<<std::endl;

    int idx = player_num - 1;


    std::cout << "player " << player_num << " hand: " << std::endl;
    hands[idx].print(std::cout, CARDS_EACH_LINE);
    std::cout << "book: " << std::endl;
    books[idx].print(std::cout, CARDS_EACH_LINE);

    std::string rank_str;
    std::string from_player_str;

    int rank_num;
    int from_player_num;

    while(true){
        std::cout << "Ask for card of rank: ";
        std::cin >> rank_str;

        try{
            std::stoi(rank_str);
        }
        catch (std::invalid_argument const& ex){
            std::cout << "Please input rank in digit form"<<std::endl;
            continue;
        }
        

        rank_num = std::stoi(rank_str);
        
        
        R curr_rank = static_cast<R>(rank_num);
        
        typename std::vector< Card<R,S> >::iterator it = std::find_if(hands[idx].get_start(), hands[idx].get_end(), [curr_rank](Card<R,S> card){
        return card.rank == curr_rank;
        });

        if (it == hands[idx].get_end()){
            std::cout << "Please input rank currently in hand"<<std::endl;
            continue;
        }

        std::cout << "Ask from player: ";
        std::cin >> from_player_str;

        try{
            std::stoi(from_player_str);
        }
        catch (std::invalid_argument const& ex){
            std::cout << "Please input player number in digit form"<<std::endl;
            continue;
        }
        
        from_player_num = std::stoi(from_player_str);

        

        if(from_player_num==player_num){
            std::cout << "Cannot fish for card from yourself"<<std::endl;
            continue;
        }

        if(!(from_player_num >= 1 && from_player_num <= hands.size())){
            std::cout << "Please input valid player number between "<<1<<" and "<<hands.size()<<std::endl;
            continue;
        }

        break;
    }

    
    
    if (hands[idx].request(hands[from_player_num-1], static_cast<R>(rank_num))) {
        std::cout<<"RANK SUCCESSFULLY REQUESTED WAS "<<static_cast<R>(rank_num)<<std::endl;
        while (collect_books(player_num)) {}
        return true;
    } else {
        if (deck.is_empty()) {
            std::cout << "Player " << player_num << " is removed from the game" << std::endl;
            // TODO step 19: indicate player take no more moves;
            return false;
        } else {
            std::cout<<"GO FISH"<<std::endl;
            deck >> hands[idx];
            if ((*(--hands[idx].get_end())).rank == static_cast<R>(rank_num)) {
                return true;
            } else {
                return false;
            }
        }
    }


    

    return true;
}