/******************************************************************************
 * GRIDGEN: Grid Generating Compiler
 * By: Andy Stone (aistone@gmail.com)
 * (C) Copyright 2011 Colorado State University
 *****************************************************************************/
#ifndef SVGPRINTER_HPP_
#define SVGPRINTER_HPP_

#include <iostream>
using namespace std;

/**
 * This printer is used to output simple shapes and text into an SVG file.
 */
class SVGPrinter {
  public:
    SVGPrinter(ostream &out, int height, int width);

    void printHeader();
    void printFooter();
    void printText(int x, int y, string text);
    void printCircle(int x, int y, int r, string stroke, string fill);
    void printCenteredText(int x, int y, string text);
    void printUnderCenteredText(int x, int y, string text);
    void printRectangle(int x, int y, int w, int h, string stroke, string fill);

  private:
    ostream &mOut;
    int mHeight;
    int mWidth;
};

#endif
