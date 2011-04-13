Parameter E : Set.
Parameter S : E -> E -> Prop.Axiom re : forall (x : E), S x x.
Axiom trs: forall (x y z : E),  S x y -> S y z -> S x z.
Axiom asym : forall (x y : E), S x y -> S y x -> x = y.

Definition smallest (x0 : E) := forall (x : E), S x0 x.
Definition minimal (x0:E) := forall (x : E), S x x0 -> x = x0.

Check smallest.
Check minimal.

Proposition smallest_minimal : forall (x : E), smallest x -> minimal x.
unfold smallest.
unfold minimal.
intros.
apply asym.
assumption.
apply (H x0).
Qed.

Proposition smallest_minimal_eq : forall (x y : E), smallest x -> minimal y -> (x=y).
unfold smallest.
unfold minimal.
intros.
apply (H0 x).
apply (H y).
Qed.

Proposition unique_smallest : forall (x y : E), smallest x -> smallest y -> (x=y).

(* Your assignment, doable with 5 single-tactic lines *)



