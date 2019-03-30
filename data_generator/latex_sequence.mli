(**********************************************)
(*                 KakaiNet                   *)
(* Script for generating semi latex sequences *) 
(* let place_holder_i = phi                   *)
(* @f ph1 @f@ ph2 f@ <=> \frac{ph1}{ph2}      *)
(**********************************************)

(*
@brief generates random semi latex sequence with nesting
@param length of output
@param nest - degree of nesting (at most)
@return semi latex sequence
*)
val nesting_seq : int -> int -> string list

(*
@brief generates random semi latex sequence
@param length of output
@return semi latex sequence
*)
val flat_seq : int -> string list
