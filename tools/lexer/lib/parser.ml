open Core_kernel

module type S = functor (SX : Syntax.S) -> sig

    val compose : string -> SX.t
    val decompose : SX.t -> string list * int

end

module Search (QE : sig 
    
    type 'a t

    val empty : 'a t

    val push : 'a t -> 'a -> 'a t

    val pop : 'a t -> 'a t option * 'a option

end) = (functor (SX : Syntax.S) -> struct

    let push_all t list =
        List.fold ~init:t ~f:QE.push list

    let rec decompose_aux queue result = match QE.pop queue with 
        | (Some queue', Some element) -> (match element with
            | SX.Inner t ->
                decompose_aux (push_all queue' (SX.decouple t)) result
            | SX.Outer s -> let (list, length) = result in
                decompose_aux queue' (s :: list, length + String.length s))
        | _ -> result

    let decompose t = 
        decompose_aux (QE.push QE.empty (SX.Inner t)) ([], 0)

    let compose s =
        failwith "Not implemented yet"

end : S)

module Make : S = Search (struct 

    type 'a t = 'a list

    let empty = []

    let push t x = x :: t

    let pop t = (List.tl t, List.hd t)

end)

