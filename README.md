# Red Black Tree

A red-black tree is a binary search tree where each node has a color attribute, the value of which is either red or black. Essentially, it is just a convenient way to express a 2-3-4 binary search tree where the color indicates whether the node is part of a 3-node or a 4-node. 2-3-4 trees and red-black trees are equivalent data structures, red-black trees are simpler to implement, so tend to be used instead.

Binary search property or order property: the key in each node must be greater than or equal to any key stored in the left sub-tree, and less than or equal to any key stored in the right sub-tree.

In addition to the ordinary requirements imposed on binary search trees, we make the following additional requirements of any valid red-black tree.

Red-black properties:

1. Every node is either red or black.
2. The root and leaves (NIL's) are black.
3. Parent of each red node is black.
4. Both children of each red node are black.
5. Every path from a given node to any of its descendant NIL nodes contains the same number of black nodes.
