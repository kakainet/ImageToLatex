(* [NOTE]: consider using Sequence.random instead of manual List.init *)

open Syntax

open Instances
open Operators

open Structures
open Functions


let rec generate_L1 props =
    match props with
    | (len, bnd, chc) :: [] -> assert (len mod 2 <> 0);
        sequence (List.init len (fun i ->
            if i mod 2 = 0 then Constant.Int.random bnd else Binary.random ()))
    | (len, bnd, chc) :: t -> assert (len mod 2 <> 0);
        sequence (List.init len (fun i -> 
            if i mod 2 = 0 then begin
                if Random.float 100.0 < chc
                then fraction (generate_L1 t) (generate_L1 t) 
                else Constant.Int.random bnd
            end else Binary.random ()))
    | _ -> assert false
