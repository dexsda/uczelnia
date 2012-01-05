-module(programmer).
-export([start/0, stop/0, night/0, day/0, give_project/0, one_day_to_deadline/0, deadline/0, relax/0]).

start() ->
	random:seed(erlang:now()),
	register(programmer, spawn(?MODULE, relax, [])).
stop() ->
	programmer ! stop.

%public func
night() ->
	programmer ! night.

day() ->
	programmer ! day.

give_project() ->
	io:fwrite("Client: Hey, I've got a cool project for you!~n",[]),
	programmer ! {project, self()}, 
	receive
		ok ->
			io:fwrite("Client: Awesome!~n",[]) 
	end,
	ok.

one_day_to_deadline() ->
	io:fwrite("Client: Hey, how's that project you were making for tomorrow going?~n",[]),
	programmer ! {remind, self()},
	receive
		movedeadlineplease ->
			Temp = random:uniform(2),
			case Temp of
				1 ->
					io:fwrite("Client: You kidding me?~n",[]),
					programmer ! false;
				2 ->
					io:fwrite("Client: It better be done soon, though!~n",[]),
					programmer ! true
			end;
		_ ->
			io:fwrite("Client: Great!~n",[])
	end,
	ok.

deadline() ->
	io:fwrite("Client: So, you got it?~n",[]),
	programmer ! {request, self()},
	receive
		aprojectfacsimile ->
			io:fwrite("Client: Yaaay!~n",[]);
		notdone ->
			io:fwrite("Client: You're fired!~n",[])
	end,
	ok.

%states
relax() ->
	io:fwrite("Programmer: Nothing to do!~n",[]),
	receive
		day ->
			relax();
		night ->
			io:fwrite("Programmer: Sleep time!~n",[]),
			sleep();
		{project, Pid} -> 
			io:fwrite("Programmer: Sounds great, I'll get to work right away!~n",[]),
			Pid ! ok,
			timer:sleep(50),
			procrastinate(Pid)
	end.

sleep() ->
	io:fwrite("Programmer: ZZZZZZZZZZZ!~n",[]),
	receive
		night ->
			sleep();
		day ->
			relax()
	end.

procrastinate(Pid) -> 
	io:fwrite("Programmer: Still nothing to do!~n",[]),
	receive
		day ->
			procrastinate(Pid);
		night ->
			io:fwrite("Programmer: Sleep time!~n",[]),
			sleep_project(Pid);
		{remind, Pid} -> 
			io:fwrite("Programmer: Uh, I need a little more time...~n",[]),
			Pid ! movedeadlineplease,
			wait_for_deadline_reply(Pid)
	end.

wait_for_deadline_reply(Pid) ->
	receive
		false ->
			io:fwrite("Programmer: Oh, okay! (damn)~n",[]),
			coding(Pid);
		true ->
			io:fwrite("Programmer: Phew!~n",[]),
			procrastinate(Pid)
	end.

coding(Pid) ->
	receive
		day ->
			io:fwrite("Programmer: Next time I'll do it earlier!~n",[]),
			coding(Pid);
		night ->
			io:fwrite("Programmer: I'm tired... coffee time!~n",[]),
			coding_night(Pid);
		{request, Pid} ->
			io:fwrite("Programmer: .... no. :(~n",[]),
			Pid ! notdone,
			timer:sleep(50),
			relax()
	end.

coding_night(Pid) ->
	receive
		day ->
			io:fwrite("Programmer: noooooooooooooooooooooooooo!~n",[]),
			relax();
		night ->
			io:fwrite("Programmer: COFFEE!! *jitters*~n",[]),
			coding_night(Pid);
		{request, Pid} ->
			io:fwrite("Programmer: Y-y-yepppppppppppp!~n",[]),
			Pid ! aprojectfacsimile,
			timer:sleep(50),
			sleep()
	end.


sleep_project(Pid) ->
	io:fwrite("Programmer: ZZZZZZZZZZZ!~n",[]),
	receive
		night ->
			sleep_project(Pid);
		day ->
			procrastinate(Pid)
	end.

