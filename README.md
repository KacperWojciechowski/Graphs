# Graphs
Implementation of basic graph software made for university classes.

Overview:
-------------------------------------------------------------
The implementation provides adjacency matrix and adjacency list representations compatibile with the House of Graphs portal data formats, and GraphML format.
Each graph representation provides implementation of basic graph coloring and path searching algorithms. The adjacency list representation however, unlike adjacency matrix representation, does not provide weighted graphs support.

The implementation is suited for weighted and non-weighted undirected and directed graphs. It does support graphs with loops, however it does not support multigraphs and will treat them as weighted graphs instead.

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
- Graph::Examples library containing static methods to demonstrate the use and results of certain features.
