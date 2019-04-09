open Syntax

module Unary : (sig

    val unary : string -> t

    val negate : ?brackets:(t -> t) -> t -> t

end)

module Binary : (sig
    
    val binary : string -> t

    val random : ?pool:(string array) -> unit -> t

end)

