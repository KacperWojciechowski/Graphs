# Graphs
Implementation of basic graph software made for university classes.


Overview:
-------------------------------------------------------------
The implementation provides adjacency matrix and adjacency list representations compatibile with the House of Graphs portal data formats, and GraphML format.
Each graph representation provides implementation of basic graph coloring and path searching algorithms. Each of the representations support both directed and undirected graphs, however does not support multigraphs, and graphs with negative weights (the latter causing to raise an exception).

The implementation is suited for weighted and non-weighted undirected and directed graphs. It does support graphs with loops, however it does not support multigraphs and will treat them as weighted graphs instead.
Graph representation classes implement a common interface via inheritance from an abstract Graph class, which allows user to exchange the representations without modifying the code around it. 

Features:
-------------------------------------------------------------
**Each of the representation implements following algorithms:**
- Greedy coloring with optional permutations
- LF coloring with optional permutations
- SL coloring with optional permutations
- Belman-Ford one-to-all path search

**Matrix representation also implements:**
- modified Belman-Ford algorithm one-to-all path search with throughtput requirements

**The project provides additional features such as:**
- Data::PixelMap class allowing for representation of a terrain map via binary values. Can be used with the graph representations to create a graph of the terrain for tasks such as path searching.
- Data::Benchmark class generating the raports of time efficiency and results of various algorithms implemented by the graph representations.
- Data::Generator class allowing user to generate sample weighted graphs, pixel maps and throughput matrices.
- Examples library available also in documentation, showing how to use the library's API

**For further information see doxygen documentation**

Side notes:
------------------------------------------------------------
Current CMakeLists.txt version requires user to have installed g++-10 and gcc-10 in their /usr/bin directory. To be fixed in the future.
