-module(rAddressBook).

-export([addContact/2, addEmail/3, addPhone/3, removeContact/2, removeEmail/3, removePhone/3, getEmails/2, getPhones/2, findByEmail/1, findByPhone/1, start/0, stop/0]).
-export([init/1]).

init(AB)->
	receive
		{addContact, Pid, Name, Surname} ->
			NewAB = addressBook:addContact(Name, Surname, AB),
			Pid ! NewAB,
			init(NewAB);
		{addEmail, Pid, Name, Surname, Email} ->
			NewAB = addressBook:addEmail(Name, Surname, Email, AB),
			Pid ! NewAB,
			init(NewAB);
		{addPhone, Pid, Name, Surname, Phone} ->
			NewAB = addressBook:addPhone(Name, Surname, Phone, AB),
			Pid ! NewAB,
			init(NewAB);
		{removeContact, Pid, Name, Surname} ->
			NewAB = addressBook:removeContact(Name, Surname, AB),
			Pid ! NewAB,
			init(NewAB);
		{removeEmail, Pid, Name, Surname, Email} ->
			NewAB = addressBook:removeEmail(Name, Surname, Email, AB),
			Pid ! NewAB,
			init(NewAB);
		{removePhone, Pid, Name, Surname, Phone} ->
			NewAB = addressBook:removePhone(Name, Surname, Phone, AB),
			Pid ! NewAB,
			init(NewAB);
		{getEmails, Pid, Name, Surname} ->
			Pid ! addressBook:getEmails(Name, Surname, AB),
			init(AB);
		{getPhones, Pid, Name, Surname} ->
			Pid ! addressBook:getPhones(Name, Surname, AB),
			init(AB);
		{findByEmail, Pid, Email} ->
			Pid ! addressBook:findByEmail(Email, AB),
			init(AB);
		{findByPhone, Pid, Phone} ->
			Pid ! addressBook:findByPhone(Phone, AB),
			init(AB)
	%	{prettyPrintAll, Pid, Type} ->
	%		NewAB = addressBook:sort(Type, AB),
	%		Pid ! NewAB,
	%		if
	%			is_list(NewAB)->
	%				init(NewAB);
	%			true ->
	%				init(AB)
	%		end
	end.

%%
%% API Functions
%%

addContact(Name, Surname)->
	addressBookServer ! {addContact, self(), Name, Surname},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

addEmail(Name, Surname, Email)->
	addressBookServer ! {addEmail, self(), Name, Surname, Email},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

addPhone(Name, Surname, Phone)->
	addressBookServer ! {addPhone, self(), Name, Surname, Phone},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

removeContact(Name, Surname)->
	addressBookServer ! {removeContact, self(), Name, Surname},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

removeEmail(Name, Surname, Email)->
	addressBookServer ! {removeEmail, self(), Name, Surname, Email},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

removePhone(Name, Surname, Phone)->
	addressBookServer ! {removePhone, self(), Name, Surname, Phone},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

getEmails(Name, Surname)->
	addressBookServer ! {getEmails, self(), Name, Surname},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

getPhones(Name, Surname)->
	addressBookServer ! {getPhones, self(), Name, Surname},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

findByEmail(Email)->
	addressBookServer ! {findByEmail, self(), Email},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

findByPhone(Phone)->
	addressBookServer ! {findByPhone, self(), Phone},
	receive
		ReturnMessage ->
			ReturnMessage
	after 5000 ->
			{didNotReceiveReply}
	end.

start()->
	StartingAB = addressBook:createAddressBook(),
	register(addressBookServer, spawn(?MODULE, init,[StartingAB])).

stop()->
	exit(whereis(addressBookServer), finishedWork).
