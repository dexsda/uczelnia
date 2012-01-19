-module(log_handler).
-export([init/1, terminate/1, handle_event/2]).
init(File) ->
	{ok, Fd} = file:open(File, write),
	Fd.

terminate(File) -> file:close(File).

handle_event({login, Name}, File) ->
	print(login,Name,File),
	File;
handle_event({logoff, Name, _}, File) ->
	print(logoff,Name,File),
	File;
handle_event({request, Name, Req}, File) ->
	print(request, Name, Req, File),
	File.

print(login, Name, File) ->
	io:format(File,"~s logged on:  ~s~n",
		[date_time(), Name]);
print(logoff, Name, File) ->
	io:format(File,"~s logged off: ~s~n",
		[date_time(), Name]).
print(request, Name, Req, File) ->
	io:format(File,"~s request:    ~s {for ~s}~n",
		[date_time(), Name, Req]).

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
