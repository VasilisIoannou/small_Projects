# Terminal Image Renderer and Bundler

This project is a C-based utility designed to render images directly into a terminal environment. It features a custom linked-list management system for handling image collections and provides tools for both single-image processing and batch directory bundling.

---

## Overview

The system transforms standard image pixel data into terminal-friendly output. Beyond simple rendering, it implements a robust "Image Bundle" system that scans directories, parses filenames for numerical ordering, and manages memory through a specialized linked-list structure.

## Core Data Structures

To handle dynamic image collections, the project utilizes a nested pointer architecture:

* **ImageFile**: A container for the filename and the raw Image data.
* **ImageFileNode**: A standard linked-list node containing an `ImageFile` pointer and a link to the next node.
* **ImageBundle**: A management structure holding a double-pointer to the head of the list, ensuring safe initialization and modification of the image collection.

---

## Building the Project

The project uses a multi-target Makefile to manage dependencies and compilation. 

### Prerequisites
* GCC Compiler
* Standard C Libraries (Math library linked via `-lm`)

### Compilation Commands
To compile both example programs and all dependencies, run:

make all
make clean

### Usage Examples

The project includes two primary example programs to demonstrate functionality.
1. Single Image Rendering

Use this program to render a single image file to your current terminal window.
Executable:  image_to_terminal.exe
Command: ./image_to_terminal.exe path/to/image.png

There is an example a 32x32 image called tree.png
(./image_to_terminal.exe tree.png)

2. Directory Bundling

This program scans an entire directory, builds a linked list of all images found, and processes them as a bundle.
Executable: imageBundle.exe
Command: ./imageBundle.exe path/to/image_directory/

There is an example of a directory called ./cats with 8 pictures of 32x32 images of a cats
(./image_to_terminal.exe cats)
