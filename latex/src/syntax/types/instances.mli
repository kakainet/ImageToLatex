open Syntax

val instance : ?sign:bool -> string -> t

module Variable : (sig

    val variable : ?sign:bool -> ?case:bool -> string -> t

    val random : ?sign:bool -> ?case:bool -> unit -> t

    val lowercase : ?sign:bool -> unit -> t

    val uppercase : ?sign:bool -> unit -> t

end)

module Constant : (sig

    val constant : ?sign:bool -> string -> t

    module Int : (sig

        val random : ?sign:bool -> int -> t

    end)

    module Float : (sig
        
        val random : ?sign:bool -> int -> float -> t

    end)

end)

