#!/usr/bin/env li

import <os>
import <colorizer>

# create a colorizer function
const color = colorizer();

# read the files in the current directory
const files = listdir(".");

# loop through the files
foreach (file : files) {
    # print the file name with a color
    println(color() + file + Color.reset);
}
