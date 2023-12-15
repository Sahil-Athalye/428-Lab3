Sahil Athalye: a.sahil@wustl.edu
Yuheng Ding: d.yuheng@wustl.edu
Reedham Kalariya j.kalariya@wustl.edu

Lab 3 README

Design decisions for STEP 16: 

To make tracking the number of books made by each player easier and to prevent having to iterate through the CardSet of each player's books, 
we added a protected member variable book_nums. Additionally, to track the players that were out of the game we added a set of ints to track the indices of eliminated players.


Design decisions for STEP 18: 

For the collect_books method we needed to add a counter to the predicate that could be tracked across calls in order to make sure that only 4 cards
were being moved from a player's hand to their book. Doing so controlled for the edge case where a player's hand is initialized with more than 4 of a kind.


Design decisions for STEP 19: 

We decided to add an additional print statement at the start of a player's first turn in order to provide clarity in who must act. It also must be noted that
the rank values required to be inputted by the user for non-numerical ranks have been assigned their sequential numerical value. For example, to request a card of rank Ace the player must 
input 14 in digit form to qualify as a successful request this goes for King as 13, Queen as 12, and Jack as 11. For gameplay with an Uno Deck, the input values for non-numerical ranks are
indicated in the enum class below: 
    skip = 10,
    reverse = 11,
    drawtwo = 12,
    drawfour = 13,
    wild = 14,
    blank = 15,

Additionally, when testing we found an edge case that contradicts part of the instructions. The instructions specify, in terms of what ranks of cards can be requested: "provides a 
valid card rank (which must be the rank of a card in the player's hand)". Following this, if a player's hand has been emptied it is no longer able to request cards which does not make sense.
To alleviate this, we allow players who have emptied their hands to request any possible card in the deck. We also did not allow players to request cards from themselves.

Furthermore, if at any point a player receives a card from the deck or another player's hand books are collected from the requesting player's hand until no more books are left to be collected.
This is because if a player requests a card of certain rank, is denied that request and forced to GO FISH, the player can still receive a card from the deck that completes a 4 of a kind.

We also added print statements that indicate the status of requests made and actions taken in order to bring a better sense of awareness for game events.


Design decisions for STEP 20:

Included an if statement for when the number of players is less than the minimum amount of 2 even though Command Line Checking already takes care of this so that all control paths return a value.


Design decisions for STEP 21:

Rather than printing the round number at the end of the round we printed the round number at the start of a round as we felt this let users know which phase of the game they were at in a more
timely fashion. Utilizing the member variable "book_nums" greatly reduced the work needing to be done when printing the number of books achieved by each player. 

In contradiction to lab instructions, we decided that it would make the game more fun if one more round of requesting/stealing cards from opponents would be allowed to commence after the deck
was emptied as this would create higher scoring games and allow players to utilize their knowledge gained from counting cards throughout the game. This is also how some of our group members have
grown up playing the game and would feel heartbroken playing it any other way.

Lastly, we were a bit confused by what scope this statement was referencing, "print out how many books each player has made", as it could reference how many books each player made in the round
or overall. We decided to print out how many books have been made overall by each player as that would serve as a better reminder for the state of the game.

Design decisions for STEP 22:

We did not encapsulate the contruction of the GoFishGame with a try catch block because all inputs other than correct inputs are caught and handled before the constructor is able to be called



Errors Encountered:

1. When utilizing the collect_if() function within collect_books(), we noticed that the cards were not being taken out of the deck passed in as a parameter and were rather being copied into it
an obscene amount of times. At first we thought this was an issue with collect_if() but the fix turned out to require passing in the hand by reference so copy_if(), remove_if(), and erase() 
could all act on the desired hand.

2. When trying to control for the case where a player's hand is initialized with more than 4 of a kind, the compiler threw an error saying that we cannot modify a variable passed in by the
capture clause. This is because the capture clause passes variables in as const, adding the mutable keyword after the parameter list allowed us to increment the variable needed across calls
to the lambda function.

3. We encountered many warnings regarding unsigned/signed comparisons which were alleviated by casting signed variables as unsigned prior to comparison.

4. When trying to make_shared ptr with GoFishGame, the game did not fit the template as we had not yet set GoFishGame to inherit from Game. After setting Game as the base class for GoFishGame
and making the adequate changes to the rest of our functions we were then able to make_shared ptr with GoFishGame.

5. We failed to scope into std:: and our classes on many occasions, 
this was easily fixed by adding scope to defined variables

6. When trying to set the type for the predicate needing to be passed into collect_if() within the collect_books() function, the compiler would not accept <bool(Card<S, R>&)>, we were able to fix
this by letting the compiler decipher the type by using the auto keyword.

7. When switching the constructor to inherit Game, the compiler did not like our inline instantiations of member variables. This was fixed by moving their instantiations to the body
of the constructor.

These were the majority of the errors we ran into during the coding process, however we are very glad to have been able
to consult the textbook, studios, and professors for exact syntax on many occasions.


Trials Ran:

For testing GoFishGame we ran the game to completion many times and verified correct behavior for each run. We checked that the hands, books, and player names were printed neatly and in order
corresponding to how they were inputted. We verified that game statements such as round numbers, player book numbers, GO FISH, winners, and GAME OVER all printed at the right times and 
did so neatly. In many of the first trials, the formatting was slightly off and critical game statements were not printed, keeping some game state knowledge to be deciphered. Adding extra
lines of blank space and printing game statements improved the readability of the command line and kept the game easy to follow. 

When testing we kept a special eye out for when a player requested a card unsuccessfully, GO FISHED, received a card they didn't want but completed a 4 of a kind was immediately removed from 
their hand and placed in their books. In the first set of trials, the 4 of a kind was only moved into a player's books once it reached their turn again. This was remediated by 
calling collect_books() under any circumstance a player received a card. Another thing we kept a special eye out for was when a player's hand was initialized with 4 or more of a kind that upon 
the hand's very first print 4 of said card's kind was placed in their books. In the first set of trials, any 5 of a kind was moved into a player's books. This was remediated by saving a counter
variable across comparisons in the lambda function to track how many cards were to be moved.

During the many test runs we followed cards that were requested and succesfully stolen to make sure they moved to the requesting player's hand from the requested player's hand every time
without fail. The case where a player had zero cards and someone still requested a card from them was accounted for and passed succesfully; the requesting player GO FISHED and play continued
as normal. Additionally, in the case where a player had zero cards and it was their turn we made sure that the player could request any card in the deck, because in the first set of trials 
they were denied requesting any rank of card because there were no cards in their hand and the game got stuck in a denied request loop.

For testing Pinochle and HoldEm we played the games many many times and made sure actions went unchanged from Lab2. For more information see Lab2 README.

Overall, through brute force testing of GoFish, Pinochle, and HoldEm as well as specifically targeted tests for GoFish, we were able to verify correct behavior of our program for lab 3.



