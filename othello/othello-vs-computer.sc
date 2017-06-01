
array state[10][10];

show(){
	puts (" || A | B | C | D | E | F | G | H | I | J ||");
	puts ("===============================================");
	for (i = 0; i < 10; i = i+1;){
		puti_ (i);
		puts_ ("|");
		for (j = 0; j < 10; j=j+1;){
			if (@state[i][j] == 1){
				 puts_ ("| O ");
			}
			else if (@state[i][j] == 2){
				puts_ ("| X ");
			}
			else if (@state[i][j] == 0){
				puts_ ("|   ");
			}
			else if (@state[i][j] == 5){
				puts_ ("| * ");
			}
		}
		puts_ ("|| ");
		puti (i);
		puts ("_______________________________________________");
	}
	puts (" || A | B | C | D | E | F | G | H | I | J ||");
}

checkDir(row, col, rowDir, colDir, player, mo){
	firstRow = -1; 
	firstCol = -1; 
	beat = 0; 
	curRow = row + rowDir; 
	curCol = col + colDir; 

	while (curRow >= 0 && curRow <= 9 && curCol >=0 && curCol <= 9) {
		if (@state[curRow][curCol] == 0 || @state[curRow][curCol] == 5){
			//cannot skip empty point
			break;
		}
		if (@state[curRow][curCol] != player){
			beat = 1; 
		}
		if (@state[curRow][curCol] == player){
			firstRow= curRow;
			firstCol= curCol;
			break;
		}
		curRow = curRow + rowDir;
		curCol = curCol + ColDir;
	}

	// printf("beat = %d\n", beat);

	if (firstCol >= 0 && beat == 1){
		
		if (mo == 1){
			while (curRow != row || curCol != col){
				// printf("[assigning row = %d, col = %d]\n",curRow, curCol );
				@state[curRow][curCol] = player;
				curRow = curRow - rowDir;
				curCol = curCol - ColDir;
			}
			@state[row][col]=player;
		}
		return 1; 
	}
	return 0;
}

check(row, col, player, mo){
	beat = 0; 
	beat = beat + checkDir(row, col, 0, 1, player, mo);
	beat = beat + checkDir(row, col, 0, -1, player, mo);
	beat = beat + checkDir(row, col, 1, 0, player, mo);
	beat = beat + checkDir(row, col, -1, 0, player, mo);

	beat = beat + checkDir(row, col, 1, 1, player, mo);
	beat = beat + checkDir(row, col, 1, -1, player, mo);
	beat = beat + checkDir(row, col, -1, 1, player, mo);
	beat = beat + checkDir(row, col, -1, -1, player, mo);

	return beat; 
}

summary(){
	count1 = 0;
	count2 = 0;
	for (i = 0; i<= 9; i= i+1;){
		for (j = 0; j <= 9; j=j+1;){
			if (@state[i][j] == 1){
				count1 = count1 + 1;
			}
			else if (@state[i][j] == 2){
				count2 = count2 + 1;
			}
		}
	}

	puts ("\n********Game Ended********");	
	puts_ ("/  Player 1's pieces: ");
	puti (count1);
	puts_ ("/  Player 2's pieces: "); 
	puti (count2);
	if (count1 > count2){
		puts ("/  Player 1 wins!");
	}
	else if (count1 < count2){
		puts("/  Player 2 wins!");
	}
	else {
		puts ("/  Game draw!");
	}
	puts ("**************************\n");	

}

move(player){

	ret = 0;  
	row = 0;
	col = 0;
	colchar = 0;


	if (player == 2){
		for (i = 0; i< 10; i=i+1;){
			for (j= 0; j<10; j=j+1;){
				if (@state[i][j] == 5){
					row = i; 
					col = j; 
				}
			}
		}
		colchar = 'A' + col; 
		puts ("\n******************************");
		puts_ ("Computer placed a piece on ");
		putc_ (colchar);
		puti (row);
		puts ("******************************");
	} 
	else {



		puts_ ("\n\nPlalyer ");
		puti_ (player);

		puts (" : please input the position you want to place your piece!");
		if (player == 1){
			puts ("-- You are holding the pieces of 'O'");
		}
		else{
			puts ("-- You are holding the pieces of 'O'");	
		}
		
		puts ("-- Leagl positions are marked with '*'");
		puts ("-- Input format: 'R4', 'A9', etc. ");
		puts ("-- Type '?' for help on the rules. ");
		puts ("-- Type 'Q' to exit. ");

		getc (colchar);
		if (colchar == '?'){
			gets (tmp);
			puts ("\n\n******************************Ohtello Rules*******************************");

			puts ("0. A move consists of \"outflanking\" your opponent's disc(s)");
			puts ("   then flipping the outflanked disc(s) to your colour.");
			puts ("   To outflank means to place a disc on the board so that");
			puts ("	  your opponent's row (or rows) of disc(s) is bordered");
			puts ("   at each end by a disc of your colour");
			puts ("1. The two player take turns to make moves.");
			puts ("2. If on your turn you cannot outflank and flip at least one opposing disc,");
			puts ("   your turn is forfeited and your opponent moves again.");
			puts ("   However, if a move is available to you, you may not forfeit your turn.");
			puts ("3. You cannot skip over your own disc(s) or empty slots \n");
			puts ("(Rules from http://www.hannu.se/games/othello/rules.html)");
			puts ("****************************************************************************\n\n\n");
			return -2; 
		}
		if (colchar == 'Q'){
			summary();
			return -10; 
		}

		geti (row); 


		if (ret < 0 || colchar < 'A' || colchar > 'J') {
			puts ("Illegal Input, format example: \"A2\", \"E4\"");
			return -1;
		}
		col = colchar - 'A';
		if (@state[row][col] == 1 || @state[row][col] == 2){
			puts ("Illegal Input, you cannot override exising pieces");
			return -1;	
		}
	}
	beat = 0; 
	beat = check(row, col, player, 1);
	if (beat == 0){
		puts ("Illegal Input, you must beat at least one pieces\n");
		return -1; 
	}
	return 0; 
}


checkStatus(player){
	i =0; 
	j =0; 
	movable = 0; 
	count = 0;
	count1 = 0;
	count2 = 0;
	for (i = 0; i<= 9; i= i+1;){
		for (j = 0; j <= 9; j=j+1;){
			if (@state[i][j] == 1){
				count1 = count1 + 1;
			}
			else if (@state[i][j] == 2){
				count2 = count2 + 1;
			}
			else if (check(i, j, player, 0) == 1){
				@state[i][j] = 5; 
				movable = movable + 1;
			}
		}
	}
	if (count1 + count2 == 100 || count1 == 0 || count2 == 0){
		puts ("\n********Game Ended********\n");	
		puts_ ("Player 1's pieces: ");
		puti (count1);
		puts_ ("Player 2's pieces: "); 
		puti (count2);
		if (count1 > count2){
			puts ("Player 1 wins!");
		}
		else if (count1 < count2){
			puts("Player 2 wins!");
		}
		else {
			puts ("Game draw!");
		}
		return -1; 
	}

	if (movable == 0){
		return -2;
	}
	return 0;
}

clear(){
	for (i = 0; i<= 9; i= i+1;){
		for (j = 0; j <= 9; j= j+1;){
			if (@state[i][j] == 5){
				@state[i][j] = 0;
			}
		}
	}
}


ret = 0; 
state[4][4]= 1;
state[4][5]= 2;
state[5][4]= 2; 
state[5][5]= 1;
player = 1;
puts ("\n********Game Start********\n");
while(1){
	ret = checkStatus(player);
	if (ret == -1){
		break;
	}
	if (ret == -2){
		puts_ ("Player ");
		puti_ (player);
		puts ("Cannot make a move");
		if (player == 1){
			player = 2;
		}else{
			player = 1;
		}
		continue; 
	}

	show();
	
	ret = move(player);
	if (ret == 0){
		if (player == 1){
			player = 2;
		}else{
			player = 1;
		}
		clear();
	}
	if (ret == -10){
		break;
	}
}