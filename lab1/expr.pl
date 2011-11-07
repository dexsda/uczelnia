#!/usr/bin/perl -w
%ops = qw#+ Add * Mult / Div - Sub#; #robimy sobie hash do latwego wypisywania
sub Add ($$) { return [shift,"+",shift]}; #tworzenie listy zawierajacej referencje do lewego syna (lub wartosc, jezeli to lisc)
sub Mult ($$) { return [shift,"*",shift]}; #latwo w razie czego opakowac obiektowo
sub Div ($$) { return [shift,"/",shift]};
sub Sub ($$) { return [shift,"-",shift]};
sub Minus {["-",shift]}; #dla minusa specjalny rodzaj listy
sub myeval { #nazwa eval juz zajeta, ponadto znacznie ulatwia parsing do drzewa
	my $_=pop; #argument funkcji
	if(ref() && @$_[0] eq "-"){ #jezeli referencja i lista Minusa
		return eval '- myeval(@$_[1])' #ewaluujemy przy uzyciu zwyklej arytmetyki perla
	} elsif (ref()) { #jezeli referencja i normalny przypadek
		return eval'myeval(@$_[0])'.@$_[1].'myeval(@$_[2])' #znowu arytmetyka perla
	} else { 
		return $_
	}
}	
sub simplify_expr {
	my $_=pop;
	if(ref() && @$_[0] eq "-"){ #referencja Minusowa
		@$_[1]=simplify_expr(@$_[1]); #upraszczamy podwyrazenie
		return $_;
	} elsif(ref()) { #referencja
		my ($a,$b)=(simplify_expr(@$_[0]),simplify_expr(@$_[2])); #upraszczamy podwyrazenia
		if(@$_[1] eq '+' && $a==0){# 0+e
			return $b; # e
		} elsif(@$_[1] eq '*' && $a==1){# 1*e
			return $b; # e
		} elsif (@$_[1] eq '+' && $b==0){# e+0
			return $a; # e
		} elsif (@$_[1] eq '*' && $b==1){# e*1
			return $a; # e
		} else { #normalny przypadek
			return [$a,@$_[1],$b] #zwracamy poddrzewo
		} 
	} else { #wartosc
		return $_
	} 
} 
sub string { 
	my $_=pop;
	if(ref() && @$_[0] eq "-") {
		return "Minus(".string(@$_[1]).")"
	} elsif (ref()){
		return $ops{@$_[1]}."(".string(@$_[0]).", ".string(@$_[2]).")"
	} else {
		return $_
	}
}
print "Test dla Minus(Add(Div(Mult(2,Sub(10,Add(0,Mult(2,3)))),3),1))...\n";
$ref = eval "Minus(Add(Div(Mult(2,Sub(10,Add(0,Mult(2,3)))),3),1))";
print myeval($ref)."\n";
print string(simplify_expr($ref))."\n";

print "Podaj wyrazenie...\n>";
while($line = <>){
	last if !$line;
	$ref=eval $line;
	if($@){
		warn "error parsing input!"
	} else {
		print "Wynik: ".myeval($ref)."\n";
		print "Uproszczony: ".string(simplify_expr($ref))."\n>";
	}
}
