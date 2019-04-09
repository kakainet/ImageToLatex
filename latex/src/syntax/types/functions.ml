open Printf

open Syntax
open Structures.Brackets

let factorial ?(brackets=round) expr =
    plural (sprintf "%s!" (to_string (apply_to_plural brackets expr)))

let power ?(brackets=round) expr1 expr2 =
    plural (sprintf "%s^{%s}" (to_string (apply_to_plural brackets expr1)) (to_string expr2))

