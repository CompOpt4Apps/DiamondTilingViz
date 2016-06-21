/*!
 * \file PrinterPOV.hpp
 *
 * \brief Object that knows how to print a povray file and can print spheres.
 *
 *
 * \date Started: 12/20/14
 *
 * \authors Michelle Strout
 *
 * Copyright (c) 2014, Colorado State University <br>
 * All rights reserved. <br>
 */
#ifndef PRINTERPOV_HPP_
#define PRINTERPOV_HPP_

#include "Printer.hpp"

#include <iostream>
using namespace std;

/**
 * This printer is used to output simple shapes and text into an SVG file.
 */
class PrinterPOV : public Printer {
  public:
    PrinterPOV(ostream &out);

    void printHeader();
    void printFooter();
    void printCircle(int t, int x, int y, int red, int blue, int green);
    void printCenteredText(int t, int x, int y, string text) {}

  private:
    ostream &mOut;
};

#endif
