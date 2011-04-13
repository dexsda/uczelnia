Parameter X Y : Set. 
Parameter A B : X -> Prop. 
Parameter R : X -> Y -> Prop.

Lemma univ_and : (forall x:X,A x /\ B x) <-> (forall x:X,A x)/\(forall x:X,B x).
split.
intro.
split.
intro.
apply (H x).
intro.
apply (H x).
intros.
destruct H.
split.
apply (H x).
apply (H0 x).
Qed.

Lemma exists_or : (exists x:X,A x \/ B x) <-> (exists x:X,A x) \/ (exists x:X,B x).
split.
intros.
destruct H.
destruct H.
left.
exists x.
assumption.
right.
exists x.
assumption.
intro.
destruct H.
destruct H.
exists x.
left.
assumption.
destruct H.
exists x.
right.
assumption.
Qed.

Lemma exists_forall : (exists y:Y,forall x:X,R x y) -> (forall x:X,exists y:Y,R x y).

(* Your assignmnent, doable with 5 single-tactic lines *)
