SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ifeq ($(origin .RECIPEPREFIX), undefined)
	$(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or late)
endif
.RECIPEPREFIX = >

CXX = g++
CXXFLAGS = --std=c++2a

SOURCES = $(wildcard *.cc)
HEADERS = $(wildcard *.h)

main: $(SOURCES) $(HEADERS)
> $(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
> rm -f *.o main
.PHONY: clean