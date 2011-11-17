-module(echo).
-export([send/2,rec/0,start/1]).

send(Pid,Msg) ->
	io:format("Sent hello: ~s \n",[Msg]),
	Pid ! {hello, [Msg]}.

rec() ->
	receive
		{hello, Text} -> io:format("Got hello: ~s \n",[Text])
	end.

start(Msg) ->
	Pid = spawn(echo,rec,[]),
	spawn(echo,send,[Pid,Msg]).
