open OUnit2

let suite =
    "suite" >::: [
        "test1" >:: (fun _ -> print_endline "test1");
        "test2" >:: (fun _ -> print_endline "test2")
    ]

let () =
    run_test_tt_main suite