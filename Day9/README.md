# Day 9 - Cryptography (?)


Today's challenge involves a series of numbers, in which each number *ought* to be a sum
of 2 of the preceding 25 numbers.

## Part 1

Part 1 is to find the first number in the series for which this isn't the case, the *weak number*.

This is pretty straight forward to do, but I wanted to do it as efficiently as possible. I took a c++ multiset of the preamble for each number (effectively just an ordered list with optimized insert/lookup). Then I search every value smaller than the target, and search for its complement.

## Part 2

Part 2 is to find a contiguous portion of the input array which sums to the *weak number* (i.e. no longer just two values).

So far I've achieved this simply by trying every possible lower bound in turn, but I know it can be made more efficient.

## Part 2.2

I'm going to do this better. I can consider a contiguous range from ```i``` to ```j```, starting at the beginning. If the current sum is less than the target value, I increment ```j```, if it's greater than the target value, I increment ```i```. This should reach the right answer with fewer operations.

Yay it works!
