let bound_int = 200;;
let bound_frac = 3;;

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
    Random.self_init();
    let rec gen output itr len end_line_code = 
        if itr = len
        then
            (end_line_code::output) |> List.rev
        else
            let element = if itr mod 2 = 0
                            then
                                (Random.int bound_int) |> string_of_int
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
    let rec gen itr output =
        if itr = length
        then
            output |> List.rev
        else
            gen 
                (itr+1) 
                (if itr mod 2 = 0 
                then
                    begin
                        if Random.bool()
                        then
                            (generate_frac nest (Random.int bound_frac))@output
                        else
                            ((Random.int bound_int) |> string_of_int)::output
                    end
                else generate_symbol::output
                )
    in gen 0 []
;;


let flat_seq length =
    nesting_seq length 0
;;

let rec stringList l =
  match l with
  | [] -> "\n"
  | h::t -> (h)^" "^(stringList t)
;;

let printList l =
  Printf.printf "%s" (stringList l)
;;

printList (nesting_seq 10 2);;