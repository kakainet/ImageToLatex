open Core

type t =
    | Singular of string Lazy.t
    | Plural of string Lazy.t

let to_string expr = match expr with
    | Singular x -> Lazy.force x
    | Plural x -> Lazy.force x

let apply f g expr =
    if f expr then g expr else expr

let singular repr =
    Singular (lazy repr)

let is_singular = function Singular _ -> true | _ -> false

let apply_to_singular f expr = apply is_singular f expr

let plural repr =
    Plural (lazy repr)

let is_plural = function Plural _ -> true | _ -> false

let apply_to_plural f expr = apply is_plural f expr

