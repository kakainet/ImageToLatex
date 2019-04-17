(* [NOTE]: consider a function Array.random_choice [array] *)
(* [NOTE]: consider refactoring binary operators as plural *)

open Core

open Syntax
open Structures.Brackets

module Unary = struct

    let unary = plural

    let negate ?(brackets=round) expr =
        brackets (unary (sprintf "-%s" (to_string (apply_to_plural brackets expr))))

end

module Binary = struct

    let binary = singular

    let random ?(pool=[|"+"; "-"; "\cdot"; "/"|]) () =
        binary (pool.(Random.int (Array.length pool)))

end

