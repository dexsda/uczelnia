-module(activeobject).

%%
%% Include files
%%

%%
%% Exported Functions
%%
-export([buffer/1, proxy/2, start/0, take/1, give/2, scheduler/0, methodrequest/4, future/1, check/1, test/2, testGive/2, testTake/2, testTime/2]).

%%
%% API Functions
%%

scheduler() ->
	receive
		MethodRequest ->
			MethodRequest ! guard,
			receive
				true ->
					MethodRequest ! call,
					scheduler();
				false ->
					self() ! MethodRequest,
					scheduler()
			end
	end.

initfuture() ->
	spawn(?MODULE, future,[null]).

future(Inside)->
	receive
		{giveInside, PID } ->
			case Inside of
				null ->
					PID ! Inside;
					%%future ! {giveInside, PID};
				Else ->
					PID ! Inside,
					exit(self(),kill)
			end;
		NewInside ->
			future(NewInside)
	end.

proxy(Scheduler, Servant) ->
	receive
		{UserPID, {give, El}} ->
			Future = initfuture(),
			UserPID ! Future,
			Scheduler ! spawn(?MODULE, methodrequest, [{ give, El}, Scheduler, Servant, Future]),
			proxy(Scheduler, Servant);
		{UserPID, take} ->
			Future = initfuture(),
			UserPID ! Future,
			Scheduler ! spawn(?MODULE, methodrequest, [take, Scheduler, Servant, Future]),
			proxy(Scheduler, Servant)
	end.
		

buffer({Buffer, CurrentL, MaxL}) ->
	receive
		{PID, take} ->
			PID ! lists:last(Buffer),
			buffer({lists:sublist(Buffer, CurrentL - 1), CurrentL - 1, MaxL});
		{PID, {give, El}} ->
			PID ! ok,
			buffer({[El | Buffer], CurrentL + 1, MaxL});
		{PID, isEmpty} ->
			PID ! CurrentL == 0,
			buffer({Buffer, CurrentL, MaxL});
		{PID, isFull} ->
			PID ! CurrentL == MaxL,
			buffer({Buffer, CurrentL, MaxL})
	end.

methodrequest(Job, Scheduler, Servant, Future) ->
	receive
		guard ->
			case Job of
				{ give, El } ->
					Servant ! {self(), isFull},
					receive
						true ->
							Scheduler ! false;
						false ->
							Scheduler ! true
					end;
				take ->
					Servant ! {self(), isEmpty},
					receive
						true ->
							Scheduler ! false;
						false ->
							Scheduler ! true
					end
			end,
			methodrequest(Job, Scheduler, Servant, Future);
		call ->
			case Job of
				{ give, El } ->
					Servant ! {Future, { give, El }};
				take ->
					Servant ! {Future, take}
			end
	end.

take(Proxy) ->
	Proxy ! {self(), take},
	receive
		FuturePid -> FuturePid
	after 3000 -> noFuture
	end.

give(Proxy, El) ->
	Proxy ! {self(), {give, El}},
	receive
		FuturePid -> FuturePid
	after 3000 -> noFuture
	end.

check(Future) ->
	Future ! { giveInside, self() },
	receive 
		Msg -> Msg
	after 2000 -> noMessage
	end.


start() ->
	Servant = spawn(?MODULE, buffer, [ { [], 0, 6 } ]),
	Scheduler = spawn(?MODULE, scheduler, []),
	spawn(?MODULE, proxy, [Scheduler, Servant]).
	

test(Proxy,M) when M>0 ->
	Future = take(Proxy),
	check(Future),
	timer:sleep(1000),
	check(Future),
	Future1=give(Proxy, l),
	check(Future1),
	timer:sleep(1000),
	check(Future1),
	Future2=give(Proxy, l),
	check(Future2),
	timer:sleep(1000),
	check(Future2),
	test(Proxy,M-1);
test(_,_) ->
	koniec.

testTake(Proxy,M) when M>0 ->
	Future = take(Proxy),
	check(Future),
	testTake(Proxy,M-1);
testTake(_,_) ->
	koniec.

testGive(Proxy,M) when M>0 ->
	Future1=give(Proxy, M-1),
	check(Future1),
	testGive(Proxy,M-1);
testGive(_,_) ->
	koniec.

testTime(Proxy, M) ->
	Start = erlang:now(),
	testTake(Proxy, M),
	testGive(Proxy, M),
	timer:now_diff(Start, erlang:now()).
						   

%%
%% Local Functions
%%

