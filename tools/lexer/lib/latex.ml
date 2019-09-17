open Core_kernel

module type S = sig
    include Syntax.S

    val to_string : t -> string

    (* TODO remove 'include Syntax.S', specify signature *)
end

module Make = functor (SX : Syntax.S) (PR : Parser.S) -> (struct

    include SX 

    module Parser = PR (SX)

    let of_string s =
        Parser.compose s

    let to_string t =
        let (parts, length) = Parser.decompose t in
        let buffer = Buffer.create length in
        List.iter ~f:(fun x -> Buffer.add_string buffer x) parts;
        Buffer.contents buffer

end : S with type t = SX.t)

module Base = Make (Syntax.Base) (Parser.Make)

