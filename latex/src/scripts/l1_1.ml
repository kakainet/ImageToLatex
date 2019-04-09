open Syntax

open Generators

let rec random_odd_incl bnd =
    let x = Random.int (bnd + 1) in
    if x mod 2 <> 0 then x else random_odd_incl bnd

let feed_L1 nest_cap len_cap bnd_cap chc_cap =
    List.init nest_cap (fun _ ->
        (random_odd_incl len_cap, Random.int (bnd_cap + 1), Random.float chc_cap))

(* path, count, nest_cap, len_cap, bnd_cap, chc_cap *)
(* test.out 25 2 5 512 70.0 *)

let () =
    let oc = open_out Sys.argv.(1) in
    List.iter (fun props -> (generate_L1 props |> to_string) ^ "\n" |> output_string oc)
        (List.init (int_of_string Sys.argv.(2))
            (fun _ -> feed_L1
                (int_of_string Sys.argv.(3))
                (int_of_string Sys.argv.(4))
                (int_of_string Sys.argv.(5))
                (float_of_string Sys.argv.(6))));
    close_out oc
