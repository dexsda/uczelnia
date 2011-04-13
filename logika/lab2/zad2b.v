Parameter G : Set.
Parameter e : G.
Parameter m : G -> G -> G.
Parameter i : G -> G.

Axiom left_e : forall (x : G), m e x = x.
Axiom right_e : forall (x : G), m x e = x.
Axiom left_i : forall (x : G), (m (i x) x) = e.
Axiom right_i : forall (x : G), (m x (i x)) = e.
Axiom assoc : forall (x y z : G), (m x (m y z)) = (m (m x y) z).

Lemma left_mul_eq : forall (a x y: G), (x = y) -> (m a x) = (m a y).
intros b u v.
intro Heq.
(* 
We name the items introduced in the context here, otherwise a single parameterless "intros" would do the trick.
But then the paramter of the next "rewrite" would need to be adjusted to the system-generated name, here typically
H since there's only one hypothesis  
*)
rewrite Heq.
reflexivity.
Qed.

Lemma left_cancel : forall (a x y: G), (m a x) = (m a y) -> (x = y).
intros.
apply (left_mul_eq (i a)) in H.
rewrite assoc in H.
rewrite assoc in H.
rewrite left_i in H.
rewrite left_e in H.
rewrite left_e in H.
assumption.
Qed.

Proposition prod_mul_inv : forall (x y : G), (i (m x y)) = (m (i y) (i x)).
intros.
assert (m (m x y) (i (m x y)) = m (m x y) (m (i y) (i x))). 
rewrite right_i.
rewrite assoc.
replace (m (m x y) (i y)) with (m x (m y (i y))).
rewrite right_i.
rewrite right_e.
rewrite right_i.
reflexivity.
rewrite right_i.
rewrite right_e.
rewrite <- assoc.
rewrite right_i.
rewrite right_e.
reflexivity.
apply left_cancel in H.
assumption.
Qed.

Proposition prod_mul_inv' : forall (x y : G), (i (m x y)) = (m (i y) (i x)).
intros.
cut (m (m x y) (i (m x y)) = m (m x y) (m (i y) (i x))).
apply (left_cancel (m x y) (i (m x y)) (m (i y) (i x))).
rewrite right_i.
rewrite assoc.
replace (m (m x y) (i y)) with (m x (m y (i y))).
rewrite right_i.
rewrite right_e.
rewrite right_i.
reflexivity.
rewrite right_i.
rewrite right_e.
rewrite <- assoc.
rewrite right_i.
rewrite right_e.
reflexivity.
Qed.

Lemma right_mul_eq : forall (a x y: G), (x = y) -> (m x a) = (m y a).

Proof.
intros.
rewrite H.
reflexivity.
Qed.

Lemma right_cancel : forall (x y a: G), (m x a) = (m y a) -> (x = y).

Proof.
intros.
apply (right_mul_eq (i a)) in H.
rewrite <- assoc in H.
rewrite <- assoc in H.
rewrite right_i in H.
rewrite right_e in H.
rewrite right_e in H.
assumption.
Qed.

(* Your assignment, doable in 8 single-tactic lines *)




