* Day 22 - Crab War!

Simulating the card game 'War', though they call it 'Combat'. Each player flips a card, higher card takes both cards on their deck. Game ends when one player has no cards.

** Part 1 - A simple game of War

Just play against the crab & work out who wins. Pretty easy, but reminded me whhy this is such a rubbish game. No player input/decision is possible! The game is won once the cards are dealt.

** Part 2 - Recursive War

More complicated rules - each time a pair of cards is played, if both players have more cards than the value of the card, the play a sub-game with those cards. Fortunately it's with a *copy* of those cards, so the sub-game doesn't affect the main game cards, just the winner takes the cards.

Also need to prevent infinite games - if the exact deck configuration reappears, Player 1 wins. For this I used a (poor) hash function - which is the same function which scores the game.