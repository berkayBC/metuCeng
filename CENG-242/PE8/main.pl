:- module(main, [sum_age/2, max_age_of_hobby/3, person_in_range/4]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

sum_age([],0).
sum_age([H|T],TotalAge) :-
    kb:person(H, Age, _),
    sum_age(T,Y),
    TotalAge is Y + Age.
    
    
max_age_of_hobby_h([],_, Acc, Acc).

max_age_of_hobby_h([First|Remaining], Hobby, Acc, Res) :-
    kb:person(First, Age, H),
    H = Hobby,
    Age > Acc,
    max_age_of_hobby_h(Remaining, Hobby, Age, Res).

max_age_of_hobby_h([First|Remaining], Hobby, Acc, Res) :-
    kb:person(First, Age, H),
    ((H = Hobby, Age =< Acc); H \= Hobby),
    max_age_of_hobby_h(Remaining, Hobby, Acc, Res).

max_age_of_hobby(List, Hobby, MaxAge) :-
    max_age_of_hobby_h(List, Hobby, 0, MaxAge).
    

person_in_range_h([],_,_,Acc,Acc).

person_in_range_h([First|Remaining], Min, Max, Acc, Res):-
    kb:person(First, Age, _),
    Age >= Min,
    Age =< Max,
    append(Acc,[First], NewAcc),
    person_in_range_h(Remaining, Min,Max,NewAcc,Res).

person_in_range_h([First|Remaining], Min, Max, Acc, Res):-
    kb:person(First, Age, _),
    (Age < Min; Age > Max),
    person_in_range_h(Remaining, Min,Max,Acc,Res).

person_in_range(List,Min,Max,Res) :-
    person_in_range_h(List,Min,Max,[],Res).
    










    
    
    
    