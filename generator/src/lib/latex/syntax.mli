type t

val to_string : t -> string

val singular : string -> t

val is_singular : t -> bool

val apply_to_singular : (t -> t) -> t -> t

val plural : string -> t

val is_plural : t -> bool

val apply_to_plural : (t -> t) -> t -> t

