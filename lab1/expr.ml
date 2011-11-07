type expr = 
        | Int of int
        | Float of float
        | Add of expr * expr
        | Sub of expr * expr
        | Mult of expr * expr
        | Div of expr * expr
        | Minus of expr

let rec eval expr = match expr with
        | Int a -> float a
        | Float a -> a
        | Add (a,b) -> (eval a)+.(eval b)
        | Sub (a,b) -> (eval a)-.(eval b)
        | Mult (a,b) -> (eval a)*.(eval b)
        | Div (a,b) -> (eval a)/.(eval b)
        | Minus a -> -.(eval a)
let rec simplify expr = match expr with
        | Int a -> Float (float a)
        | Float a -> Float a
        | Minus a -> if (simplify a)=Float 0. then
                Float 0. else Minus (simplify a)
        | Div (a,b) -> if(simplify b)=Float 1. then
                simplify a else Div (simplify a, simplify b)
        | Add (a,b) -> if (simplify a)=Float 0. then 
                simplify b else if (simplify b)=Float 0. then
                simplify a else Add (simplify a,simplify b)
        | Sub (a,b) -> if (simplify a)=Float 0. then 
                Minus (simplify b) else if (simplify b)=Float 0. then
                simplify a else Sub (simplify a,simplify b)
        | Mult (a,b) -> if(simplify a)=Float 1. then
                simplify b else if (simplify b)=Float 1. then
                simplify a else Mult (simplify a, simplify b)

let rec simplify_expr expr= 
        let expr2 = simplify expr in if expr2 <> expr then simplify_expr expr2 else expr2

let print_expr expr =
        let openpar prec op_prec =
                if prec > op_prec then "(" else "" in
        let closepar prec op_prec =
                if prec > op_prec then ")" else "" in
        let rec print prec expr = 
                match expr with
                Float f -> string_of_float f
                | Int i -> string_of_int i
                | Add(f, g) ->
                        openpar prec 1 ^ print 1 f ^ "+" ^ print 1 g ^ closepar prec 1
                | Sub(f, g) ->
                        openpar prec 1 ^ print 1 f ^ "-" ^ print 2 g ^ closepar prec 1
                | Mult(f, g) ->
                        openpar prec 3 ^ print 3 f ^ "*" ^ print 3 g ^ closepar prec 3
                | Div(f, g) ->
                        openpar prec 3 ^ print 3 f ^ "/" ^ print 4 g ^ closepar prec 3
                | Minus(a) ->
                        match a with
                                Float f -> openpar prec 0 ^ "-" ^ print 0 a ^ closepar prec 0
                                | Int i -> openpar prec 0 ^ "-" ^ print 0 a ^ closepar prec 0
                                | _ -> openpar prec 0 ^ "-" ^ openpar prec 0 ^ print 0 a ^ closepar prec 0 ^ closepar prec 0
        in print 0 expr
