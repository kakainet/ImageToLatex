(* [NOTE]: we assume that repr is positive and uppercase by default *)

open Printf

open Syntax
open Operators.Unary

let instance ?(sign=true) repr =
    if sign then singular repr
    else negate (singular repr)

module Variable = struct

    let variable ?sign ?(case=true) repr =
        if case then instance ?sign repr
        else instance ?sign (String.lowercase_ascii repr)

    let random ?(sign=Random.bool ()) ?(case=Random.bool ()) () = 
        variable ~sign ~case (Random.int 26 + 65 |> Char.chr |> String.make 1)

    let lowercase ?sign () =
        random ?sign ~case:false ()

    let uppercase ?sign () =
        random ?sign ~case:true ()

end

module Constant = struct

    let constant ?sign = instance ?sign

    module Int = struct

        let random ?(sign=Random.bool ()) bnd =
            constant ~sign (Random.int bnd |> string_of_int)

    end

    module Float = struct

        let random ?(sign=Random.bool ()) prs bnd =
            constant ~sign (Random.float bnd |> sprintf "%.*f" prs)

    end

end

