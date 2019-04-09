#!/usr/bin/env bash

# [NOTE]: consider using _tags instead

ocamlbuild -Is syntax,syntax/types,generators scripts/"$1".byte
