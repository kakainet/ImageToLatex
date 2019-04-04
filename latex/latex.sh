#!/usr/bin/env bash

# [NOTE]: consider using _tags instead

ocamlbuild -Is syntax,syntax/builders latex.byte
./latex.byte "$@"
