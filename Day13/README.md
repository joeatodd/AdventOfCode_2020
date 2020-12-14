Day 13 - Chinese Remainder Theorem
----

I must admit that I got completely stuck on this until TJ gave me a hint, which was to use Chinese Remainder Theorem.

Part 1 was very straight forward - given a series of buses which arrive periodically according to their ID (e.g. 17), find the next bus after a given time.

** Part 2 

This was a real challenge: Given a series of buses, find the first time that they arrive at a certain spacing in time.

So, for example:

```7,13,x,x,59,x,31,19```

means find the time when bus 7 arrives, followed 1 min later by bus 13, 4 min later bus 59 shows up.

I worked out fairly quickly that this is a modular arithmetic problem, but trying to write an algorithm for this was really tough. In the end I adopted the 'sieving' technique from the wiki page on [Chinese Remainder Theorem](https://www.wikiwand.com/en/Chinese_remainder_theorem). Even this was quite slow until I pre-sorted the bus return intervals in descending order.