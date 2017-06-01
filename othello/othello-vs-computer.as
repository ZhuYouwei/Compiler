	push	sp
	push	104
	add
	pop	sp
	push	10
	pop	sb[0]
	push	10
	pop	sb[1]
	jmp	L007
L000:
	push	sp
	push	2
	add
	pop	sp
	push	" || A | B | C | D | E | F | G | H | I | J ||"
	puts
	push	"==============================================="
	puts
	push	0
	pop	fp[0]
L008:
	push	fp[0]
	push	10
	compLT
	j0	L009
	push	fp[0]
	puti_
	push	"|"
	puts_
	push	0
	pop	fp[1]
L011:
	push	fp[1]
	push	10
	compLT
	j0	L012
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	1
	compEQ
	j0	L014
	push	"| O "
	puts_
	jmp	L015
L014:
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	2
	compEQ
	j0	L016
	push	"| X "
	puts_
	jmp	L017
L016:
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	0
	compEQ
	j0	L018
	push	"|   "
	puts_
	jmp	L019
L018:
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	5
	compEQ
	j0	L020
	push	"| * "
	puts_
L020:
L019:
L017:
L015:
L013:
	push	fp[1]
	push	1
	add
	pop	fp[1]
	jmp	L011
L012:
	push	"|| "
	puts_
	push	fp[0]
	puti
	push	"_______________________________________________"
	puts
L010:
	push	fp[0]
	push	1
	add
	pop	fp[0]
	jmp	L008
L009:
	push	" || A | B | C | D | E | F | G | H | I | J ||"
	puts
	push	0
	ret
L007:
	jmp	L021
L001:
	push	sp
	push	5
	add
	pop	sp
	push	1
	neg
	pop	fp[0]
	push	1
	neg
	pop	fp[1]
	push	0
	pop	fp[2]
	push	fp[-9]
	push	fp[-7]
	add
	pop	fp[3]
	push	fp[-8]
	push	fp[-6]
	add
	pop	fp[4]
L022:
	push	fp[3]
	push	0
	compGE
	push	fp[3]
	push	9
	compLE
	and
	push	fp[4]
	push	0
	compGE
	and
	push	fp[4]
	push	9
	compLE
	and
	j0	L023
	push	fp[3]
	push	sb[0]
	mul
	push	fp[4]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	0
	compEQ
	push	fp[3]
	push	sb[0]
	mul
	push	fp[4]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	5
	compEQ
	or
	j0	L024
	jmp	L023
L024:
	push	fp[3]
	push	sb[0]
	mul
	push	fp[4]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	fp[-5]
	compNE
	j0	L025
	push	1
	pop	fp[2]
L025:
	push	fp[3]
	push	sb[0]
	mul
	push	fp[4]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	fp[-5]
	compEQ
	j0	L026
	push	fp[3]
	pop	fp[0]
	push	fp[4]
	pop	fp[1]
	jmp	L023
L026:
	push	fp[3]
	push	fp[-7]
	add
	pop	fp[3]
	push	fp[4]
	push	fp[-6]
	add
	pop	fp[4]
	jmp	L022
L023:
	push	fp[1]
	push	0
	compGE
	push	fp[2]
	push	1
	compEQ
	and
	j0	L027
	push	fp[-4]
	push	1
	compEQ
	j0	L028
L029:
	push	fp[3]
	push	fp[-9]
	compNE
	push	fp[4]
	push	fp[-8]
	compNE
	or
	j0	L030
	push	fp[-5]
	push	fp[3]
	push	sb[0]
	mul
	push	fp[4]
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	fp[3]
	push	fp[-7]
	sub
	pop	fp[3]
	push	fp[4]
	push	fp[-6]
	sub
	pop	fp[4]
	jmp	L029
L030:
	push	fp[-5]
	push	fp[-9]
	push	sb[0]
	mul
	push	fp[-8]
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
L028:
	push	1
	ret
L027:
	push	0
	ret
	push	0
	ret
L021:
	jmp	L031
L002:
	push	sp
	push	1
	add
	pop	sp
	push	0
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	0
	push	1
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	0
	push	1
	neg
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	push	0
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	neg
	push	0
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	push	1
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	push	1
	neg
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	neg
	push	1
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	push	fp[-7]
	push	fp[-6]
	push	1
	neg
	push	1
	neg
	push	fp[-5]
	push	fp[-4]
	call	L001,	6
	add
	pop	fp[0]
	push	fp[0]
	ret
	push	0
	ret
L031:
	jmp	L032
L003:
	push	sp
	push	4
	add
	pop	sp
	push	0
	pop	fp[0]
	push	0
	pop	fp[1]
	push	0
	pop	fp[2]
L033:
	push	fp[2]
	push	9
	compLE
	j0	L034
	push	0
	pop	fp[3]
L036:
	push	fp[3]
	push	9
	compLE
	j0	L037
	push	fp[2]
	push	sb[0]
	mul
	push	fp[3]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	1
	compEQ
	j0	L039
	push	fp[0]
	push	1
	add
	pop	fp[0]
	jmp	L040
L039:
	push	fp[2]
	push	sb[0]
	mul
	push	fp[3]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	2
	compEQ
	j0	L041
	push	fp[1]
	push	1
	add
	pop	fp[1]
L041:
L040:
L038:
	push	fp[3]
	push	1
	add
	pop	fp[3]
	jmp	L036
L037:
L035:
	push	fp[2]
	push	1
	add
	pop	fp[2]
	jmp	L033
L034:
	push	"\n********Game Ended********"
	puts
	push	"/  Player 1's pieces: "
	puts_
	push	fp[0]
	puti
	push	"/  Player 2's pieces: "
	puts_
	push	fp[1]
	puti
	push	fp[0]
	push	fp[1]
	compGT
	j0	L042
	push	"/  Player 1 wins!"
	puts
	jmp	L043
L042:
	push	fp[0]
	push	fp[1]
	compLT
	j0	L044
	push	"/  Player 2 wins!"
	puts
	jmp	L045
L044:
	push	"/  Game draw!"
	puts
L045:
L043:
	push	"**************************\n"
	puts
	push	0
	ret
L032:
	jmp	L046
L004:
	push	sp
	push	8
	add
	pop	sp
	push	0
	pop	fp[0]
	push	0
	pop	fp[1]
	push	0
	pop	fp[2]
	push	0
	pop	fp[3]
	push	fp[-4]
	push	2
	compEQ
	j0	L047
	push	0
	pop	fp[4]
L048:
	push	fp[4]
	push	10
	compLT
	j0	L049
	push	0
	pop	fp[5]
L051:
	push	fp[5]
	push	10
	compLT
	j0	L052
	push	fp[4]
	push	sb[0]
	mul
	push	fp[5]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	5
	compEQ
	j0	L054
	push	fp[4]
	pop	fp[1]
	push	fp[5]
	pop	fp[2]
L054:
L053:
	push	fp[5]
	push	1
	add
	pop	fp[5]
	jmp	L051
L052:
L050:
	push	fp[4]
	push	1
	add
	pop	fp[4]
	jmp	L048
L049:
	push	'A'
	push	fp[2]
	add
	pop	fp[3]
	push	"\n******************************"
	puts
	push	"Computer placed a piece on "
	puts_
	push	fp[3]
	putc_
	push	fp[1]
	puti
	push	"******************************"
	puts
	jmp	L055
L047:
	push	"\n\nPlalyer "
	puts_
	push	fp[-4]
	puti_
	push	" : please input the position you want to place your piece!"
	puts
	push	fp[-4]
	push	1
	compEQ
	j0	L056
	push	"-- You are holding the pieces of 'O'"
	puts
	jmp	L057
L056:
	push	"-- You are holding the pieces of 'O'"
	puts
L057:
	push	"-- Leagl positions are marked with '*'"
	puts
	push	"-- Input format: 'R4', 'A9', etc. "
	puts
	push	"-- Type '?' for help on the rules. "
	puts
	push	"-- Type 'Q' to exit. "
	puts
	getc
	pop	fp[3]
	push	fp[3]
	push	'?'
	compEQ
	j0	L058
	gets
	pop	fp[6]
	push	"\n\n******************************Ohtello Rules*******************************"
	puts
	push	"0. A move consists of \"outflanking\" your opponent's disc(s)"
	puts
	push	"   then flipping the outflanked disc(s) to your colour."
	puts
	push	"   To outflank means to place a disc on the board so that"
	puts
	push	"	  your opponent's row (or rows) of disc(s) is bordered"
	puts
	push	"   at each end by a disc of your colour"
	puts
	push	"1. The two player take turns to make moves."
	puts
	push	"2. If on your turn you cannot outflank and flip at least one opposing disc,"
	puts
	push	"   your turn is forfeited and your opponent moves again.!"
	puts
	push	"   However, if a move is available to you, you may not forfeit your turn."
	puts
	push	"3. You cannot skip over your own disc(s) or empty slots \n"
	puts
	push	"(Rules from http://www.hannu.se/games/othello/rules.html)"
	puts
	push	"****************************************************************************\n\n\n"
	puts
	push	2
	neg
	ret
L058:
	push	fp[3]
	push	'Q'
	compEQ
	j0	L059
	call	L003,	0
	push	10
	neg
	ret
L059:
	geti
	pop	fp[1]
	push	fp[0]
	push	0
	compLT
	push	fp[3]
	push	'A'
	compLT
	or
	push	fp[3]
	push	'J'
	compGT
	or
	j0	L060
	push	"Illegal Input, format example: \"A2\", \"E4\""
	puts
	push	1
	neg
	ret
L060:
	push	fp[3]
	push	'A'
	sub
	pop	fp[2]
	push	fp[1]
	push	sb[0]
	mul
	push	fp[2]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	1
	compEQ
	push	fp[1]
	push	sb[0]
	mul
	push	fp[2]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	2
	compEQ
	or
	j0	L061
	push	"Illegal Input, you cannot override exising pieces"
	puts
	push	1
	neg
	ret
L061:
L055:
	push	0
	pop	fp[7]
	push	fp[1]
	push	fp[2]
	push	fp[-4]
	push	1
	call	L002,	4
	pop	fp[7]
	push	fp[7]
	push	0
	compEQ
	j0	L062
	push	"Illegal Input, you must beat at least one pieces\n"
	puts
	push	1
	neg
	ret
L062:
	push	0
	ret
	push	0
	ret
L046:
	jmp	L063
L005:
	push	sp
	push	6
	add
	pop	sp
	push	0
	pop	fp[0]
	push	0
	pop	fp[1]
	push	0
	pop	fp[2]
	push	0
	pop	fp[3]
	push	0
	pop	fp[4]
	push	0
	pop	fp[5]
	push	0
	pop	fp[0]
L064:
	push	fp[0]
	push	9
	compLE
	j0	L065
	push	0
	pop	fp[1]
L067:
	push	fp[1]
	push	9
	compLE
	j0	L068
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	1
	compEQ
	j0	L070
	push	fp[4]
	push	1
	add
	pop	fp[4]
	jmp	L071
L070:
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	2
	compEQ
	j0	L072
	push	fp[5]
	push	1
	add
	pop	fp[5]
	jmp	L073
L072:
	push	fp[0]
	push	fp[1]
	push	fp[-4]
	push	0
	call	L002,	4
	push	1
	compEQ
	j0	L074
	push	5
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	fp[2]
	push	1
	add
	pop	fp[2]
L074:
L073:
L071:
L069:
	push	fp[1]
	push	1
	add
	pop	fp[1]
	jmp	L067
L068:
L066:
	push	fp[0]
	push	1
	add
	pop	fp[0]
	jmp	L064
L065:
	push	fp[4]
	push	fp[5]
	add
	push	100
	compEQ
	push	fp[4]
	push	0
	compEQ
	or
	push	fp[5]
	push	0
	compEQ
	or
	j0	L075
	push	"\n********Game Ended********\n"
	puts
	push	"Player 1's pieces: "
	puts_
	push	fp[4]
	puti
	push	"Player 2's pieces: "
	puts_
	push	fp[5]
	puti
	push	fp[4]
	push	fp[5]
	compGT
	j0	L076
	push	"Player 1 wins!"
	puts
	jmp	L077
L076:
	push	fp[4]
	push	fp[5]
	compLT
	j0	L078
	push	"Player 2 wins!"
	puts
	jmp	L079
L078:
	push	"Game draw!"
	puts
L079:
L077:
	push	1
	neg
	ret
L075:
	push	fp[2]
	push	0
	compEQ
	j0	L080
	push	2
	neg
	ret
L080:
	push	0
	ret
	push	0
	ret
L063:
	jmp	L081
L006:
	push	sp
	push	2
	add
	pop	sp
	push	0
	pop	fp[0]
L082:
	push	fp[0]
	push	9
	compLE
	j0	L083
	push	0
	pop	fp[1]
L085:
	push	fp[1]
	push	9
	compLE
	j0	L086
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	push	sb[in]
	push	5
	compEQ
	j0	L088
	push	0
	push	fp[0]
	push	sb[0]
	mul
	push	fp[1]
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
L088:
L087:
	push	fp[1]
	push	1
	add
	pop	fp[1]
	jmp	L085
L086:
L084:
	push	fp[0]
	push	1
	add
	pop	fp[0]
	jmp	L082
L083:
	push	0
	ret
L081:
	push	0
	pop	sb[102]
	push	1
	push	4
	push	sb[0]
	mul
	push	4
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	2
	push	4
	push	sb[0]
	mul
	push	5
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	2
	push	5
	push	sb[0]
	mul
	push	4
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	1
	push	5
	push	sb[0]
	mul
	push	5
	add
	push	0
	add
	push	2
	add
	pop	in
	pop	sb[in]
	push	1
	pop	sb[103]
	push	"\n********Game Start********\n"
	puts
L089:
	push	1
	j0	L090
	push	sb[103]
	call	L005,	1
	pop	sb[102]
	push	sb[102]
	push	1
	neg
	compEQ
	j0	L091
	jmp	L090
L091:
	push	sb[102]
	push	2
	neg
	compEQ
	j0	L092
	push	"Player "
	puts_
	push	sb[103]
	puti_
	push	"Cannot make a move"
	puts
	push	sb[103]
	push	1
	compEQ
	j0	L093
	push	2
	pop	sb[103]
	jmp	L094
L093:
	push	1
	pop	sb[103]
L094:
	jmp	L089
L092:
	call	L000,	0
	push	sb[103]
	call	L004,	1
	pop	sb[102]
	push	sb[102]
	push	0
	compEQ
	j0	L095
	push	sb[103]
	push	1
	compEQ
	j0	L096
	push	2
	pop	sb[103]
	jmp	L097
L096:
	push	1
	pop	sb[103]
L097:
	call	L006,	0
L095:
	push	sb[102]
	push	10
	neg
	compEQ
	j0	L098
	jmp	L090
L098:
	jmp	L089
L090:
	jmp	L999
L998:
	push	999999
	puti
L999:
	end
