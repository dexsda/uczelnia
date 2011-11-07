#!/usr/bin/perl -w
%ops = @name = qw#+ Add * Mult / Div - Sub#; eval 'sub '.$name[2*$_+1].' ($$) {[shift,"'.$name[2*$_].'",shift]}' for 0..
$#name/2; sub Minus {["-",shift]} sub myeval { my $_=pop;ref()?@$_[0] eq"-"?eval'- myeval(@$_[1])':eval'myeval(@$_[0])'.
@$_[1].'myeval(@$_[2])':$_} sub simplify_expr { my $_=pop; if(ref() && @$_[0] eq "-"){ @$_[1]=simplify_expr(@$_[1]); $_;
} elsif(ref()) { my ($a,$b)=(simplify_expr(@$_[0]),simplify_expr(@$_[2])); if((@$_[1] eq'+' && $a==0)||(@$_[1] eq '*' &&
$a==1)){$b} elsif ((@$_[1] eq '+' && $b==0)||(@$_[1] eq '*' && $b==1)){$a} else {[$a,@$_[1],$b]} } else{$_} } sub string
{ my $_=pop; ref()?@$_[0] eq "-"?"Minus(".string(@$_[1]).")":$ops{@$_[1]}."(".string(@$_[0]).", ".string(@$_[2]).")":$_}

$ref=eval"Minus(Add(Div(Mult(2,Sub(10,Add(0,Mult(1,3)))),3),1))";print myeval($ref)."\n".string(simplify_expr$ref)."\n";

while(<>){if($_ ne""){$ref=eval;if($@){warn"err"}else{print myeval($ref)."\n"; print string(simplify_expr($ref))."\n"}}}

