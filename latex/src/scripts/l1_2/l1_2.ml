open Core

open Syntax
open Generators.L1

let () = Random.self_init ()

let () =
    List.map ~f:(fun props -> generate_out props |> to_string)
        (read_in Sys.argv.(1)) |> List.iter ~f:print_endline

