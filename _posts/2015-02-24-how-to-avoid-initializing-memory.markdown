---
layout: post
title:  "How to avoid initializing memory"
date:   2015-02-25
description: How to avoid initializing memory by using constant time bookkeeping.
---

<p>
Consider an algorithm that uses a large memory area.
If the running time of the algorithm is smaller than the size of the memory,
initializing the memory will take longer than running the algorithm.
However, using a shrewd trick,
it’s possible to refrain from initializing the memory.
</p>

<p>
The idea is to keep track of which memory positions have been used so far.
Using this information, you may postpone the initialization until
the first time a memory cell is encountered.
</p>

<p>
One solution would be to use an array to keep track of active memory positions.
Every time a memory position is accessed,
check whether the address is in this array of active memory positions.
If not, initialize this memory cell and add the address to the array.
</p>

<p>
For this scheme to work without increasing the overall running time,
it’s crucial that the lookups in the array can be performed in constant time.
To achieve this, use double bookkeeping.
Not only do you use an array of active memory positions,
but each memory position is also extended to contain a pointer into this array.
In this figure, for instance, the memory has three active memory cells:
cell numbers 0, 4, and&nbsp;6.
</p>

<pre>
Memory                         Active
-------------------------      -------
Address Contents Pointer

0       134431   X---------->  0
1       938434      -------->  4
2       432754      |  ----->  6
3       292343      |  |
4       874944   X---  |
5       002345         |
6       654243   X------
7       112903
</pre>


<p>
Checking whether a memory cell is active is a two-step procedure.
First, you check whether the pointer attached to this memory position is valid:
Does it point to a position in the active array?
If not, the memory position contains garbage and has not been initialized.
If the pointer is valid, you also need to check whether the corresponding pointe
in the active array actually points to this memory address.
If it does, the memory cell is active.
</p>

<p>
This check only requires two memory accesses.
Hence, it can be performed in constant time.
Adding a new address to the active list is also done in constant time:
Store the memory address in the next free position in the active array and
set up the pointer associated with the memory location accordingly.
</p>

<p class="author"><a href="https://plus.google.com/+StefanNilsson/about?rel=author">Stefan Nilsson</a></p>

<p>
In <a href="http://drdobbs.com/architecture-and-design/184404062"><i>Dr. Dobb’s Journal</i></a>,
pp.&nbsp;38-45, Vol.&nbsp;311, April&nbsp;01,&nbsp;2000.
</p>
