* Day 18 - BEASDM

Given a bunch of arithmetic problems:

1 + (2 * 3) + (4 * (5 + 6))

compute the answer, given either NO order preference between * and + (Part 1), or inverted preference (Part 2).

My approach was to read the expression into a list of chars, and then work through them left to right. Brackets make things harder. So, I wrote a recursive function which searches for balanced parentheses, then calls itself with the bracketed expression. When no more brackets are found, the maths are evaluated according to the current rules, then fed back up.

This works pretty well and executes fast.