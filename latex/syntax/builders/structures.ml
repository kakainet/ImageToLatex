(* [NOTE]: consider renaming params in Brackets.brackets *)
(* [NOTE]: consider making Sequence.random [length] [next] *)

open Printf

open Syntax

module Brackets = struct

    let brackets op cl expr =
        singular (sprintf "\\left%s%s\\right%s" op (to_string expr) cl)

    let round = brackets "(" ")"

    let square = brackets "[" "]"

    let curl = brackets "\\{" "\\}"

end

let fraction expr1 expr2 =
    plural (sprintf "\\frac{%s}{%s}" (to_string expr1) (to_string expr2))

let sequence exprs =
    plural (String.concat " " (List.map to_string exprs))

