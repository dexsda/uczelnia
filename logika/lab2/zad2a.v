Lemma ex_falso : forall (A: Prop), False -> A.
intros.
destruct H.
Qed.

Lemma tertium_double_neg : forall A : Prop, ~~(A \/ ~A).
unfold not.
intros.
apply H.
right.
intro.
apply H.
left.
assumption.
Qed.

Proposition tertium_notnot : (forall (A : Prop), (A \/ ~A)) <-> (forall (A: Prop), (~~A -> A)).
split.
intros.
destruct (H A).
assumption.
destruct H0.
assumption.
intros.
apply H.
apply tertium_double_neg.
Qed.

Proposition tertium_notnot' : (forall (A : Prop), (A \/ ~A)) <-> (forall (A: Prop), (~~A -> A)).
split.
unfold not.
intros.
destruct (H A).
assumption.
apply ex_falso.
apply H0.
assumption.
intros.
apply H.
apply tertium_double_neg.
Qed.

(* 
Your assignment doable with 12 single-tactic lines. You may need ex_falso in addition to tertium_double_neg...
*)
