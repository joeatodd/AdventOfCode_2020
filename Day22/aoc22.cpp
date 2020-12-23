#include <queue>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <set>

using std::endl;
using std::cout;

typedef std::queue<int> queue_t;
typedef std::pair<queue_t, queue_t> qpair_t;

bool verbose = false;

// Used to score the end of the game
// But also a convenient hash for comparing
// w/ previous deck configs
int deckScore(queue_t &deck){
  int score = 0, n = deck.size();

  for (int i = n; i > 0; i--) {
    int val = deck.front();
    score += i * deck.front();
    deck.pop();
    deck.push(val);
  }
  return score;
}

template <class Type>

// Print the deck (debugging)
void printQueue(std::queue<Type> inQ, std::string header = ""){

  if(!verbose) return;
  cout << header;
  int len = inQ.size();
  for(int i=0; i<len;i++){
    cout << inQ.front() << " ";
    inQ.pop();
  }
  cout << endl;
}

// Create a copy of part of the queue. My first template!
template <class Type>
std::queue<Type> copySubQueue(std::queue<Type> inQueue, int len){
  std::queue<Type> newQueue;
  for (int i=0; i<len; i++){
    newQueue.push(inQueue.front());
    inQueue.pop();
  }
  return newQueue;
}

// Play simple combat
void playCombat(queue_t p1q, queue_t p2q){
  queue_t *cq;

  while(!p1q.empty() && !p2q.empty()){

    // Get our cards
    int p1 = p1q.front();
    int p2 = p2q.front();
    p1q.pop(); p2q.pop();

    if(p1 > p2){
      p1q.push(p1);
      p1q.push(p2);
    }else{
      p2q.push(p2);
      p2q.push(p1);
    }

  }

  cq = (p1q.empty()) ? &p2q : &p1q;

  cout << "Part 1 answer: " << deckScore(*cq) << endl;
}

// Recursive Combat
bool playRCombat(queue_t p1q, queue_t p2q, bool root=false){

  std::set<std::pair<int,int>> prev_scores;

  // Play the game till someone wins
  int round = 0;
  while(!p1q.empty() && !p2q.empty()){

    printQueue(p1q, "Player 1 q ");
    printQueue(p2q, "Player 2 q ");

    int p1q_score = deckScore(p1q);
    int p2q_score = deckScore(p2q);

    // Try to insert scores into set, if failed then p1 wins
    auto ret = prev_scores.insert({p1q_score, p2q_score});
    if(!ret.second){
      if(root) cout << "Part 2 answer: " << deckScore(p1q)
    		    << endl;
      return true;
    }

    // Get our cards
    int p1 = p1q.front();
    int p2 = p2q.front();
    p1q.pop(); p2q.pop();

    assert(p1 > 0);
    assert(p2 > 0);

    bool p1WinSub;
    if(p1q.size() >= p1 && p2q.size() >= p2){

      // Copying sub-queue is kind of a pain
      queue_t p1sub = copySubQueue(p1q, p1);
      queue_t p2sub = copySubQueue(p2q, p2);
      printQueue(p1sub, "Player 1 sub ");
      printQueue(p2sub, "Player 2 sub ");

      p1WinSub = playRCombat(p1sub, p2sub);

    }else{

      p1WinSub = (p1>p2);

    }

    if(p1WinSub){
      p1q.push(p1);
      p1q.push(p2);
    }else{
      p2q.push(p2);
      p2q.push(p1);
    }

    round++;
  }

  queue_t *cq = p1q.empty() ? &p2q : &p1q;

  // Print who won
  if(root){
    cout << "Part 2 answer: " << deckScore(*cq) << endl;
  }

  return (p1q.empty()) ? false : true;
}


int main()
{
  std::ifstream infile("input.txt");
  std::string line;
  queue_t p1q_init, p2q_init, *cq;

  cq = &p1q_init;
  while(getline(infile, line)){
    if(line == "" || line == "Player 1:") continue;
    if(line == "Player 2:") {cq = &p2q_init; continue;}

    cq->push(stoi(line));
  }

  printQueue(p1q_init,"P1 init ");
  printQueue(p2q_init,"P2 init ");

  queue_t p1q = p1q_init;
  queue_t p2q = p2q_init;
  playCombat(p1q, p2q);

  p1q = p1q_init;
  p2q = p2q_init;
  playRCombat(p1q, p2q, true);

  return 0;
}
