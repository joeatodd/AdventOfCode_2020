Day 8 - Assembly
----

Today's task was a lot easier than yesterdays, and it smells like the start of a multi-day challenge like last year's Intcode computer. jmp, acc, nop are assembly language!

We're given a series of opcodes as input, and told that the program is broken (infinite loop).

Part 1 task was to find the value of 'acc' when the program first repeats an instruction.

Part 2 was to fix the machine by swapping a 'nop' for a 'jmp'. I brute forced this by trying every possible swap. I guess this is the quickest way? Not sure...

I am going my assembly stuff into its own class because I have a feeling I'll be reusing it.
