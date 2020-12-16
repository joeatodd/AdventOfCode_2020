* Day 15 - Memory Games

The task is a memory game:

Given a set of starting numbers like

```6,3,15,13,1,0```

the next number will be the time since the last occurrence of the previous number, or 0 if this number is new.

Both Part 1 and Part 2 ask for the Nth number, with N=2020 (Part 1)and N=3E7 (Part 2).

The naive approach for this (which would work fine for part 1) would be to construct a vector of all the previous numbers, and search backwards for the number each time. However, I guessed that Part 2 would be a prohibitively large number, so I used a map instead.

The run time was still ~50 seconds on my laptop, so I was looking to speed this up. I found online that, weirdly, a std::unordered_map might speed things up. Indeed, the drops the run time down to ~16 seconds. I can't figure this out - the lookup should be slower, algorithmically, but apparently the unordered_map prevents cache misses by keeping the data closer together in memory.