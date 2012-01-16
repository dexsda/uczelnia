%% Author: p4r4noj4
%% Created: 14-01-2012
%% Description: TODO: Add description to mnesiaAddressBook
-module(mnesiaAddressBook).
-record(addressBookRecord, {addressBook, insides}).
%%
%% Include files
%%

%%
%% Exported Functions
%%
-export([start/0, insert/2, retrieve/1, clear/1]).

%%
%% API Functions
%%

start() ->
	mnesia:create_schema([node()]),
	mnesia:start(),
	mnesia:create_table(addressBookRecord,
						 [{attributes, record_info(fields, addressBookRecord)},
						   {disc_copies, [node()]}]),
	mnesia:wait_for_tables([addressBookRecord], 10000).

insert(AB, Insides) ->
	Record = #addressBookRecord{addressBook = AB, insides = Insides},
	F = fun() ->
		mnesia:write(Record)
	end,
	mnesia:transaction(F).

retrieve(AB) ->
	F = fun() ->
		mnesia:read({addressBookRecord, AB})
	end,
	{atomic, Rows} = mnesia:transaction(F),
	is_empty(Rows).

clear(AB)->
	mnesia:clear_table(AB).

%%
%% Local Functions
%%
is_empty([{_, _, AddressBook}]) ->
	AddressBook;
is_empty(_) ->
	[].
