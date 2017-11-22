#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

enum {
//проверяю на правильность в программе - тестировщике
	MODESQ = 5,
	TEXTQ = 3,
	COM_SIZE = 5,
	ANSW_SIZE = 10,
};

void
make_exit(int b)
{
	if (b == -1) {
		abort();
	}
}

void
end_with_err(int r, int err)
{
	if (r == -1) {
		char *msg = strerror(err);
		int w = write(2, msg, strlen(msg));
		make_exit(w);
		exit(1);
	}
}

int
choose_mode(char *your_command, char *mode[])
{
//прогоняем нашу команду и смотрим, есть ли такая. если есть - возвращаем ее номер в массиве команд
	int i, cur_mode = -1;
	for (i = 0; i < MODESQ; i++) {
		if (memcmp(your_command, mode[i], COM_SIZE) == 0) {
			cur_mode = i;
			break;
		}
	}
	if (cur_mode < 0) {
		char *wrong_command = "There is no such a command\n";
		int w = write(2, wrong_command, strlen(wrong_command));
		end_with_err(w, errno);
		exit(1);
	}
	return cur_mode;
}

int
main(void)
{
	char *theme = "This is Math test\n";
	char *mas_of_questions[] = {"How much is 1 + 1?\n", "How much is 2 + 3?\n", "How many sides has the triangle?\n"};
	char *mas_of_answers[] = {"2\n", "5\n", "3\n"};
	char *mode[] = {"gett\n", "getq\n", "cmpa\n", "getn\n", "stop\n"};
	//MANAGE - STRINGS CREATION FINISH

	char *right = "Right answer\n", *wrong = "Wrong answer\n";
	//STRINGS FOR ENDING CREATION FINISH

	int i, n, w, r, l = 5, cur_mode;
	char your_command[COM_SIZE], number[COM_SIZE];
	char your_answer[ANSW_SIZE]; 
	memset(your_command, 0, COM_SIZE);
	while (1) {
		r = read(0, your_command, COM_SIZE);
		make_exit(r);
		if ((cur_mode = choose_mode(your_command, mode)) == 0) {
			w = write(1, theme, strlen(theme));
			end_with_err(w, errno);
		}
		else if (cur_mode == 1) {
			memset(number, 0, COM_SIZE);
			r = read(0, number, COM_SIZE);
			end_with_err(r, errno);
			int quest_num = strtol(number, NULL, 10) - 1;
			w = write(1, mas_of_questions[quest_num], strlen(mas_of_questions[quest_num]));
			end_with_err(w, errno);
		}
		else if (cur_mode == 2) {
			memset(your_answer, 0, ANSW_SIZE);
			r = read(0, your_answer, ANSW_SIZE);
			end_with_err(r, errno);
			int ans_num = strtol(your_answer, NULL, 10) - 1;

			memset(your_answer, 0, ANSW_SIZE);
			r = read(0, your_answer, ANSW_SIZE);

			if (memcmp(your_answer, mas_of_answers[ans_num], strlen(mas_of_answers[ans_num])) == 0) {
				w = write(1, right, strlen(right));
				end_with_err(w, errno);
			} else {
				w = write(1, wrong, strlen(wrong));
				end_with_err(w, errno);
			}
		}
		else if (cur_mode == 3) {
			char str[3];
			memset(str, 0, 3);
			sprintf(str, "%d", TEXTQ);
			w = write(1, str, strlen(str));
			end_with_err(w, errno);
		}
		else if (cur_mode == 4) {
			break;
		}
	}		
	return 0;
}
