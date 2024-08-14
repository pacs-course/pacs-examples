# A generic implementation of an Alternate Digital Tree (ADT) for spatial searches #

An ADT tree is a binary tree that stores "Cartesian boxes" or points (a point is a 0d Box) and allows a fast search for intersections or inclusions.
Compared to other binary spatial search strategies, like RB trees, its advantage is the flexibility in addition/deletion of new node, at the price of lack of control on the equilibration of the underlying binary tree.  It may be therefore advantageous in a very dynamic setting, less in more static ones.

It is a *region based* data structure: the position of a node depends on its coordinates, not on those of its parent node in the tree.

The name comes from the fact that in theory (but complicated in practice) it may be reduced to the analysis of individual bits.

Just to illustrate the point let's consider the case an ADT tree that stores four 1D points represented in binary format.
The points are p1=1001 p2=0100 p3=0110 p4=0001. They are inserted in the structure one at a time. p1 will go at the root of the tree. Then we insert p2. The root is already used so we look at the digit corresponding to the root level (which is level 0). So we look at the first digit. if the digit is zero we move to the left, if it is one we move to the right. Since the first digit of p2 is 0 we place it to the left of p1:

```
          p1
         /
        p2
```

Now p3. The root is taken, the first digit is 0, the location at the left of the root is taken so we look at the second digit. It is 1, so we move to the right:



```
          p1
         /
        p2
         \
          p3
```

Now p4: the second digit is 0 and the place at the left of p2 is available:

```
          p1
         /
        p2
       /  \
      p4    p3
```

Clearly we normally operate with floating point numbers, and each point has more than one coordinate. 
The fact that we use floating points and not binary numbers is solved easily 
if we the max and min coordinates of all points that are going to be inserted. 
This is indeed another requirement for our ADT implementation. Then, after a re-scaling 
of the coordinates between 0 and 1, one can test them  against 2<sup>-l</sup>, l being the level of the tree, 
to decide whether to move to the left or the right branch.

Finally, how to treat points in nD and Boxes? Here we find out the meaning of the term "alternating": 
the test is done alternating across the dimension: in a nD, with point's coordinates (x<sub>0</sub>,..,x<sub>i</sub>,..x<sub>nD</sub>) the test is made rotating cycling the coordinates and against 2<sup>-l/nD</sup> (integer division). Note that it means that at tree level l we test coordinate with index l mod nD.

Boxes are defined by the two corners defining the minimum and maximum coordinates. 
An ADT tree *Box node* is seen as a 2nD point where the min and max coordinates of the box are alternated. 
Therefore, the algorithm to insert a box is the same as the one to insert a point. 
We need however to distinguish between the number of physical coordinates and the number of coordinates used 
internally by the algorithm, which is double the number of physical coordinates in case of boxes.

The code provides the class template

```
  template <std::size_t DIM, AdtType Type = AdtType::Box> 
  class Box
```
to define a Box. `AdtType` can be either `AdtType::Point` or `AdtType::Box`. 
In the first case the Box is infact a single point. `DIM` is the physical dimension. 

The basic element to be stored in the AdtTree is the **AdtNode**, defined by the variadic template class

```
template <typename BOX, typename... Args>
  struct AdtNode : public BasicNode, public BOX, public Args...

```
The additional template parameters con be defined by the user to enrich the node. A note require a `AdtBox` type as first argument.

For instance,

    AdtNode<Box<2,AdtType::Box>> node;
creates a node capable of storing a 2-dimensional box.

This version od `AdtTree` requires to store the boxes normalised between 0 and 1, and the normalization must be made **outside** the `AdtTree`. This for efficiency, to avoid having to normalize the stored nodes at every search.



We have said that coodinates must be scaled between 0 and 1. This can be done, for instance, by creating a `AdtTree::BOX` storing the bounding box:

    Box<2,AdtType::Box> boundingbox;

and pass it to a `Normaliser` object:

    Normalizer normalize(boundingbox);
    
or creating a vector of boxes representing the non-normalised data, and use the `setBoundingBox()` method

    std:;vector<Box<2,AdtType::Box>> boxes;
    // fill boxes
    
    Normalizer normalize;
    normalize.setBoundingBox(boxes);
    
At this point the normalizer is a callable object that takes by
reference a box and returns it scaled in-place:
    
    Box<2,AdtType::Box> b;
    // fill b
    normalize(b); // b is now normalized

You can recover the original box with the method `denormalize()`. Remember that a node is a Box, so all said above can be applied to nodes.

    
Once you have the list of normalised nodes, you can create the `AdtTree`, whose definition is

    template <typename NODE> 
    class AdtTree

insert the nodes singularly with the `add()` method. For instance,

```
using Node=AdtNode<Box<2,AdtType::Box>>;
AdtTree<Node> adt;
...
  for(auto const & n:vp)
    adtTree.add(b);
```

or, if you want to move

```
  for(auto & n:vp)
    adtTree.add(std::move(b));
```

If you know the maximal expected number of nodes to be stored in the tree structure is more convenient to indicate it in the constructor

```
AdtTree<Node> adt{size};

```
or set it with the methos `increaseSize()`
```
AdtTree<Node> adt;
...
adt.increasSize(size);

```


## Node Storage ##

Internally the nodes are stored in a linear structure. The idea is to reduce cache misses, 
even if that goal could be obtained only by an appropriate renumbering, which is not implemented yet. The consequence 
of having a linear structure is that any node is identified by an Id that, 
for a node stored in an `AdtTree` structure, can be obtained with the method `Id()`

Having the `Id` of a node it is possible to delete it using

```
adtTree.erase(id);
```

Not only, if you have a valid `Id` you can retrive the node  from the tree using

```
auto node = adtTree[id];
```

## Visitors ##
Searching an AdtTree is performed using **visitors**, which are given as argument of the method

```    
template <class Visitor> 
 Visitor  AdtTree::visit(Visitor visitor);
```

which returns a copy of the visitor, after it has visited the tree. So it is responsibility of the visitor to collect the information we want to extract. In other word, visitor may be a stateful object whose role is to collect data while visiting.

The method `visit` traverse the tree in a preorder fashion, and after the visit of a node it returns the instuction on how to continue.

The key is the following line in `AdtTree::visit()` 

    Action  status = visitor(thisNode, control, level);

`Action` can be 

```
 enum Action
  {
    Stop = 0,
    GoLeft = 1,
    GoRight = 2,
    GoAll = GoLeft | GoRight
  };
```
- `Stop`   visit ends returning the current visitor. 
- `GoLeft`  go only to the left branch (if not empty)
- `GoRight` go only to the right branch (if not empty)
- `GoAll`  continue preorder trasversal by visiting both branches (left first).

The visitor technique allow to develop different search algorithms using the same structure. For instance the `BasicTrasversal` visitor just visits all mesh nodes collecting the indexes. While,

```
template <std::size_t DIM, BOXTYPE>
  class IntersectionVisitor
```
computes intersections with a given box.

The implementation of a visitor is a bit complicated. It is a callable object that implements

```
template<typename ADTNODE>
    Action operator()(ADTNODE const & node, NodeControl<ADTNODE::BOXDIMS> const & control, std::size_t level)
```
where 
- `node` is the node being visited
- `control` is the control structure of type   `template <std::size_t BOXDIMS> class NodeControl` which returns the information on the level and position in the tree structure
- `level` The level on the tree.

**A Note** The `control` structure is a bit complicated, but it is used to avoid code replication. It is a template class that returns the information on the level and position in the tree structure. The template parameter `BOXDIMS` is the dimension of the box.


# What do I learn here? #
- A rather complex data structure;
- The use of generic programming to avoid code replications: we treat Points and Boxes in the same code;
- An example of visitor design pattern. Indeed this is a simplified version.
- The use of variadic templates to define a node with additional data.
- The use of `std::enable_if` to define a template function that is enabled only if a condition is true.


## Bibliography ##
- Bonet, J. and Peraire, J. (1991), An alternating digital tree (ADT) algorithm for 3D geometric searching and intersection problems. Int. J. Numer. Meth. Engng., 31: 1-17. https://doi.org/10.1002/nme.1620310102
- Samet, H., 1990. The design and analysis of spatial data structures (Vol. 85, p. 87). Reading, MA: Addison-wesley.
- Samet, H., 2006. Foundations of multidimensional and metric data structures. Morgan Kaufmann.



