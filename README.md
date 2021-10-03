# Shatnard
# Language: C
Shatnard console game; a simplae combination of two famous board games: Chess &amp; backgammon.
Its mostly written considering backgammon rules and gameplay with a few minor changes and some chess rules; e.g. pieces are not all same and they have chess names actually.
winner is the one who moves his/her pieces all out of the board faster.
Gameplay note:
1. each player throws dices with pressing d
2. each dice move contains two strings:
    1. First input string is one of his piece's source address and the second string is the destination
    2. Addressing is quite simple really; Castles are 'C' and Roads are 'R'
    3. Player 1's Castle is C1, his Road is R1, and so is for Player 2: C2, R2
    4. and then comes the room index followed by an Underline '_' e.g. Player 2's 3rd Room in Roads => R2_3 or r2_3
so for example if Player 1 has the dice 4 and wants to move his/her piece in first room in player 2's castle
-> C2_1 C2_5
3. other moving rules are quite like backgammon; and there is more also
4. there are some holes and golds in some of the rooms
5. holes trap your piece so you can not move that piece for 2 turns
6. golds lets you rescue one trapped
7. tnx for reading
