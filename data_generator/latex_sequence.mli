(**********************************************)
(*                 KakaiNet                   *)
(* Script for generating semi latex sequences *) 
(* let place_holder_i be phi                  *)
(* @f ph1 ph2 <=> \frac{ph1}{ph2}             *)
(**********************************************)

(*
@brief generates random semi latex sequence with nesting
@param length of output
@param nest - degree of nesting
@return semi latex sequence
*)
val sequence_with_nesting : int -> int -> string list

(*
@brief generates random semi latex sequence
@param length of output
@return semi latex sequence
*)
val flat_sequence : int -> string list
