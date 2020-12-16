* Day 16 - Ticket Fields

Given a set of tickets like:

```89,139,79,151,97,67,71,53,59,149,127,131,103,109,137,73,101,83,61,107```

and a set of rules like:

```departure location: 44-401 or 415-965```

Find tickets which have some fields which don't match any field's rule (Part 1), then find the order of the fields (given that they are the same for every ticket) (Part 2).

Part one is pretty straightforward - simply check each value in every ticket against all possible rules (ranges).

For Part 2, have to first work out which positions *might* correspond to which field, then find the field which has only 1 option, then eliminate and repeat.