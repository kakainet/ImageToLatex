open Core

open Syntax
open Generators.L1

let () = Random.self_init ()

let () =
    List.init (Int.of_string Sys.argv.(1))
        ~f:(fun _ -> generate_in 2 10 512 75.0 |> generate_out |> to_string)
            |> List.iter ~f:print_endline
        
