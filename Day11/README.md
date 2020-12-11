Day 11 - Bugs again (but seats!)
----

Game of life style problem - compute the empty & occupied seats, based on rules from the previous timestep, and find the equilibrium point.

The rules were fairly straightforward for part 1 - the adjacent 8 seats. This was fine, though my code took a surprisingly long time to run (~15 secs) till I realised that I needed to be *passing by reference*! This cut the run time way down.

Part 2 was a little tougher - nearest *visible* seat. In other words, for each of the 8 directions, keep searching until an occupied or empty seat is found. This was a little tougher, but fortunately I'd already written my part 1 with a 'dx,dy' formulation.