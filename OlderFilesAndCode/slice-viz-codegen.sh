#!/bin/sh
# slice-viz.sh is a shell script that calls the isl code generator
# iscc for different tile sizes for parallelogram or diamond tiles.
#
# Michelle Strout, 9/21/13
# Copyright (c) 2013, Colorado State University
# 
# usage examples, must specify no parameters or all of them:
#   # Uses defaults and prints to diamonds-3x3x3.is
#   # The 3x3x3 indicates the values for tile sizes TAU, SIGMA, and GAMMA.
#   ./slice-viz-codegen.sh
#
#   # Code printed to pipelined-4x6x6.is
#   ./slice-viz-codegen.sh pipelined 4 6 6
#
#   # Code printed to diamond-prizms-6x6.is
#   # Even though only needs two parameters, need to provide all 3
#   ./slice-viz-codegen.sh diamond-prizms 6 6 999
#
#   # Code printed to diamond-prizms-6x6.is
#   # Even though only needs two parameters, need to provide all 3
#   ./slice-viz-codegen.sh diamond-prizms-noping 6 6 999
#
#
# Files this script depends on:
#   facts.piscc
#   diamonds-tij-skew.piscc
#   pipelined-skew.piscc
#   diamond-prizms-skew.piscc
#   diamond-prizms-skew-noping.piscc
#   iscc  // should be installed and available in path
#

##############################################################
# Main routine for slice visualization.

# Prints out all commands being executed by shell for debugging.
#set -x

# Default values for parameters.
tiling="diamonds"
tau_val=3
sigma_val=3
gamma_val=3

# Check for the various command-line parameters.
if [ $# -gt 0 ]
then
    tiling=$1
    tau_val=$2
    sigma_val=$3
    gamma_val=$4
fi

tau_sub="s/TAU/$tau_val/g"
sig_sub="s/SIGMA/$sigma_val/g"
gam_sub="s/GAMMA/$gamma_val/g"

# Create the input files for ISL.
echo "==== Creating the input for ISL ===="
(cat facts.piscc; echo ' '; cat pipelined.piscc) | sed -e $tau_sub -e $sig_sub -e $gam_sub > pipelined.iscc
(cat facts.piscc; echo ' '; cat diamonds-tij-skew.piscc) | sed -e $tau_sub -e $sig_sub -e $gam_sub > diamonds-tij-skew.iscc
(cat facts.piscc; echo ' '; cat diamond-prizms-skew.piscc) | sed -e $tau_sub -e $sig_sub > diamond-prizms-skew.iscc
(cat facts-nopingpong.piscc; echo ' '; cat diamond-prizms-skew-noping.piscc) | sed -e $tau_sub -e $sig_sub > diamond-prizms-skew-noping.iscc

# Have ISL do the code generation.
echo "==== ISL doing code gen ===="
echo "TAU=$tau_val, SIGMA=$sigma_val, GAMMA=$gamma_val"
echo "tiling = $tiling"
if [ $tiling = "diamonds" ]
then
    echo "Generating diamonds-tij-skew-${tau_val}x${sigma_val}x${gamma_val}.is"
    iscc < diamonds-tij-skew.iscc | sed -e 's/for (int /for (/' > diamonds-tij-skew-${tau_val}x${sigma_val}x${gamma_val}.is
fi

if [ $tiling = "pipelined" ]
then
    echo "Generating pipelined-skew-${tau_val}x${sigma_val}x${gamma_val}.is"
    iscc < pipelined.iscc | sed -e 's/for (int /for (/' > pipelined-${tau_val}x${sigma_val}x${gamma_val}.is
fi

if [ $tiling = "diamond-prizms" ]
then
    echo "Generating diamond-prizms-skew-${tau_val}x${sigma_val}.is"
    iscc < diamond-prizms-skew.iscc | sed -e 's/for (int /for (/' > diamond-prizms-skew-${tau_val}x${sigma_val}.is
fi

if [ $tiling = "diamond-prizms-noping" ]
then
    echo "Generating diamond-prizms-skew-noping-${tau_val}x${sigma_val}.is"
    iscc < diamond-prizms-skew-noping.iscc | sed -e 's/for (int /for (/' > diamond-prizms-skew-noping-${tau_val}x${sigma_val}.is
fi

echo


