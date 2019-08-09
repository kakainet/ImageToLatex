open Core
open OUnit2

open Latex


let assert_raises_no_exn f =
    try (f () |> ignore; assert false)
    with _ -> ()

module Safe = struct

    let int_length value =
        Int.to_string value |> String.length

    let float_length value precision =
        Helpers.float_to_string value precision |> String.length

end

(* [todo]: less specific tests *)

let tests = [
    "int_length" >::: [
        "max_value" >:: (fun _ ->
            assert_equal (Helpers.int_length Int.max_value)
                         (Safe.int_length Int.max_value));
        "neg_max_value" >:: (fun _ ->
            let neg_max_value = Int.neg Int.max_value in
            assert_equal (Helpers.int_length neg_max_value)
                         (Safe.int_length neg_max_value))
    ];
    "float_length" >::: [
        "max_value" >:: (fun _ ->
            assert_equal (Helpers.float_length Float.max_finite_value 5)
                         (Safe.float_length Float.max_finite_value 5));
        "neg_max_value" >:: (fun _ ->
            let neg_max_value = Float.neg Float.max_finite_value in
            assert_equal (Helpers.float_length neg_max_value 5)
                         (Safe.float_length neg_max_value 5));
        "infinity" >:: (fun _ ->
            assert_raises_no_exn (fun () ->
                Helpers.float_length Float.infinity 5));
        "neg_infinity" >:: (fun _ -> 
            assert_raises_no_exn (fun () ->
                Helpers.float_length Float.neg_infinity 5));
        "nan" >:: (fun _ ->
            assert_raises_no_exn (fun () ->
                Helpers.float_length Float.nan 5));
    ]
]


let () =
    run_test_tt_main ("tests" >::: tests)

