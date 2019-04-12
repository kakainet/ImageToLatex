open Core

open Syntax
open Generators.L1

let () = Random.self_init ()

let () =
    List.map ~f:generate_out (In_channel.(input_lines stdin) |> read_in) |> write_out

