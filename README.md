Terra
=====

Low-level library

Terra is a library designed to handle various tasks both on Windows and Linux. Each part has been divided between various folder
for specific purposes. For example, a folder like structure contains various data structures that the user can use at their leisure.

Implemented features:
Base: Memory allocation, Error manager, PRG, Thread, Time and imer.
Debugging: Asserts, log file, memory leak detector, profiler, crash report.
Math: Basic mathematic objects.
Hardware : Computer monitor listing.
IO : Stream reader, stream writer, path manager.
Structures : Stack, Linked List, Dynamic Array, Heap, Red-Black Tree and QuadTree.
Utility : Various string, integer and file system manipulation functions and an argument parser.

Build:
Terra is designed as a Static library. To build it, you need to include terra.h from the source folder and build it using either the visual studio project file on windows or make on linux.
