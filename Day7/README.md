Day 7 - Bag Graph
----

This one was a step up from the previous challenges. We are given a set of rules which define which 'bags' fit inside which other bags, and in what number.

I finally see the value of C++'s ```std::multimap```! I use this to represent two DAGs which describe the 'contains' and 'contained in' relationships.

## Part 1

The task for Part 1 is, given my *shiny gold bag*, find all the possible colours of the outer bag. In other words, which bags can contain my shiny gold bag either directly or indirectly.

So I just have to work up through my DAG, adding nodes to a set which is my answer.

## Part 2

I had suspected that I'd need the opposite DAG ('contains'), and I was right. I have to work out how many bags in total fit inside my *shiny gold bag*.

I setup a priority queue of bags to check, which starts with 1 *shiny gold bag*, and I go through my 'contains' multimap, adding bag counts to my running total, and bag colours to the priority queue. Once the queue is empty, I'm done.



This was hard but fun!
