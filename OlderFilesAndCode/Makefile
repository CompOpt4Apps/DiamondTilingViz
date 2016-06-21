# Makefile for creating slice-viz executable

all: slice-viz diamond-slice-viz #diamond-slice-viz-pov

IS_FILES = pipelined-4x4x4.is

slice-viz: slice-viz.cpp svgprinter.cpp svgprinter.hpp CellField.hpp CellField.cpp CellFieldArray.hpp CellFieldArray.cpp CmdParams.h CmdParams.c ${IS_FILES}
	g++ -O0 -g -Wno-write-strings slice-viz.cpp svgprinter.cpp CellFieldArray.cpp CellField.cpp CmdParams.c -o slice-viz 

diamond-slice-viz: diamond-slice-viz.cpp svgprinter.cpp svgprinter.hpp CellField.hpp CellField.cpp CellFieldArray.hpp CellFieldArray.cpp CmdParams.h CmdParams.c 
	g++ -O0 -g -Wno-write-strings diamond-slice-viz.cpp svgprinter.cpp CellFieldArray.cpp CellField.cpp CmdParams.c -o diamond-slice-viz 

diamond-slice-viz-pov: diamond-slice-viz-pov.cpp Printer.hpp PrinterPOV.hpp PrinterPOV.cpp PrinterSVG.hpp PrinterSVG.cpp CmdParams.h CmdParams.c  ColorInfo.hpp ColorInfo.cpp
	g++ -O0 -g -Wno-write-strings diamond-slice-viz-pov.cpp PrinterSVG.cpp PrinterPOV.cpp CmdParams.c ColorInfo.cpp -o diamond-slice-viz-pov 

#diamond-tile-viz: diamond-tile-viz.cpp svgprinter.cpp svgprinter.hpp CellField.hpp CellField.cpp CellFieldArray.hpp CellFieldArray.cpp
#	g++ -O0 -g diamond-tile-viz.cpp svgprinter.cpp CellFieldArray.cpp CellField.cpp -o diamond-tile-viz 
	

clean:
	-/bin/rm *.o diamond-tile-viz slice-viz 2> /dev/null
