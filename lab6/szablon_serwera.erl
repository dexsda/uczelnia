% modul bedacy szablonem wzorca projektowego klient-serwer
% w Erlangu

-module(szablon).
-export([start/0, stop/0, setnick/1, fun2/1]).
-export([init/0]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Funkcje startowe

start()->
   register(myserwer, spawn(szablon, init,[])).
   
init()->
    % initialization
    Dictionary = dict:new(),
    % server loop
    loop(Dictionary).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Funkcje klienta

stop() -> async_call (stop).

setnick(Nick) ->sync_call({nick, Nick}).

fun2(MyArg)-> sync_call ({fun2,MyArg}).

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

loop(Dictionary)->
   receive
      {request, Pid, {nick, Nick}}->
           {NewDictionary, Reply}=regnick(Dictionary, Pid, Nick),
           reply(Pid, Reply),
           loop(NewDictionary);
 
      {request, Pid, {say, Msg}}->
           broadcastmsg(Dictionary,Pid,Msg),
           loop(Dictionary);

      {request,  stop} ->
           cleanFun()
   end.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% funkcje wewnetrzne implementujace dzialalnosc serwera
% moga byc w osobnym module

  regnick(Dict,Pid,Nick)
       {dict:store(Pid,Nick,Dict),nickack};
 
  broadcastmsg(Dictionary, Text)
       lists:foreach(fun(X) -> Y = dict:fetch(X, Dictionary), reply(Y,{msg, Text}) end, dict:fetch_keys(Dictionary));   
