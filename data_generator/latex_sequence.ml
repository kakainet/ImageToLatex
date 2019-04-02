let random_odd bound =
    if bound <= 1 then bound else
        let rec aux () =
            let x = Random.int (bound + 1) in
            if x mod 2 = 1 then x else aux ()
        in aux ()

let random_length random bound =
    if random then random_odd bound else bound

type sequence =
    | Operator of string
    | Value of string

    | Fraction of sequence * sequence
    | Expression of sequence list
    
let operator () =
    Operator ([|"+"; "-"; "*"; "/"|].(Random.int 4))

let value bound =
    Value (Random.int bound |> string_of_int)

let fraction numerator denominator = 
    Fraction (numerator, denominator)

let expression value length =
    Expression (List.init length (fun i ->
        if i mod 2 = 0 then value () else operator ()))

let rec sequence lengths =
    match lengths with
    | (length, bound, random, _) :: [] ->
        expression (fun () -> value bound) (random_length random length)
    | (length, bound, random, chance) :: t ->
        expression (fun () ->
            if Random.float 100.0 < chance
            then fraction (sequence t) (sequence t)
            else value bound)
            (random_length random length)
    | _ -> assert false 

let rec to_string_list sequence = match sequence with
    | Operator o -> [o]
    | Value v -> [v]
    | Expression e ->
        List.flatten (List.map to_string_list e)
    | Fraction (n, d) ->
        List.flatten [["@f"]; to_string_list n; ["@f@"]; to_string_list d; ["f@"]]

let to_string sequence =
    String.concat " " (to_string_list sequence)



let () = Random.self_init ()
let () = print_endline (sequence [(1, 300, true, 56.0); (4, 100, false, 10.0); (2, 150, false, 100.0)] |> to_string)
