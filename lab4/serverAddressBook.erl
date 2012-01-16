-module(serverAddressBook).

-behaviour(gen_server).
-export([addContact/2, addEmail/3, addPhone/3, removeContact/2, removeEmail/3, removePhone/3, getEmails/2, getPhones/2, findByEmail/1, findByPhone/1]).
-export([start_link/0, stop/0]).
-export([init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2, code_change/3]).


start_link()->
	mnesiaAddressBook:start(),
	gen_server:start_link({local, ?MODULE}, ?MODULE, mnesiaAddressBook:retrieve(ab), []).



addContact(Name, Surname)->
	gen_server:call(?MODULE, {addContact, Name, Surname}).
addEmail(Name, Surname, Email)->
	gen_server:call(?MODULE, {addEmail, Name, Surname, Email}).
addPhone(Name, Surname, Phone)->
	gen_server:call(?MODULE, {addPhone, Name, Surname, Phone}).
removeContact(Name, Surname)->
	gen_server:call(?MODULE, {removeContact, Name, Surname}).
removeEmail(Name, Surname, Email)->
	gen_server:call(?MODULE, {removeEmail, Name, Surname, Email}).
removePhone(Name, Surname, Phone)->
	gen_server:call(?MODULE, {removePhone, Name, Surname, Phone}).
getEmails(Name, Surname)->
	gen_server:call(?MODULE, {getEmails, Name, Surname}).
getPhones(Name, Surname)->
	gen_server:call(?MODULE, {getPhones, Name, Surname}).
findByEmail(Email)->
	gen_server:call(?MODULE, {findByEmail, Email}).
findByPhone(Phone)->
	gen_server:call(?MODULE, {findByPhone, Phone}).
stop()->
	gen_server:cast(?MODULE, stop).

%% ====================================================================
%% Server functions
%% ====================================================================

%% --------------------------------------------------------------------
%% Function: init/1
%% Description: Initiates the server
%% Returns: {ok, State}          |
%%          {ok, State, Timeout} |
%%          ignore               |
%%          {stop, Reason}
%% --------------------------------------------------------------------

init(AB)->
	{ok, AB}.

%% --------------------------------------------------------------------
%% Function: handle_call/3
%% Description: Handling call messages
%% Returns: {reply, Reply, State}          |
%%          {reply, Reply, State, Timeout} |
%%          {noreply, State}               |
%%          {noreply, State, Timeout}      |
%%          {stop, Reason, Reply, State}   | (terminate/2 is called)
%%          {stop, Reason, State}            (terminate/2 is called)
%% --------------------------------------------------------------------

handle_call({addContact, Name, Surname}, From, AB) ->
    Reply = addressBook:addContact(Name, Surname, AB),
	choose_reply(Reply, AB);
handle_call({addEmail, Name, Surname, Email}, From, AB) ->
	Reply = addressBook:addEmail(Name, Surname, Email, AB),
	choose_reply(Reply, AB);
handle_call({addPhone, Name, Surname, Phone}, From, AB) ->
	Reply = addressBook:addPhone(Name, Surname, Phone, AB),
	choose_reply(Reply, AB);
handle_call({removeContact, Name, Surname}, From, AB) ->
	Reply = addressBook:removeContact(Name, Surname, AB),
	choose_reply(Reply, AB);
handle_call({removeEmail, Name, Surname, Email}, From, AB) ->
	Reply = addressBook:removeEmail(Name, Surname, Email, AB),
	choose_reply(Reply, AB);
handle_call({removePhone, Name, Surname, Phone}, From, AB) ->
	Reply = addressBook:removePhone(Name, Surname, Phone, AB),
	choose_reply(Reply, AB);
handle_call({getEmails, Name, Surname}, From, AB) ->
	Reply = addressBook:getEmails(Name, Surname, AB),
	{reply, Reply, AB};
handle_call({getPhones, Name, Surname}, From, AB) ->
	Reply = addressBook:getPhones(Name, Surname, AB),
	{reply, Reply, AB};
handle_call({findByEmail, Email}, From, AB) ->
	Reply = addressBook:findByEmail(Email, AB),
	{reply, Reply, AB};
handle_call({findByPhone, Phone}, From, AB) ->
	Reply = addressBook:findByPhone(Phone, AB),
	{reply, Reply, AB};
handle_call({sort, Type}, From, AB) ->
	Reply = addressBook:sort(Type, AB),
	{reply, Reply, Reply}.


%% --------------------------------------------------------------------
%% Function: handle_cast/2
%% Description: Handling cast messages
%% Returns: {noreply, State}          |
%%          {noreply, State, Timeout} |
%%          {stop, Reason, State}            (terminate/2 is called)
%% --------------------------------------------------------------------
handle_cast(stop, AB)->
	{stop, normal, AB}.

%% --------------------------------------------------------------------
%% Function: handle_info/2
%% Description: Handling all non call/cast messages
%% Returns: {noreply, State}          |
%%          {noreply, State, Timeout} |
%%          {stop, Reason, State}            (terminate/2 is called)
%% --------------------------------------------------------------------
handle_info(_Msg, LoopData)->
	{noreply, LoopData}.

%% --------------------------------------------------------------------
%% Function: terminate/2
%% Description: Shutdown the server
%% Returns: any (ignored by gen_server)
%% --------------------------------------------------------------------
terminate(_Reason, _State) ->
    ok.

%% --------------------------------------------------------------------
%% Func: code_change/3
%% Purpose: Convert process state when code is changed
%% Returns: {ok, NewState}
%% --------------------------------------------------------------------
code_change(_OldVersion, _State, _Extra)->
	ok.

%% --------------------------------------------------------------------
%%% Internal functions
%% --------------------------------------------------------------------

choose_reply(Reply, _State) when is_list(Reply) ->
	mnesiaAddressBook:insert(ab, Reply),
	{reply, Reply, Reply};
choose_reply(Reply, State)->
	{reply, Reply, State}.
