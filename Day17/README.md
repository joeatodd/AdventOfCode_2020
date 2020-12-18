* Day 17 - 4D game of life

This was not too tough but my solution wouldn't scale very well.

It's Conway's Game of Life but in 3D (Part 1) and 4D (Part 2).

I implemented this directly as 3D and 4D vectors of chars. We know the maximum required size because the number of iterations is given (6).

My solution runs very quickly compared to Baz's, but he implemented a map from x,y,z,w -> active/inactive. This scales better because it wouldn't need to consider the huge blank empty space.

For example, for 400 iterations of mine, even using just 2-byte character arrays, would require 56 Gb of RAM...