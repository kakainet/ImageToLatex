(* [NOTE]: we assume that repr is positive and uppercase by default *)

open Core

open Syntax
open Operators.Unary

let instance ?(sign=true) repr =
    if sign then singular repr
    else negate (singular repr)

module Variable = struct

    let variable ?sign ?(case=true) repr =
        if case then instance ?sign repr
        else instance ?sign (String.lowercase repr)

    let random ?(sign=Random.bool ()) ?(case=Random.bool ()) () = 
        variable ~sign ~case (Random.int_incl 65 90 |> Char.of_int_exn |> String.of_char)

    let lowercase ?sign () =
        random ?sign ~case:false ()

    let uppercase ?sign () =
        random ?sign ~case:true ()

end

module Constant = struct

    let constant ?sign = instance ?sign

    module Int = struct

        let random lbnd ubnd =
            let c = Random.int_incl lbnd ubnd in
            constant ~sign:(c >= 0) (sprintf "%d" (Int.abs c))

    end

    module Float = struct

        let random lbnd ubnd prs =
            let c = Random.float_range lbnd ubnd in
            constant ~sign:(c >= 0.0) (sprintf "%.*f" prs (Float.abs c))

    end

end

