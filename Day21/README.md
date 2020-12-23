* Day 21 - Logical Allergens

Given a list of ingredients (unknown names) and a (possibly non-exhaustive) list of allergens, identify which ingredients are which allergens.

Approached this by maintaining a set of *possible ingredients* for each *allergen*. Each time the allergen appears with a new set of ingredients, take the intersection of those sets.

** Part 1

Find the ingredients which can't possibly be allergens. These are the ones which don't appear in any of the allergen -> possible ingredient sets.

** Part 2

Identify the allergens. This is logical deduction - find an allergen with set size 1, then knock that ingredient from other allergen sets, repeat.

Pretty easy challenge compared to the others.