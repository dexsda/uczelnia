-module(echo2).
-export([send/2,rec/0,start/1]).

send(Pid,Msg) ->
	Pid ! {hello, [Msg]},
	io:format("Sent hello: ~s \n",[Msg]).

rec() ->
	receive
		{goodbye, Text} -> io:format("Got goodbye: ~s \n",[Text])
		after 1000 -> io:format("Waiting...\n"), rec()
	end.

start(Msg) ->
	Pid = spawn(echo2,rec,[]),
	spawn(echo2,send,[Pid,Msg]).
