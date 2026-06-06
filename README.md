# virtual-memory-tlb-simulator
## Overview

This project is a software-based Memory Management Unit (MMU) simulator developed in Python. It demonstrates how a modern operating system translates 32-bit virtual addresses into physical addresses using a Page Table and a Translation Lookaside Buffer (TLB).

The simulator supports page replacement algorithms, dirty page handling, configurable system parameters, and Effective Access Time (EAT) calculation.

## Features

* 32-bit Virtual Address Translation
* Translation Lookaside Buffer (TLB)
* Page Table Management
* FIFO Page Replacement
* LRU Page Replacement
* OPT (Optimal) Page Replacement
* Dirty Bit Tracking
* Free Frame Management
* Page Fault Handling
* Effective Access Time (EAT) Calculation
* Configurable Memory Parameters
* Performance Statistics

## Technologies Used

* Python 3
* JSON Configuration Files
* Object-Oriented Programming

## Project Structure

project/
│
├── config.json
├── trace.txt
├── main.py
├── mmu.py
├── tlb.py
├── page_table.py
├── replacement_algorithms.py
├── statistics.py
└── README.md

## How to Run

1. Clone the repository

git clone https://github.com/yourusername/virtual-memory-tlb-simulator.git

2. Move to the project directory

cd virtual-memory-tlb-simulator

3. Run the simulator

python main.py

## Algorithms Implemented

* FIFO (First In First Out)
* LRU (Least Recently Used)
* OPT (Optimal Replacement)

## Performance Metrics

The simulator tracks:

* TLB Hits
* TLB Misses
* Page Hits
* Page Faults
* Disk Reads
* Disk Writes
* Fault Rate
* Effective Access Time (EAT)

## Learning Outcomes

This project demonstrates:

* Operating System Memory Management
* Virtual Memory Concepts
* Address Translation
* Page Replacement Algorithms
* Performance Analysis
* System Simulation Design

## Author

Muhammad Jabir Ikram

Computer Engineering

NUST College of Electrical and Mechanical Engineering (CEME)
