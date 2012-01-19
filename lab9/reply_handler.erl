-module(reply_handler).
-export([init/1, terminate/1, handle_event/2]).
init(BotName) -> BotName.

terminate(BotName) -> {botname, BotName}.

handle_event({login, Name}, BotName) ->
	print(login, Name, BotName),
	BotName;
handle_event({logoff, Name, Reason}, BotName) ->
	print(logoff, Name, Reason, BotName),
	BotName;
handle_event({request, Name, Req}, BotName) ->
	print(request, Name, Req, BotName),
	BotName;
handle_event(_Event, BotName) ->
	BotName.

print(login, Name, BotName) ->
	io:format("~s ~s: Hello, ~s!~n",
		[date_time(), BotName, Name]).
print(logoff, Name, Reason, BotName) ->
	io:format("~s ~s: Bye, ~s! Hope ~s goes well!~n",
		[date_time(), BotName, Name, Reason]);
print(request, Name, Req, BotName) ->
	io:format("~s ~s: I will take notice of this request (~s), ~s!~n",
		[date_time(), BotName, Req, Name]).

date_time() ->
	{{Year,Month,Day},{Hour,Min,Sec}} = calendar:local_time(),
	Months = dict:from_list([
		{1,"Jan"},
		{2,"Feb"},
		{3,"Mar"},
		{4,"Apr"},
		{5,"May"},
		{6,"Jun"},
		{7,"Jul"},
		{8,"Aug"},
		{9,"Sep"},
		{10,"Oct"},
		{11,"Nov"},
		{12,"Dec"}
	]),
	DayStr = pad(integer_to_list(Day),32),
	MonthStr = dict:fetch(Month,Months),
	YearStr = integer_to_list(Year),
	HourStr = pad(integer_to_list(Hour),$0),
	MinStr = pad(integer_to_list(Min),$0),
	SecStr = pad(integer_to_list(Sec),$0),
	["[",DayStr," ",MonthStr," ",YearStr," ",HourStr,$:,MinStr,$:,SecStr,"]"].

pad([M1],Char)  -> [Char,M1];
pad(Other,_Char) -> Other.

%fix_year([_,_,B,C]) -> [B,C];
