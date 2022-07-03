# Graphs
Implementation of basic graph software made for university classes.

The implementation provides adjacency matrix and adjacency list representations compatibile with the House of Graphs portal data formats, and GraphML format.
Each graph representation provides implementation of basic graph coloring and path searching algorithms. 

The implementation is suited for weighted and non-weighted undirected and directed graphs. It does support graphs with loops, however it does not support multigraphs and will treat them as weighted graphs instead.

Along with the raw implementation, this project provides additional features, such as:
- PixelMap class which allows to represent a 2D terrain map by binary values. This class can be used to build a graph representation of the terrain, which can be later used for tasks such as finding the shortest path.
- Benchmarking class allowing users to check the time and results of execution of different implemented algorithms, such as coloring algorithms.
- Examples class showing via static methods the way of using various features of the project, and exemplary results.
