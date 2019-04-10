(* [NOTE]: consider using Sequence.random instead of manual List.init *)

open Core

open Syntax

open Instances
open Operators

open Structures
open Functions

let odd x = assert (x > 0);
    if x % 2 = 0 then x - 1 else x

module L1 = struct

    let generate_in nst len bnd chc =
        List.init (Random.int_incl 1 nst)
            (fun i -> (Random.int_incl 1 len, Random.int_incl 0 bnd, Random.float chc))

    let rec generate_out props =
        match props with
        | (len, bnd, chc) :: [] ->
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

    let read_in ~path =
        In_channel.read_lines path |>
        List.map ~f:(fun ln -> String.split ~on:';' ln |>
            List.map ~f:(fun nst ->
                match String.split ~on:',' nst |> List.map ~f:String.strip with
                | len :: bnd :: chc :: [] ->
                    (Int.of_string len, Int.of_string bnd, Float.of_string chc)
                | _ -> assert false))

    let write_out ~path exprs =
        Out_channel.write_lines path (List.map ~f:to_string exprs)

end

