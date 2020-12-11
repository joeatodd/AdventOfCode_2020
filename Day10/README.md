Day 10 - Combinatorics
----

Given a series of 'joltage adaptors', each of which accepts an input joltage 1,2 or 3 jolts lower than its output, find all the paths from 0 jolts to max_jolts.

Part 1 is super easy - sort the adaptors and count the gaps of 1 & 3 jolts.

Part 2 was tough! There are too many combinations (trillions) to do it manually, so instead we need to take advantage of the fact that there are some gaps of 3 in the given adaptors.

Each gap of 3 defines a break between independent combinations. For example if I have a 15 jolt and an 18 jolt adaptor, and none in between, I know every combination from 0 to max_joltage must use this *one* route.

Therefore, the task is to break down into independent subsets, compute the combinations for each, then compute the product of these combinations.