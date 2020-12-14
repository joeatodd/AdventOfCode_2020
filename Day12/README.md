Day 12 - Shipping!
----

The task today is, given a set of coordinate vectors (some of which represent absolute translation and some of which re-orientate the ship), find the manhattan distance of the final location of the ship relative to origin.

This was pretty easy.

In Part 2, the task changes. All instructions except for ```forward N``` actually move a 'waymarker' relative to the ship. When the ```forward N``` instruction is provided, the ship moves to the way marker N times.

In fact, the way marker can simply take the place of the previous orientation vector, with the generalization that it's no longer normalized. Small modification to my previous code to get Part 2 working.