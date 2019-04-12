open Syntax

module Brackets : (sig

    val brackets : string -> string -> t -> t

    val round : t -> t

    val square : t -> t

    val curly : t -> t

end)

val fraction : t -> t -> t

val sequence : t list -> t

