# Makefile for creating slice-viz executable

all: slice-viz

slice-viz: slice-viz.cpp svgprinter.cpp svgprinter.hpp CellField.hpp CellField.cpp CellFieldArray.hpp CellFieldArray.cpp CmdParams.h CmdParams.c ${IS_FILES}
	g++ -O0 -g -Wno-write-strings slice-viz.cpp svgprinter.cpp CellFieldArray.cpp CellField.cpp CmdParams.c -o slice-viz 

clean:
	-/bin/rm *.o slice-viz 2> /dev/null
