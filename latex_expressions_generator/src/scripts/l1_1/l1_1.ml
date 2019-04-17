open Core

open Syntax
open Generators.L1

let () = Random.self_init ()

let () =
    for _ = 1 to Int.of_string Sys.argv.(1) do
        generate_in 2 10 512 75.0 |> generate_out |> to_string |> print_endline
    done
