
Module TraditionalLogic.

Parameter Term : Type.

Parameter humans : Term.
Parameter Greeks : Term.
Parameter mortal : Term.

Record Quantity : Type :=
  universal : Quantity
| particular : Quantity.

Record Quality : Type :=
  affirmative : Quality
| negative : Quality.

Record CategoricalProposition : Type := 
cp {  
  quantity : Quantity;
  quality : Quality;
  subject : Term; 
  object : Term
}.

Definition HumansMortal : CategoricalProposition :=
  cp universal affirmative humans mortal.

Notation "'All' subject 'are' object " := 
   (cp universal affirmative subject object) (at level 50).

Notation "'No' subject 'are' object " := 
   (cp universal negative subject object) (at level 50).

Notation "'Some' subject 'are' object " := 
   (cp particular affirmative subject object) (at level 50).

Notation "'Some' subject 'are' 'not' object " := 
   (cp particular negative subject object) (at level 50).

Definition HumansMortal2 : CategoricalProposition :=
   All humans are mortal.

Parameter cats : Term.

Definition NoExample: CategoricalProposition := 
   No Greeks are cats.

Parameter animals : Term.

Definition SomeExample: CategoricalProposition := 
   Some animals are cats.

Parameter brave : Term.

Definition SomeNotExample: CategoricalProposition := 
   Some cats are not brave.

Parameter holds : CategoricalProposition -> Prop.

Axiom HumansMortality: holds (All humans are mortal).

Axiom GreeksHumanity: holds (All Greeks are humans).

Lemma HumansMortality2: holds (All humans are mortal).

Proof.
apply HumansMortality.
Qed.

Parameter non: Term -> Term.

Definition immortal: Term := non mortal.

Definition SomeHumansAreImmortal : CategoricalProposition := 
   Some humans are immortal.

Axiom NonNon: forall t, non (non t) = t.

Lemma HumansNonImmortality: holds (All humans are (non immortal)).

Proof.
unfold immortal.  
rewrite NonNon.
apply HumansMortality.
Qed.

Definition convert: CategoricalProposition -> CategoricalProposition := 
  fun x : CategoricalProposition => match x with
    | cp quantity quality subject object => 
      cp quantity quality object subject
  end.

Definition AllGreeksHumansConverted  := 
      convert (All Greeks are humans).

Axiom ConvE1:
   forall subject object,
   holds (convert (Some subject are object))
   ->
   holds (Some subject are object).

Axiom ConvE2:
   forall subject object,
   holds (convert (No subject are object))
   ->
   holds (No subject are object).

Axiom SomeAnimalsAreCats : holds (Some animals are cats).

Lemma SomeCatsAreAnimals : holds (Some cats are animals).

Proof.
assert (holds (convert (Some cats are animals))
        -> holds (Some cats are animals)).
apply ConvE1.
apply H.
unfold convert.
apply SomeAnimalsAreCats.
Qed.

Axiom NoGreeksAreCats : holds (No Greeks are cats).

Lemma NoCatsAreGreeks : holds (No cats are Greeks).

Proof.
assert (holds (convert (No cats are Greeks)) 
        -> holds (No cats are Greeks)).
apply ConvE2.
apply H.
unfold convert.
apply NoGreeksAreCats.
Qed.

Ltac eliminateConversion1 :=
  match goal with 
   | [ |- holds (cp ?quantity ?quality ?subject ?object)] 
     => 
     assert (holds (convert (cp quantity quality subject object))
             -> 
             holds (cp quantity quality subject
             object))
        as ConvertPremise;
     try solve [eapply ConvE1;
                eauto];
     apply ConvertPremise;
     clear ConvertPremise; 
     unfold convert
  end.

Ltac eliminateConversion2 :=
  match goal with 
   | [ |- holds (cp ?quantity ?quality ?subject ?object)] 
     => 
     assert (holds (convert (cp quantity quality subject object))
             -> 
             holds (cp quantity quality subject
             object))
        as ConvertPremise;
     try solve [eapply ConvE2;
                eauto];
     apply ConvertPremise;
     clear ConvertPremise; 
     unfold convert
  end.

Lemma NoCatsAreGreeks2 : holds (No cats are Greeks).

Proof.
eliminateConversion2.
apply NoGreeksAreCats.
Qed.

Definition contrapose: CategoricalProposition -> 
                       CategoricalProposition := 
  fun x : CategoricalProposition => match x with
    | cp quantity quality subject object => 
      cp quantity quality (non object) (non subject)
  end.

Definition AllGreeksHumansContraposed  := 
      contrapose (All Greeks are humans).

Axiom ContrE1:
   forall subject object,
   holds (contrapose (All subject are object))
   ->
   holds (All subject are object).

Axiom ContrE2:
   forall subject object,
   holds (contrapose (Some subject are not object))
   ->
   holds (Some subject are not object).

Axiom AllCatsAreAnimals : holds (All cats are animals).

Lemma AllNonAnimalsAreNonCats : holds (All (non animals) are (non cats)).

(* Zadanie 1: Uzupelnij ponizszy dowod. 10 punktow *)
						 
Proof.
apply ContrE1.
unfold contrapose.
rewrite NonNon.
rewrite NonNon.
apply AllCatsAreAnimals.
Qed.

Ltac eliminateContraposition1 :=
  match goal with 
   | [ |- holds (cp ?quantity ?quality ?subject ?object)] 
     => 
        assert (holds (contrapose (cp quantity quality subject object))
                -> holds (cp quantity quality subject object)) as ContraposePremise;
        try solve [eapply ContrE1;
                   eauto];
        apply ContraposePremise;
        clear ContraposePremise;
        unfold contrapose
  end.

Ltac eliminateContraposition2 :=
  match goal with 
   | [ |- holds (cp ?quantity ?quality ?subject ?object)] 
     => 
        assert (holds (contrapose (cp quantity quality subject object))
                -> holds (cp quantity quality subject object)) as ContraposePremise;
        try solve [eapply ContrE2;
                   eauto];
        apply ContraposePremise;
        clear ContraposePremise;
        unfold contrapose
  end.

Definition complement: Quality -> Quality :=
  fun x : Quality => match x with
    | affirmative => negative
    | negative => affirmative
  end.

Definition obvert: CategoricalProposition -> CategoricalProposition := 
  fun x : CategoricalProposition => match x with
    | cp quantity quality subject object 
      => cp quantity (complement quality) subject (non object)  
  end.

Axiom ObvE : 
   forall catprop, holds (obvert catprop) -> holds catprop.

Parameter vegetarians : Term.

Axiom SomeHumansVegetarians : holds (Some humans are vegetarians).

Lemma SomeHumansAreNotNonVegetarians : 
   holds (Some humans are not (non vegetarians)).

Proof.
assert (holds (obvert (Some humans are not (non vegetarians)))
        -> holds (Some humans are not (non vegetarians))).
apply ObvE.
apply H.
unfold obvert.
unfold complement.
rewrite NonNon.
apply SomeHumansVegetarians.
Qed.

Ltac eliminateObversion :=
  match goal with 
   | [ |- holds (cp ?quantity ?quality ?subject ?object)] 
     => 
       assert (holds (obvert (cp quantity quality subject object))
               -> holds (cp quantity quality subject object)) as ObvertPremise;
       try solve [eapply ObvE; eauto];
       apply ObvertPremise;  
       clear ObvertPremise;
       unfold obvert;
       unfold complement
  end.

Lemma SomeHumansAreNotNonVegetarians2 : 
   holds (Some humans are not (non vegetarians)).

Proof.
eliminateObversion.
rewrite NonNon.
apply SomeHumansVegetarians.
Qed.

Lemma SomeNon: forall subject object, 
     holds (Some non object are non subject)
    -> holds (Some non subject are not object).
 
Proof.
intros.
eliminateObversion.
eliminateConversion1.
Qed.

Lemma AllNonNon : forall subject object,
   holds (All non subject are non object) <-> holds (All object are subject).

Proof.
intro.
split.
intro.
eliminateObversion.
eliminateConversion2.
eliminateObversion.
intro.
eliminateObversion.
eliminateConversion2.
eliminateObversion.
rewrite NonNon.
rewrite NonNon.
apply H.
Qed.

Axiom Barbara : forall major minor middle, 
    holds (All middle are major)
 /\ holds (All minor are middle)
 -> holds (All minor are major).

Lemma GreeksMortality : holds (All Greeks are mortal).

Proof.
apply Barbara with (middle := humans).
split.
apply HumansMortality.
apply GreeksHumanity.
Qed.

Parameter ducks : Term.
Parameter things_that_waltz : Term.
Parameter officers : Term.
Parameter my_poultry : Term.

Lemma No_Officers_Are_My_Poulty : 
   holds (No ducks are things_that_waltz) /\
   holds (No officers are non things_that_waltz) /\
   holds (All my_poultry are ducks)
   -> 
   holds (No officers are my_poultry).

Proof.
intro.
destruct H.
destruct H0.
eliminateObversion.
apply Barbara with (middle := things_that_waltz).
split.
apply Barbara with (middle := non ducks).
split.
eliminateContraposition1.
rewrite NonNon.
rewrite NonNon.
apply H1.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
eliminateObversion.
Qed.

Parameter babies : Term.
Parameter logical_persons : Term.
Parameter despised_persons : Term.
Parameter persons_who_can_manage_a_crocodile : Term.

Lemma No_baby_can_manage_a_crocodile : 
holds (All babies are non logical_persons) /\
holds (No persons_who_can_manage_a_crocodile are despised_persons) /\
holds (All non logical_persons are despised_persons) 
->
holds (No babies are persons_who_can_manage_a_crocodile).

(* Zadanie 2: Uzupelnij ponizszy dowod. 10 punktow *)
  
Proof.
intro.
destruct H.
destruct H0.
eliminateObversion.
apply Barbara with (middle := non logical_persons).
split.
apply Barbara with (middle := despised_persons).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
apply H1.
apply H.
Qed.

Parameter boys_under_12 : Term.
Parameter boys_admitted_as_boarders : Term.
Parameter industrious_boys : Term.
Parameter red_haired_boys : Term.
Parameter boys_who_learn_Greek : Term.

Lemma All_Boys_Who_learn_Greek_are_red_haired :
   holds (No boys_under_12 are boys_admitted_as_boarders) /\
   holds (All industrious_boys are red_haired_boys) /\
   holds (No non boys_admitted_as_boarders are boys_who_learn_Greek) /\
   holds (No non boys_under_12 are non industrious_boys)
   ->
   holds (All boys_who_learn_Greek are red_haired_boys).

(* Zadanie 3: Uzupelnij
 ponizszy dowod. 10 punktow *)
     
Proof.
intro.
destruct H.
destruct H0.
destruct H1.
apply Barbara with (middle := industrious_boys).
split.
apply H0.
apply Barbara with (middle := non boys_under_12).
split.
eliminateObversion.
apply Barbara with (middle := boys_admitted_as_boarders).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
eliminateObversion.
eliminateConversion2.
Qed.

Parameter interesting_poems : Term.
Parameter poems_that_are_popular_among_people_of_real_taste : Term.
Parameter modern_poems : Term.
Parameter affected_poems : Term.
Parameter your_poems : Term.
Parameter poems_on_the_subject_of_soap_bubbles : Term.

Lemma Your_Poems_Are_Not_Interesting : 
   holds (No interesting_poems are non poems_that_are_popular_among_people_of_real_taste) /\
   holds (No modern_poems are non affected_poems) /\
   holds (All your_poems are poems_on_the_subject_of_soap_bubbles) /\
   holds (No affected_poems are poems_that_are_popular_among_people_of_real_taste) /\
   holds (No non modern_poems are poems_on_the_subject_of_soap_bubbles)
   ->
   holds (All your_poems are non interesting_poems).

(* Zadanie 4: Uzupelnij ponizszy dowod. 10 punktow *)
     
Proof.
intros.
destruct H.
destruct H0.
destruct H1.
destruct H2.
apply Barbara with (middle := poems_on_the_subject_of_soap_bubbles).
split.
apply Barbara with (middle:= modern_poems).
split.
apply Barbara with (middle := affected_poems).
split.
apply Barbara with (middle := non poems_that_are_popular_among_people_of_real_taste).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
eliminateObversion.
rewrite NonNon.
apply H2.
eliminateObversion.
eliminateObversion.
eliminateConversion2.
apply H1.
Qed.

Parameter animals_in_this_house : Term.
Parameter things_suitable_for_a_pet : Term.
Parameter animals_that_love_to_gaze_at_the_moon : Term.
Parameter things_that_I_avoid : Term.
Parameter animals_that_I_detest : Term.
Parameter carnivorous_animals : Term.
Parameter things_that_prowl_at_night : Term.
Parameter things_that_kill_mice : Term.
Parameter animals_that_take_to_me : Term.
Parameter kangaroos : Term.

Lemma I_avoid_kangoroos : 
holds (All animals_in_this_house are cats) /\
holds (All animals_that_love_to_gaze_at_the_moon are things_suitable_for_a_pet) /\
holds (All animals_that_I_detest are things_that_I_avoid) /\
holds (No carnivorous_animals are non things_that_prowl_at_night) /\
holds (No cats are non things_that_kill_mice) /\
holds (No animals_that_take_to_me are non animals_in_this_house) /\
holds (All kangaroos are non things_suitable_for_a_pet) /\
holds (No things_that_kill_mice are (non carnivorous_animals)) /\
holds (All non animals_that_take_to_me are animals_that_I_detest) /\
holds (All things_that_prowl_at_night are animals_that_love_to_gaze_at_the_moon)
->
holds (All kangaroos are things_that_I_avoid).

(* Zadanie 5: Uzupelnij ponizszy dowod. 10 punktow *)
  
Proof.
intro.
destruct H.
destruct H0.
destruct H1.
destruct H2.
destruct H3.
destruct H4.
destruct H5.
destruct H6.
destruct H7.
apply Barbara with (middle := animals_that_I_detest).
split.
apply H1.
apply Barbara with (middle := non animals_that_take_to_me).
split.
apply H7.
apply Barbara with (middle := non animals_in_this_house).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
apply Barbara with (middle := non cats).
split.
eliminateContraposition1.
rewrite NonNon.
rewrite NonNon.
apply H.
apply Barbara with (middle := non things_that_kill_mice).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
apply Barbara with (middle := non carnivorous_animals).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
apply Barbara with (middle := non things_that_prowl_at_night).
split.
eliminateObversion.
rewrite NonNon.
eliminateConversion2.
apply Barbara with (middle := non animals_that_love_to_gaze_at_the_moon).
split.
eliminateContraposition1.
rewrite NonNon.
rewrite NonNon.
apply H8.
apply Barbara with (middle := non things_suitable_for_a_pet).
split.
eliminateContraposition1.
rewrite NonNon.
rewrite NonNon.
apply H0.
apply H5.
Qed.

