(* W miejsce "admit" nalezy uzupelnic brakujace dowody. 
 Nie wolno stosowac taktyk "admit", "firstorder" oraz "intuition"!
 Tym razem wolno stosowac "trivial", "auto", "tauto", "congruence" i "subst".
 Mozna tez (a nawet nalezy!) stosowac "apply" lub "rewrite" w stosunku do udowodnionych juz czesci zadania.
*)

Module Induction.

(* Liczby naturalne *)
Inductive Nat : Type :=
  | Zero : Nat
  | Succ : forall n : Nat, Nat. (* Succ to skrot od "sukcesor" *)

(* Rekursywna definicja dodawania. *)
Fixpoint Add (n1 n2 : Nat) : Nat :=
  match (n1, n2) with
   | (Zero, n2') => n2' (* 0 + n2 = n2 *)
   | (Succ n1', n2') => Succ (Add n1' n2') (* (Succ n1) + n2 = Succ (n1 + n2) *)
  end.

(* Przykladowe lematy, czesciowo zapozyczone z tutoriala... *)
Lemma Add_n1_Zero: forall n1,
  Add n1 Zero = n1.
Proof.
  induction n1; trivial.
  simpl; rewrite IHn1.
  trivial.
Qed.

Lemma Add_n1_Succ_n2: forall n1 n2,
  Add n1 (Succ n2) = Succ(Add n1 n2).
Proof.
  intros; induction n1; auto.
  simpl; rewrite IHn1.
  trivial.
Qed.

Lemma Add_Comm: forall n1 n2,
  Add n1 n2 = Add n2 n1.
Proof.
  intros; induction n1; simpl.
  rewrite Add_n1_Zero; trivial.
  simpl; rewrite Add_n1_Succ_n2.
  congruence.
Qed.

(* 6 punktow -- uzupelnij ponizszy dowod *)
Lemma Add_Assoc: forall n1 n2 n3,
  Add n1 (Add n2 n3) = Add (Add n1 n2) n3.
Proof.
intros; induction n1; trivial.
simpl; rewrite IHn1.
congruence.
Qed.

(* Rekursywna definicja mnozenia *)
Fixpoint Mult (n1 n2 : Nat) : Nat :=
  match (n1, n2) with
   | (Zero, n2') => Zero
   | (Succ n1', n2') => Add (Mult n1' n2') n2'
  end.

(* 6 punktow *)
Lemma Mult_Succ: forall n1 n2,
  Mult n1 (Succ n2) = Add n1 (Mult n1 n2).
Proof.
intros; induction n1; trivial.
simpl; rewrite IHn1.
rewrite Add_n1_Succ_n2.
rewrite Add_Assoc.
congruence.
Qed.

(* 8 punktow *)
Lemma Mult_Comm: forall n1 n2,
  Mult n1 n2 = Mult n2 n1.
Proof.
intros; induction n1; simpl.
induction n2; trivial.
simpl; rewrite <- IHn2; trivial.
rewrite Mult_Succ; rewrite Add_Comm.
rewrite IHn1.
congruence.
Qed.

(* Rekursywna definicja odejmowania -- warto sie zastanowic, czemu jest taka skomplikowana... *)
Fixpoint Sub (n1 n2 : Nat) : Nat :=
  match (n1, n2) with
   | (Zero, n2') => Zero
   | (n1', Zero) => n1'
   | (Succ n1', Succ n2') => Sub n1' n2'
  end.

(* 4 punkty *)
Lemma Sub_Prop1: forall n1, 
  Sub n1 Zero = n1.
Proof.
intros; induction n1; trivial.
Qed.

(* 2 punkty *)
Lemma Sub_Prop2: forall n2,
  Sub Zero n2 = Zero.
Proof.
intros; induction n2; trivial.
Qed.

(* (* 16 punktow *)
(* Wskazowka: Nalezy sie upewnic, ze zalozenie indukcyjne jest wlasciwie dobrane, bo inaczej dowod bedzie
 naprawde bolesny. Jeden z mozliwych sposobow to zastosowanie taktyki "revert" *)
Lemma Sub_Prop3: forall n3 n2 n1,
  Sub n1 n2 = n3 ->
  (n3 = Zero) \/ (Add n2 n3 = n1).
Proof.
  admit.
Qed.

(* 8 punktow *)
(* Jesli jako zalozenie pojawi sie cos takiego jak "H: Succ n = Zero", to otrzymalismy sprzecznosc. 
 Nalezy powiadomic o tym Coq stosujac "inversion H". Mozna tez zastosowac taktyke "discriminate" *)
Lemma Sub_Prop4: forall n1 n2 n3,
  Add n1 n2 = n3 ->
  Sub n3 n1 = n2.
Proof.
  admit.
Qed.*)

End Induction.
