// pipeline api
val createInstrForStageInfo = impure {c: "createInstrForStageInfo"} : unit -> unit
val sendInstToPipeline = impure {c: "sendInstToPipeline"} : unit -> unit
val set_rs = pure {c: "set_rs"} : (bits(5), bits(5)) -> unit
val set_rs1 = pure {c: "set_rs1"} : bits(5) -> unit
val set_rs2 = pure {c: "set_rs2"} : bits(5) -> unit
val set_rd = pure {c: "set_rd"} : bits(5) -> unit
val set_taken = pure {c: "set_taken"} : bool -> unit
