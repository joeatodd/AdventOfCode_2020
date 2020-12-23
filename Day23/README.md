* Day 23 - Cup Shuffling

The crab shuffles 9 cups (numbered 1-9) according to rules:

Take the 3 cups after the current cup, move them to just after the cup which has value 1 less than the current cup. If this cup is in the 3, work downwards to find the new cup location.

** Part 1 - 10 cups, 100 shuffles

Easy, just simulate the list of cups directly, move the cups around.

** Part 2 - 1e6 cups, 1e7 shuffles (of course...)

This reminded me *so* much of the huge shuffling problem from AoC2019 that I spent quite a long time trying to work out how to arrive at the answer analytically rather than computing them. I also tried running the code for Part 1, but it's crazy slow.

I decided, working through it on paper, that it can't be worked out analytically, and has to be computed. I also came to realise that:

1. Only the relative, rather than the absolute, order matters (circle).
2. The relative order of only 4 cups changes each time.

So the efficient solution is rather than maintaining the list, is to maintain the list of the clockwise neighbour of each cup in a vector. Then only 3 changes need to be made each time.