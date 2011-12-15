% modul bedacy szablonem wzorca projektowego klient-serwer
% w Erlangu
-module(szablon).
-export([start/1, stop/0]).
-export([init/1, start_cli1/0, init_cli1/0, start_cli2/0, init_cli2/0]).

% Funkcje startowe

start(MinPlayers)->
	register(myserwer, spawn(szablon, init,[MinPlayers])).

init(MinPlayers)->
	% initialization
	Players = dict:new(), %bedzie Pid = {char, {X,Y}, score}
	Field   = dict:new(), %bedzie {x,y} = char | #

	% server loop
	loop(Players,Field,0,MinPlayers,true).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Funkcje klienta

stop() -> async_call (stop).

start_cli1() -> 
	spawn(szablon, init_cli1,[]).

init_cli1() ->
	PlayerNum=sync_call(connect),cli1_loop1(PlayerNum).

cli1_loop1(Num) ->
	receive 
		{movereq, _} -> 
			myserwer ! {request, self(), spawnreq}, cli1_loop2(Num)
	end.

cli1_loop2(Num) ->
	receive
		{movereq, _} ->
			myserwer ! {request, self(), {movereq, left}}, cli1_loop2(Num);
		{despawnreq} ->
			cli1_loop1(Num)
	end.

start_cli2() -> 
	spawn(szablon, init_cli2,[]).

init_cli2() ->
	random:seed(erlang:now()),
	PlayerNum=sync_call(connect),cli2_loop1(PlayerNum).

cli2_loop1(Num) ->
	receive 
		{movereq, _} -> 
			myserwer ! {request, self(), spawnreq}, cli2_loop2(Num)
	end.

cli2_loop2(Num) ->
	receive
		{movereq, {Field, Players}} ->
			{Num,{PosX,PosY},_} = dict:fetch(self(),Players),
			Legal = {cli2_islegal(Field,{PosX-1,PosY}),cli2_islegal(Field,{PosX,PosY-1}),cli2_islegal(Field,{PosX+1,PosY}),cli2_islegal(Field,{PosX,PosY+1})},
			{LegalLeft,LegalUp,LegalRight,LegalDown} = Legal,
			MoveList=cli2_randomize([{left, LegalLeft},{right,LegalRight},{up,LegalUp},{down,LegalDown}]),
			cli2_send_move(MoveList),
			cli2_loop2(Num);
		{despawnreq} ->
			cli2_loop1(Num)
	end.

cli2_send_move([]) ->
	myserwer ! {request, self(), {movereq, down}},ok;
cli2_send_move([{_,false} | T]) ->
	cli2_send_move(T);
cli2_send_move([{Direction,true}| _]) ->
	myserwer ! {request, self(), {movereq, Direction}},ok.

cli2_islegal(Field,{PosX,PosY}) ->
	Bool = dict:is_key({PosX,PosY},Field),
	if
		Bool or (PosX<0) or (PosY<0) or(PosX>10) or(PosY>10) ->
			false;
		true ->
			true
	end.

cli2_randomize(List) ->
	D = lists:map(fun(A) ->
				{random:uniform(), A}
		end, List),
	{_, D1} = lists:unzip(lists:keysort(1, D)), 
	D1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Intejfejs ukrywajacy komunikacje klient-serwer 

%Zwykle uzywamy wywolan synchronicznych, gdy klient potrzebuje odpowiedzi 
%od serwera

sync_call(Message)->
	myserwer ! {request, self(), Message},
	receive
		{reply, Reply}->Reply
	end.

%Moze byc uzyte, gdy klient nie potrzebuje odpowiedzi od serwera 

async_call(Message)->
	myserwer ! {request, Message},
	ok.

%funkcja serwera wysylajaca odpowiedz do klienta

reply(Pid, Message)->
	Pid! {reply, Message}.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%glowna petla

loop(Players,Field,PlayerNumber,MinPlayers,Spawn)->
	print_field(Players,Field,0,0),
	{Players2,Field2,PlayerNumber2}=accept_connections(Players,Field,PlayerNumber),
	AliveNumber=alive_players(Players2),
	if 
		(PlayerNumber2>=MinPlayers) and (AliveNumber==0) and (Spawn==false) ->
			Players3=Players2,
			Field3=dict:new(),
			[Pid ! {despawnreq} || Pid<-dict:fetch_keys(Players3)],
			loop(Players3,Field3,PlayerNumber2,MinPlayers,true);
		(PlayerNumber2>=MinPlayers) and (AliveNumber/=1) -> 
			{Players3,Field3}=getmoves(Players2,Field2),
			loop(Players3,Field3,PlayerNumber2,MinPlayers,false);
		(PlayerNumber2>=MinPlayers) and (AliveNumber==1) ->
			[PlayerPid]=dict:fetch_keys(dict:filter(fun(_,{_,Pos,_}) -> Pos /= {-1,-1} end,Players2)),
			{PlayerChar,_,Score} = dict:fetch(PlayerPid,Players2),
			Players3=dict:store(PlayerPid,{PlayerChar,{-1,-1},Score+1},Players2),
			Field3=dict:new(),
			[Pid ! {despawnreq} || Pid<-dict:fetch_keys(Players3)],
			loop(Players3,Field3,PlayerNumber2,MinPlayers,true);
		true -> 
			{Players3,Field3}={Players2,Field2},
			loop(Players3,Field3,PlayerNumber2,MinPlayers,Spawn)
	end.

alive_players(Players) ->
	dict:size(dict:filter(fun(_,{_,{X,Y}, _}) -> {X,Y} /= {-1,-1} end, Players)).

accept_connections(Players,Field,PlayerNumber)->
	receive
		{request, Pid, connect}->
			{NewPlayers, NewPlayerNumber, Reply}=addplayer(Players, Pid, PlayerNumber),
			reply(Pid, Reply),
			accept_connections(NewPlayers,Field,NewPlayerNumber);
		{request, Pid, disconnect}->
			{NewPlayers,NewField}=delplayer(Players,Field,Pid),
			accept_connections(NewPlayers,NewField,PlayerNumber)
	after 50 ->
			{Players,Field,PlayerNumber}
	end.

print_field(Players,Field,X,Y)->
	Z=dict:is_key({X,Y},Field),
	if
		Y==11 ->
			print_score(Players);
		X==11 ->
			io:fwrite("~n"), print_field(Players,Field,0,Y+1);
		Z ->
			io:fwrite("~c",[dict:fetch({X,Y},Field)]), print_field(Players,Field,X+1,Y);
		true ->
			io:fwrite(" "), print_field(Players,Field,X+1,Y)
	end.

print_score(Players)->
	lists:foreach(fun(X) -> {Char,_,Y}=dict:fetch(X,Players), io:fwrite("~c : ~B ~n",[Char,Y]) end, dict:fetch_keys(Players)), ok.

getmoves(Players,Field) ->
	[Pid ! {movereq, {Field, Players}} || Pid<-dict:fetch_keys(Players)],
	getmoveloop(Players,Field,sets:from_list(dict:fetch_keys(Players))).

getmoveloop(Players,Field,[]) ->
	{Players, Field};
getmoveloop(Players,Field,NoMove) ->
	receive
		{request, Pid, spawnreq} -> 
			Pos = {random:uniform(10),random:uniform(10)},
			{PlayerChar,_,Score} = dict:fetch(Pid,Players),
			NewPlayers = dict:store(Pid,{PlayerChar,Pos,Score},Players),
			NewField = dict:store(Pos,PlayerChar,Field),
			getmoveloop(NewPlayers,NewField,sets:subtract(NoMove,sets:from_list([Pid])));
		{request, Pid, {movereq, Direction}} ->
			{PlayerChar,{PosX,PosY},Score} = dict:fetch(Pid,Players),
			if
				Direction == left -> {NewPosX,NewPosY}={PosX-1,PosY};
				Direction == right -> {NewPosX,NewPosY}={PosX+1,PosY};
				Direction == up -> {NewPosX,NewPosY}={PosX,PosY-1};
				Direction == down -> {NewPosX,NewPosY}={PosX,PosY+1};
				true -> {NewPosX, NewPosY} = {PosX,PosY}
			end,
			Bool = dict:is_key({NewPosX,NewPosY},Field),
			if
				Bool or (NewPosX < 0) or (NewPosX > 10) or (NewPosY < 0) or (NewPosY > 10) -> 
					NewPlayers = dict:store(Pid,{PlayerChar,{-1,-1},Score},Players),
					NewField = dict:store({PosX,PosY},PlayerChar+32,Field);
				true ->
					NewPlayers = dict:store(Pid,{PlayerChar,{NewPosX,NewPosY},Score},Players),
					NewField = dict:store({PosX,PosY},PlayerChar+32,dict:store({NewPosX,NewPosY},PlayerChar,Field))
			end,
			getmoveloop(NewPlayers,NewField,sets:subtract(NoMove,sets:from_list([Pid])))
	after 50 -> {Players,Field}
	end.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% funkcje wewnetrzne implementujace dzialalnosc serwera
% moga byc w osobnym module

addplayer(Players,Pid,PlayerNumber)->
	{dict:store(Pid,{PlayerNumber+65, {-1,-1}, 0},Players),PlayerNumber+1,PlayerNumber+65}.

delplayer(Players,Field,Pid)->
	{_,Pos,_,_}=dict:fetch(Pid,Players),
	NewPlayers=dict:erase(Pid,Players),
	NewField=dict:erase(Pos,Field),
	{NewPlayers,NewField}.
