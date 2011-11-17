-module(echoes).
-export([procA/1,procA_loop/2,procB/1,procB_loop/2,procC1/0,procC2/0,procC3/0,start1/0,start2/0,start3/0]).

procA(Pid) ->
	procA_loop(Pid,0).

procA_loop(Pid,10000) -> Pid ! {aaa,10000};
procA_loop(Pid,It) ->
	Pid ! {aaa, It},
	procA_loop(Pid,It+1).

procB(Pid) -> 
	procB_loop(Pid,0).

procB_loop(Pid,10000) -> Pid ! {bbb,10000};
procB_loop(Pid,It) ->
	Pid ! {bbb, It},
	procB_loop(Pid,It+1).

procC1() ->
	receive
		{aaa, It} -> io:format("received aaa, it ~B\n",[It])
	end,
	receive
		{bbb, It2} -> io:format("received bbb, it ~B\n",[It2])
	end,
	procC1().

procC2() ->
	receive
		{aaa, It} -> io:format("received aaa, it ~B\n",[It]), procC2();
		{bbb, It} -> io:format("received bbb, it ~B\n",[It]), procC2()
	end.

procC3() ->
	receive
		M -> io:format("other message ~p\n",[M]), procC3()
	end.

start1() ->
	Pid = spawn(echoes,procC1,[]),
	spawn(echoes,procA,[Pid]),
	spawn(echoes,procB,[Pid]).

start2() ->
	Pid = spawn(echoes,procC2,[]),
	spawn(echoes,procA,[Pid]),
	spawn(echoes,procB,[Pid]).

start3() ->
	Pid = spawn(echoes,procC3,[]),
	spawn(echoes,procA,[Pid]),
	spawn(echoes,procB,[Pid]).
