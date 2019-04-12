#!/usr/bin/env bash

# [NOTE]: consider using _tags instead

ocamlbuild -use-ocamlfind -Is latex,latex/types,scripts -tag thread -pkg core scripts/"$1"/"$1".native
