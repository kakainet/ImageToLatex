(* [NOTE]: consider using Sequence.random instead of manual List.init *)

open Core

open Latex
open Syntax

open Components
open Instances
open Operators

open Structures
(*open Functions*)

let odd x = assert (x > 0);
    if x % 2 = 0 then x - 1 else x

module type L = sig
    type conspect
    type properties
    val generate_in : conspect -> properties
    val generate_out : properties -> t
end

module L1 = struct

    type conspect = int * int * int * float
    type properties = (int * int * float) list

    let create_conspect (a : int * int * int * float) = (a : conspect)

    let generate_in consp =
        let (nst, len, bnd, chc) = consp in
        List.init (Random.int_incl 1 nst)
            ~f:(fun _ -> (Random.int_incl 1 len, Random.int_incl 0 bnd, Random.float chc))

    let rec generate_out props =
        match props with
        | (len, bnd, _) :: [] ->
            sequence (List.init (odd len) ~f:(fun i ->
                if i % 2 = 0 then Constant.Int.random (-bnd) bnd else Binary.random ()))
        | (len, bnd, chc) :: t ->
            sequence (List.init (odd len) ~f:(fun i -> 
                if i % 2 = 0 then begin
                    if Random.float 100.0 < chc
                    then fraction (generate_out t) (generate_out t) 
                    else Constant.Int.random (-bnd) bnd
                end else Binary.random ()))
        | _ -> assert false
        
end

