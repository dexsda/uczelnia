-module(prl_one).
-export([test/1, fact/1]).

fact(0)->1;
fact(N)->N*fact(N-1).

test(N)->
	List=lists:flatten(lists:duplicate(N div 100,lists:seq(0,100))),
	{Time1, _} = timer:tc(fun(X) -> run_single(X) end,[List]),
	{Time2, _} = timer:tc(fun(X) -> run_multi(X) end,[List]),
	{Time1/1000000,Time2/1000000}.

run_single(L)->
	lists:map(fun(X) -> fact(X) end, L).
run_multi(L)->
	pmap(fun(X) -> fact(X) end, L).

pmap(F, L) ->
	S = self(),
	Pids = lists:map(fun(I) ->
				spawn(fun() -> do_f(S, F, I) end)
		end, L),
	gather(Pids).

gather([H|T]) ->
	receive
		{H, Ret} -> [Ret|gather(T)]
	end;
gather([]) ->
	[].

do_f(Parent, F, I) ->
	Parent ! {self(), (catch F(I))}.
