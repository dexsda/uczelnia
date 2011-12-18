%% Author: dex
%% Created: 06-12-2011
%% Description: TODO: Add description to addressBook
-module(addressBook).

%%
%% Include files
%%

%%
%% Exported Functions
%%
-export([createAddressBook/0, addContact/3, addEmail/4, addPhone/4, removeContact/3]).
-export([removeEmail/4, removePhone/4, getEmails/3, getPhones/3, findByEmail/2, findByPhone/2, prettyPrint/2, prettyPrint/3, prettyPrintAll/1]).

%%
%% API Functions
%%
createAddressBook()->[].

addContact(Name,Surname,Book)->addContactDo(containsName({Name,Surname},Book),{Name,Surname},Book).

addEmail(Name,Surname,Email,Book)-> addEmailDo(containsName({Name,Surname},Book),containsEmail(Email,Book),Name,Surname,Email,Book).

addPhone(Name,Surname,Phone,Book)-> addPhoneDo(containsName({Name,Surname},Book),containsPhone(Phone,Book),Name,Surname,Phone,Book).

removeContact(_,_,[])->[];
removeContact(Name,Surname,[{{Name,Surname},_,_} | T])->T;
removeContact(Name,Surname,[H | T])->[H | removeContact(Name,Surname,T)].

removeEmail(_,_,_,[])->[];
removeEmail(Name,Surname,Email,[{{Name,Surname},Emails,Phones} | T])->[{{Name,Surname},removeFromList(Email,Emails),Phones} | T];
removeEmail(Name,Surname,Email,[H | T])->[H | removeEmail(Name,Surname,Email,T)].

removePhone(_,_,_,[])->[];
removePhone(Name,Surname,Phone,[{{Name,Surname},Emails,Phones} | T])->[{{Name,Surname},Emails,removeFromList(Phone,Phones)} | T];
removePhone(Name,Surname,Phone,[H | T])->[H | removePhone(Name,Surname,Phone,T)].

getEmails(_,_,[])->[];
getEmails(Name,Surname,[{{Name,Surname},Emails,_} | _])->Emails;
getEmails(Name,Surname,[_ | T])->getEmails(Name,Surname,T).

getPhones(_,_,[])->[];
getPhones(Name,Surname,[{{Name,Surname},_,Phones} | _])->Phones;
getPhones(Name,Surname,[_ | T])->getPhones(Name,Surname,T).

findByEmail(_,[])->ok;
findByEmail(Email,[{Name,Emails,_} | T])-> 
	Bool=contains(Emails,Email),
	if
		Bool -> Name;
		true -> findByEmail(Email,T)
	end.

findByPhone(_,[])->ok;
findByPhone(Phone,[{Name,_,Phones} | T])-> 
	Bool=contains(Phones,Phone),
	if
		Bool -> Name;
		true -> findByPhone(Phone,T)
	end.

prettyPrintAll([])->ok;
prettyPrintAll([{{Name,Surname},Emails,Phones} | T])->printEntry(Name,Surname,Emails,Phones),prettyPrintAll(T).

prettyPrint(Name,Surname,Book)->prettyPrint({Name,Surname},Book).

prettyPrint(_,[])->ok;
prettyPrint({Name,Surname},[{{Name,Surname},Emails,Phones} | _])->printEntry(Name,Surname,Emails,Phones);
prettyPrint(Person,[_ | T])->prettyPrint(Person,T).
%%
%% Local Functions
%%

addEmailDo(_,true,_,_,_,Book)->Book;
addEmailDo(false,false,Name,Surname,Email,Book)->addEmailDo(true,false,Name,Surname,Email,addContact(Name,Surname,Book));
addEmailDo(true,false,Name,Surname,Email,[{{Name,Surname},Emails,Phones} | T])->[ {{Name,Surname},[Email | Emails],Phones} | T];
addEmailDo(true,false,Name,Surname,Email,[H | T])->[H | addEmailDo(true,false,Name,Surname,Email,T)].

addPhoneDo(_,true,_,_,_,Book)->Book;
addPhoneDo(false,false,Name,Surname,Phone,Book)->addPhoneDo(true,false,Name,Surname,Phone,addContact(Name,Surname,Book));
addPhoneDo(true,false,Name,Surname,Phone,[{{Name,Surname},Emails,Phones} | T])->[ {{Name,Surname},Emails,[Phone | Phones]} | T];
addPhoneDo(true,false,Name,Surname,Phone,[H | T])->[H | addPhoneDo(true,false,Name,Surname,Phone,T)].

removeFromList(_,[])->[];
removeFromList(Item,[Item | T])->T;
removeFromList(Item,[H | T])->[H | removeFromList(Item,T)].

addContactDo(true,_,Book)->Book;
addContactDo(false,Entry,Book)-> [{Entry,[],[]} | Book].

containsEmail(_,[])-> false;
containsEmail(Email,[{_,EmailList,_} | BookT])-> contains(EmailList,Email) orelse containsEmail(Email,BookT).

containsPhone(_,[])-> false;
containsPhone(Phone,[{_,_,PhoneList} | BookT])-> contains(PhoneList,Phone) orelse containsPhone(Phone,BookT).

containsName(_,[])-> false;
containsName(Entry,[{Entry,_,_} | _])-> true;
containsName(Entry,[_ | T])-> containsName(Entry,T).

printEntry(Name,Surname,Emails,Phones)->
	io:fwrite("~s ~s:",[Name,Surname]),
	io:fwrite("~n~c Emails: ",[9]),
	printList(Emails),
	io:fwrite("~n~c Phones: ",[9]),
	printList(Phones),
	io:fwrite("~n",[]).

printList([])->ok;
printList([H | T])->io:fwrite("~s; ",[H]),printList(T).

contains([],_) -> false;
contains([H | _],H) -> true;
contains([_ | T],Var) -> contains(T,Var).
