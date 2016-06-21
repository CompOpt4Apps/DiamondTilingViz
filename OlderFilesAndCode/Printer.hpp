/*!
 * \file Printer.hpp
 *
 * \brief Interface for a printer that can print circles, text, and rectangles.
 *
 * Coordinates will be given in terms of a 3D iteration space and the various
 * printers should covert those coordinates to where things should actually 
 * be printed.
 *
 * \date Started: 12/21/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */
#ifndef PRINTER_HPP_
#define PRINTER_HPP_

#include <iostream>
using namespace std;

class Printer {
  public:

    virtual void printHeader() = 0;
    virtual void printFooter() = 0;
    virtual void printCircle(int t, int x, int y, 
                             int red, int blue, int green)=0;
    //void printCircle(int x, int y, int r, string stroke, string fill);
    virtual void printCenteredText(int t, int x, int y, string text)=0;
    //void printRectangle(int x, int y, int w, int h, string stroke, string fill);

};

#endif
