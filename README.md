# TileMapEditor

A general purpose map editor. This program allows users the flexibility of importing their own tilesets to create maps. TileMapEditor is designed to be intuitive and easy to use. The maps produced by this program can be used for any purpose, but are primarily for game making. The output of this program can be any number of different formats, from a special format specifically for use with this program, to images or .c files.

## Compiling
To compile this program you will need the following things installed
1. Cmake
2. wxWidgets 3.0
3. Boost 1.54
4. ImageMagick (Magick++ specifically)

To set up the build environment run cmake 

## Note
Please note that this is a work in progress, its not actually complete and this repo contains the major API rewrite I am doing as I got frustrated with the design of my old codebase.

A build that exists with the old codebase and was nearly fully complete mind you can be found at the following link [TileMapEditor](https://drive.google.com/open?id=0B6g7zcZaFwPTYi1xTXBFcW5aZDA) It is pretty usuable and comes complete with documentation, it supports exporting in a variety of ways (As a single image, As an image for each layer, in a format specific to this application, text, xml, and .c files for gba development).
