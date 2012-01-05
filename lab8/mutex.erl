-module(mutex).
-export([start/0, stop/0, free/0, wait/0, signal/0]).

start() -> register(mutex, spawn(?MODULE, free, [])).
stop() -> mutex ! stop.

wait() ->
	mutex ! {wait, self()},
	receive ok -> ok end.

signal() ->
	mutex ! {signal, self()},
	ok.

free() ->
	receive 
		{wait, Pid} ->
			Pid ! ok,
			blocked(Pid);
		stop ->
			terminate()
	end.

blocked(Pid) -> receive {signal, Pid} -> free() end.

terminate() ->
	receive 
		{wait, Pid} ->
			exit(Pid,kill),
			terminate()
	after
		0 -> ok
	end.

