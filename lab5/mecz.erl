-module(mecz).
-export([loop/1,start/1]).

create(Kraj) ->
	receive
		{beg, Pid} -> 

start(Kraj,Partner) ->
	Pid = spawn(mecz,loop,[Kraj]),
	Pid2 = spawn(mecz,loop,[Kraj]),
	spawn(echo2,send,[Pid,Msg]).
