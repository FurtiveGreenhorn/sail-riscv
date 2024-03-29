(*Generated by Lem from handwritten_support/0.11/riscv_extras_fdext.lem.*)
open HolKernel Parse boolLib bossLib;
open lem_pervasivesTheory lem_pervasives_extraTheory sail2_instr_kindsTheory sail2_valuesTheory sail2_prompt_monadTheory sail2_operators_mwordsTheory sail2_promptTheory;

val _ = numLib.prefer_num();



val _ = new_theory "riscv_extras_fdext"

(*open import Pervasives*)
(*open import Pervasives_extra*)
(*open import Sail2_instr_kinds*)
(*open import Sail2_values*)
(*open import Sail2_operators_mwords*)
(*open import Sail2_prompt_monad*)
(*open import Sail2_prompt*)

val _ = type_abbrev((*  'a *) "bitvector0" , ``:  'a words$word``);

(* stub functions emulating the C softfloat interface *)

(*val softfloat_f16_add : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_add:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f16_sub : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_sub:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f16_mul : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_mul:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f16_div : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_div:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;

(*val softfloat_f32_add : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_add:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f32_sub : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_sub:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f32_mul : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_mul:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f32_div : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_div:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f64_add : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_add:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f64_sub : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_sub:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f64_mul : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_mul:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;


(*val softfloat_f64_div : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_div:'rm words$word -> 's words$word -> 's words$word -> unit) _ _ _=  () )`;



(*val softfloat_f16_muladd : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_muladd:'rm words$word -> 's words$word -> 's words$word -> 's words$word -> unit) _ _ _ _=  () )`;


(*val softfloat_f32_muladd : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_muladd:'rm words$word -> 's words$word -> 's words$word -> 's words$word -> unit) _ _ _ _=  () )`;


(*val softfloat_f64_muladd : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_muladd:'rm words$word -> 's words$word -> 's words$word -> 's words$word -> unit) _ _ _ _=  () )`;



(*val softfloat_f16_sqrt : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_sqrt:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_sqrt : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_sqrt:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_sqrt : forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_sqrt:'rm words$word -> 's words$word -> unit) _ _=  () )`;



(*val softfloat_f16_to_i32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_i32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_to_ui32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_ui32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i32_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i32_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui32_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui32_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_to_i64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_i64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_to_ui64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_ui64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i64_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i64_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui64_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui64_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;



(*val softfloat_f32_to_i32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_i32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_to_ui32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_ui32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i32_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i32_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui32_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui32_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_to_i64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_i64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_to_ui64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_ui64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i64_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i64_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui64_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui64_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;



(*val softfloat_f64_to_i32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_i32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_to_ui32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_ui32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i32_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i32_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui32_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui32_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_to_i64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_i64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_to_ui64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_ui64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_i64_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_i64_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_ui64_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_ui64_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;



(*val softfloat_f16_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_to_f64: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_f64:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_to_f16: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_f16:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_to_f32: forall 'rm 's. Size 'rm, Size 's => bitvector 'rm -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_to_f32:'rm words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_lt : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_lt:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_le : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_le:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f16_eq : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f16_eq:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_lt : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_lt:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_le : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_le:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f32_eq : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f32_eq:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_lt : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_lt:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_le : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_le:'s words$word -> 's words$word -> unit) _ _=  () )`;


(*val softfloat_f64_eq : forall 's. Size 's => bitvector 's -> bitvector 's -> unit*)
val _ = Define `
 ((softfloat_f64_eq:'s words$word -> 's words$word -> unit) _ _=  () )`;

val _ = export_theory()

