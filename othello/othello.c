#include <stdio.h>
#include <stdlib.h>

static int state[10][10];

void show(){
	printf(" || A | B | C | D | E | F | G | H | I | J ||\n");
	printf("===============================================\n");
	int i, j; 
	for (i = 0; i < 10; i++){
		printf("%d|",i);
		for (j = 0; j < 10; j++){
			if (state[i][j] == 1){
				printf("| O ");
			}
			else if (state[i][j] == 2){
				printf("| X ");
			}
			else if (state[i][j] == 0){
				printf("|   ");
			}
			else if (state[i][j] == 5){
				printf("| * ");
			}

		}
		printf("|| %d\n", i);
		printf("_______________________________________________\n");
	}
	printf(" || A | B | C | D | E | F | G | H | I | J ||\n");
}

int check_dir(int row, int col, int row_dir, int col_dir, int player, int mo){
	int first_row = -1; 
	int first_col = -1;
	int beat = 0; 

	int cur_row = row + row_dir;
	int cur_col = col + col_dir;

	int i; 

	while (cur_row >= 0 && cur_row <= 9 && cur_col >= 0 && cur_col <= 9){


		// printf("[checking row = %d, col = %d]\n",cur_row, cur_col );
		if (state[cur_row][cur_col] == 0 || state[cur_row][cur_col] == 5){
			//cannot skip empty point
			break;
		}
		if (state[cur_row][cur_col] != player){
			beat = 1; 
		}
		if (state[cur_row][cur_col] == player){
			first_row= cur_row;
			first_col= cur_col;
			break;
		}
		cur_row = cur_row + row_dir;
		cur_col = cur_col + col_dir;
	}

	// printf("beat = %d\n", beat);

	if (first_col >= 0 && beat == 1){
		
		if (mo == 1){
			while (cur_row != row || cur_col != col){
				// printf("[assigning row = %d, col = %d]\n",cur_row, cur_col );
				state[cur_row][cur_col] = player;
				cur_row = cur_row - row_dir;
				cur_col = cur_col - col_dir;
			}
			state[row][col]=player;
		}
		return 1; 
	}
	return 0;
}


int check(int row, int col, int player, int mo){
	int cur_r, cur_c, first; 
	int beat = 0; 
	int i; 
	beat += check_dir(row, col, 0, 1, player, mo);
	beat += check_dir(row, col, 0, -1, player, mo);
	beat += check_dir(row, col, 1, 0, player, mo);
	beat += check_dir(row, col, -1, 0, player, mo);

	beat += check_dir(row, col, 1, 1, player, mo);
	beat += check_dir(row, col, 1, -1, player, mo);
	beat += check_dir(row, col, -1, 1, player, mo);
	beat += check_dir(row, col, -1, -1, player, mo);

	




	return beat; 
}


int move(int player){
	printf("Plalyer %d: (please input the point as the format of A2):\n", player);
	int ret;  
	int row, col;
	char colchar;  
	ret = scanf (" %c%d", &colchar, &row);
	if (ret < 0 || colchar < 'A' || colchar > 'J') {
		printf("Illegal Input, format example: \"A2\", \"E4\"\n");
		return -1;
	}
	col = colchar - 'A';
	if (state[row][col] == 1 || state[row][col] == 2){
		printf("Illegal Input, you cannot override exising pieces\n");
		return -1;	
	}
	int beat = 0; 
	beat = check(row, col, player, 1);
	if (beat == 0){
		printf("Illegal Input, you must beat at least one pieces\n");
		return -1; 
	}
	//state[row][col] = player;
	//show();
	return 0; 
}

int check_status(int player){
	int i; 
	int j; 
	int movable = 0; 
	int count = 0;
	int count1 = 0, count2 = 0;
	for (i = 0; i<= 9; i++){
		for (j = 0; j <= 9; j++){
			if (state[i][j] == 1){
				count1++;
			}
			else if (state[i][j] == 2){
				count2++;
			}
			else if (check(i, j, player, 0) == 1){
				state[i][j] = 5; 
				movable++;
			}
		}
	}
	if (count1 + count2 == 100 || count1 == 0 || count2 == 0){
		printf("\n********Game Ended********\n");	
		printf("Player 1's pieces: %d\n", count1);
		printf("Player 2's pieces: %d\n", count2);
		if (count1 > count2){
			printf("Player 1 wins\n");
		}else if (count1 < count2){
			printf("Player 2 wins\n");
		}else {
			printf("Game draw\n");
		}
		return -1; 
	}

	if (movable == 0){
		return -2;
	}
	return 0;
}

void clear(){
	int i; 
	int j; 
	for (i = 0; i<= 9; i++){
		for (j = 0; j <= 9; j++){
			if (state[i][j] == 5){
				state[i][j] = 0;
			}
		}
	}
}


int main(){
	int ret; 
	state[4][4]= 1;
	state[4][5]= 2;
	state[5][4]= 2; 
	state[5][5]= 1;
	printf("\n********Game Start********\n");
	int player = 1; 
	while(1){
		ret = check_status(player);
		if (ret == -1){
			exit(0);
		}

		if (ret == -2){
			printf("Player %d Cannot make a move\n", player);
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
	}
}