#!/usr/bin/env bash

# [NOTE]: consider using _tags instead

ocamlbuild -use-ocamlfind -Is syntax,syntax/types  -tag thread -pkg core scripts/"$1"/"$1".native
