(* [NOTE]: consider a function Array.random_choice [array] *)
(* [NOTE]: consider refactoring binary operators as plural *)

open Printf

open Syntax
open Structures.Brackets

module Unary = struct

    let unary = plural

    let negate ?(brackets=round) expr =
        brackets (unary (sprintf "-%s" (to_string expr)))

end

module Binary = struct

    let binary = singular

    let random ?(pool=[|"+"; "-"; "*"; "/"|]) () =
        binary (pool.(Random.int (Array.length pool)))

end
