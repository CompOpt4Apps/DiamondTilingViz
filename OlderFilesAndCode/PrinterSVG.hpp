/******************************************************************************
 * GRIDGEN: Grid Generating Compiler
 * By: Andy Stone (aistone@gmail.com)
 * (C) Copyright 2011 Colorado State University
 * 
 * MMS, modified to work with Printer.hpp interface.
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 *****************************************************************************/
#ifndef PRINTERSVG_HPP_
#define PRINTERSVG_HPP_

#include "Printer.hpp"

#include <iostream>
using namespace std;

/**
 * This printer is used to output simple shapes and text into an SVG file.
 */
class PrinterSVG : public Printer {
  public:
    PrinterSVG(ostream &out, int height, int width, int radius, int spacing);

    void printHeader();
    void printFooter();
    void printCircle(int t, int x, int y, int red, int green, int blue);

    //void printCircle(int x, int y, int r, string stroke, string fill);
    void printCenteredText(int t, int x, int y, string text);
    //void printRectangle(int x, int y, int w, int h, string stroke, string fill);

  private:
    ostream &mOut;
    int mHeight;
    int mWidth;
    int mCellRadius;
    int mCellSpacing;
};

#endif
