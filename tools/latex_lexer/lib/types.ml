open Core

(* [note]: polimorphic variants may be replaced in the future 
    by standard variants aggregate type for performance reasons, for example:

    module Text : Sexpable = struct

        type t =
            | String of ...

        let to_string ...
        let to_buffer ...
        let length ...

    end

    ...

    type t =
        | Text of Text.t
        | ...
*)

type number = [
    `Float of float * int | `Int of int
]
[@@deriving sexp]

type text = [
    `Char of char | `String of string
]
[@@deriving sexp]

type t = [ number | text ]
[@@deriving sexp]


let escape t = match t with (* todo *)
    | `Char value -> `String ""
    | `String value -> `String ""

let to_string t = match t with
    | `Float (value, precision) ->
        Helpers.float_to_string value precision
    | `Int value ->
        Int.to_string value
    | `Char value ->
        Char.to_string value
    | `String value -> value

let to_buffer buffer t = match t with
    | `Char value -> Buffer.add_char buffer value
    | `String value -> Buffer.add_string buffer value
    | _ -> Buffer.add_string buffer (to_string t)

let length t = match t with
    | `Char (_ : char) -> 1
    | `String value -> String.length value
    | `Int value -> Helpers.int_length value
    | `Float (value, precision) -> Helpers.float_length value precision

