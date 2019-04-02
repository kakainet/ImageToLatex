(*                 KakaiNet                     *)
(* Module for generating sequences of           *)
(* basic integer arithmetic expressions         *)
(* with nested fractions in pseudo-latex syntax *)
(* @f exp1 @f@ exp2 f@ <=> \frac{exp1}{exp2}    *)

type sequence

val sequence : (int * int * bool * float) list -> sequence

val to_string_list : sequence -> string list

val to_string : sequence -> string
