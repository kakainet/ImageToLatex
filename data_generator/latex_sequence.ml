let bound = 200;;

let generate_symbol =
    if Random.bool() 
    then
        if Random.bool() 
        then
            "+"
        else 
            "-"
    else
        "*"
;;

let generate_frac nest len =
    let rec gen output itr len end_line_code = 
        if itr = len
        then
            (end_line_code::output) |> List.rev
        else
            let element = if itr mod 2 = 0
                            then
                                (Random.int bound) |> string_of_int
                            else
                                generate_symbol
            in
            gen (element::output) (itr+1) (len) end_line_code
    in 
    let len_nominator = Random.int (len-1) + 1 in
    let len_denominator = Random.int (len-1) + 1 in
    "@f"::(gen [] 0 len_nominator "@f@") @ (gen [] 0 len_denominator "f@")
;;

let nesting_seq length nest =
    []
;;


let flat_seq length =
    nesting_seq length 0
;;