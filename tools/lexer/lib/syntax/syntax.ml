open Core_kernel

module type S = sig

    include Sexpable

    type decoupled =
        Inner of t | Outer of string

    val decouple : t -> decoupled list

    (* TODO specify signature *)
end

module Default = struct

    type t
    [@@deriving sexp]

end

module Base = (struct

    include Default

    type decoupled =
        Inner of t | Outer of string

    let decouple t = match t with
        | _ -> []

    (* TODO implement signature *)

end : S with type t = Default.t)

