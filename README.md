# Surface_Analysis
Program for analyzing surface of proteins. 


# Setup
This project requires WXWidgets (tested with [3.0](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.0.5)) 

``g++ ./src/*.cpp -std=c++17 -O3 -o ./bin/surface_analysis.exe `wx-config --cxxflags --libs std` ``

This project uses PyMOL and comes with the windows version of PyMOL at bin/PyMOL. If you wish try to run on linux using wine change line 21 of src/fetch.hpp to
`string temp{"wine ../../PyMOL/python.exe ../../fetch.py "};`

# Use
To load a protein use the Protein -> From PDB... or Protein -> From File... to load either the PDB or from a local file.

To analyze a protein use Analyze -> Analyze... to add an analyses to the queue and Analyze -> Run... to run the entire queue.

