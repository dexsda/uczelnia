-module(prl_clr).
-export([test/1, fact/1]).

fact(0)->1;
fact(N)->N*fact(N-1).

test(N)->
	List=lists:flatten(lists:duplicate(N div 1000,lists:seq(0,1000))),
	%{Time1, _} = timer:tc(fun(X) -> run_single(X) end,[List]),
	{Time2, _} = timer:tc(fun(X) -> run_multi(X) end,[List]),
	{Time2/1000000}.

run_single(L)->
	lists:map(fun(X) -> fact(X) end, L).
run_multi(L)->
	pmap(fun(X) -> fact(X) end, L).

pmap(F, L) ->
	S = self(),
	Nod = [node()|nodes()],
	{Pids, _} = lists:mapfoldl(
		fun(I, {N1, N2}) ->
				case N1 == [] of
					true -> N = N2;
					false -> N = N1
				end,
				[H|T] = N,
				{spawn(H, fun() -> do_f(S, F, I) end), {T, N2}}
		end, {Nod, Nod}, L),
	gather(Pids).


gather([H|T]) ->
	receive
		{H, Ret} -> [Ret|gather(T)]
	end;
gather([]) ->
	[].

do_f(Parent, F, I) ->
	Parent ! {self(), (catch F(I))}.
