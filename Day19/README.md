* Day 19 - Message Parsing Nightmare

Given a bunch of recursive rules with 'OR' branches, find which messages of the form:

```abababbaaababaaabbb```

match the core rule 0.

This was easy for the example case, but the simple solution (keep trying every substring for every rule, working up to rule 0) didn't scale well at all for the real case.

The solution for Part 1, at least, was to use a tree structure of 'Rule' objects, where each rule has one or two vectors of Rules which must be satisfied. The exception is the base rules which map to 'a' and 'b'.

Then, given a message ```aabbaabb```, recursively snip off from the front any characters which match a rule and pass back up the tree. Whenever the tree successfully returns a blank string, this implies success.


* Part 2 - Infinite Loops

Now a couple of the rules also match themselves & another rule:

```8: 42 | 42 8
11: 42 31 | 42 11 31```

In the first case, rule 8 will match any length combination of ```42``` (actually whatever ab characters resolve to 42.

In the second case, rule 11 will match any nested combination of ```42 31```, ```42 42 31 31```, ```42 42 42 31 31 31```...

At the moment I'm not totally sure how to deal with this. I am committing my code for part 1 so I can safely mess with it.