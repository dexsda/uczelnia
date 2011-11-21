let head ls = match ls with
        | [] -> invalid_arg "to bez sensu!"
        | h::t -> h

let tail ls = match ls with
        | [] -> invalid_arg "to bez sensu!"
        | h::t -> t

let rec str ls = match ls with
        | [] -> []
        | h::t -> string_of_int(h)::str(t)

let rec sum ls = match ls with
        | [] -> 0
        | h::t -> h + sum(t)

let rec negatives ls = match ls with
        | [] -> []
        | h::t -> if h<0 then h::negatives(t) else negatives(t)

let rec min ls = match ls with
        | [] -> invalid_arg "what"
        | [a] -> a
        | h::t -> let minim = min(t) in if h < minim then h else minim

let rec capitalized ls = match ls with
        | [] -> []
        | h::t -> if String.capitalize(h) <> h then capitalized(t) else h::capitalized(t)

let rec firsts ls = match ls with
        | [] -> []
        | h::t -> match h with
                | (a, b) -> a::firsts(t)

let rec append ls1 ls2 = match ls1 with
        | [] -> ls2
        | h::t -> h::(append t ls2)
