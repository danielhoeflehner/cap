#!/bin/bash

set -e

# Make
cd build && make
cd test && ./ccapTest
