open Ast

exception Interpretation_error

let emptyEnv () = 
        let hash1 = Hashtbl.create 25 in
        let hash2 = Hashtbl.create 25 in
        (hash1, hash2)

(*let interpret env stmt = print_string "Implement me\n"; env*)

let rec eval expr env = match expr with
        | Const x -> x
        | Var name -> Hashtbl.find (fst(env)) name
        | Add (a,b) -> eval a env +. eval b env
        | Mult (a,b) -> eval a env *. eval b env
        | Sub (a,b) -> eval a env -. eval b env
        | Div (a,b) -> eval a env /. eval b env
        | Minus a -> -. eval a env
        | Call (name, args) -> let hash1_copy = Hashtbl.copy (fst(env)) in
                let hash2_copy = Hashtbl.copy (snd(env)) in
                let tmp_env = (hash1_copy, hash2_copy) in 
                let arglist = fst(Hashtbl.find (snd(tmp_env)) name) in
                if List.length(args) == List.length(arglist) then
                        let rec loop tmp_env2 args arglist = match args, arglist with
                                | [], [] -> tmp_env2
                                | h::t, h1::t1 -> let evaluated = eval h env in
                                        Hashtbl.replace(fst(tmp_env2)) h1 evaluated;
                                        loop tmp_env2 t t1 
                                | _ -> tmp_env2 in
                        eval (snd(Hashtbl.find (snd(tmp_env)) name))
                        (loop(tmp_env) (args) (arglist))
                else invalid_arg "wrong number of arguments \n"

let interpret env stmt = match stmt with
        | Eval a -> Printf.printf "%.3f\n" (eval a env); env
        | Let (name, a) -> let evaluated = eval a env in
                Hashtbl.replace (fst(env)) name evaluated; env
        | Def (name, args, a) -> Hashtbl.replace (snd(env)) name (args, a); env
