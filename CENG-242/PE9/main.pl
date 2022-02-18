:- module(main, [bigram/2, num_hobbies/2, sentence_match/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE

%bigram([],[]).

%bigram(X,R):-
%    atom_chars(X,[H1,H2|_]),
%    atom_length(X,Len),
%    Len == 2,
%    atom_concat(H1,H2,R).
    
    %atom_concat(H2,T,X1),
    %bigram(X1,R2),
    %atom_concat(R1,R2,R).
    
%bigram([H|T],R):-
%    bigram(T,R).
    
%bigram(X,R):-
%    atom_length(X,Len),
%    Len > 2,
%    atom_chars(X,[H1,H2|T]),
%    append(H2,T,X1),
%    bigram(X1,R).
bigram_helper([H1,H2|T],R):-
    atom_concat(H1,H2,Res).
    
bigram_helper([H|T],R):-
    bigram_helper(T,R).

bigram(X,R):-
    atom_chars(X,W_List),
    bigram_helper(W_List,R).
    


    


num_hobbies([], []).
num_hobbies([First|Rest],ResultList):-
    kb:person(First,Age,Hobby),
    ResultList = [hobby(Hobby,1)].


sentence_match([],[]).