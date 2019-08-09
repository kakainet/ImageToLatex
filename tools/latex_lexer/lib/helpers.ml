open Core

(* [note]: this module contains auxiliary code, independent from other modules,
    which could reduce readability if not separated, as is rather focused on performance in expense of clarity,
*)

let float_to_string value precision =
    assert (Int.is_positive precision);
    sprintf "%.*f" precision value

let int_log10 value =
    assert (Int.is_positive value);
    Float.log10 (Float.of_int value) |> Int.of_float_unchecked

let length_table = Array.init (int_log10 Int.max_value + 1)
    ~f:(fun exponent -> if exponent <> 0 then Int.pow 10 exponent else 0)

let (table_left, table_right) = (0, Array.length length_table - 1)

let rec int_length_aux ?(left=table_left) ?(right=table_right) value = 
    if left < right then
        let middle = (left + right) lsr 1 in
        if length_table.(middle) <= value then
            int_length_aux ~left:(middle + 1) ~right value
        else
            int_length_aux ~left ~right:middle value
    else left

let rec int_length value = match value with
    | _ when value < 0 -> int_length (Int.abs value) + 1
    | _ when value > length_table.(table_right) -> table_right + 1
    | _ -> int_length_aux value

let rec float_length value precision = 
    assert (Float.is_finite value);
    assert (Int.is_positive precision);
    (*int_length (Int.of_float value) + precision + 1*)
    if value < 0. then float_length (Float.abs value) precision + 1
    else (Float.log10 value |> Int.of_float) + precision + 2 

(* [todo]: Int.is_positive ... and Pervasives, +, succ ... performance comparison,
    change int_length match order
   [todo]: bench float_length with size check against Int.max_value
*)