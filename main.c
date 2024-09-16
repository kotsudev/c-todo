#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/getch/getch.h"
#include <stdbool.h>

#define ALLOWED_TASK_LEN 50
#define GRAY_BACKGROUND "\x1b[47m\x1b[30m"
#define RESET_COLOR "\x1b[0m"
#define ASCII_ENTER 10
#define ASCII_SPACE 32
#define ASCII_W 87
#define ASCII_w 119
#define ASCII_S 83
#define ASCII_s 115
#define ASCII_Q 81
#define ASCII_q 113
#define ASCII_U 85
#define ASCII_u 117
#define ASCII_C 67
#define ASCII_c 99
#define ASCII_D 68
#define ASCII_d 100

typedef struct {
	bool completed;
	char content[ALLOWED_TASK_LEN];
} Task;

void add_task(Task **tasks, int *size);
void remove_task(Task **tasks, int *size, int id);
void render_tasks(const Task *tasks, int size, int id);
void update_task(Task **tasks, int id);

int main()
{
	Task *tasks = NULL;
	int size = 0;
	int selected = 0;
	char ch;

	// Main loop.
	while (1) {
		system("clear");
		printf("W - Move Up     | S - Move Down               | U - Update Task | C - Create Task \nD - Delete Task | Enter - Mark as Done/Undone | Q - quit\n\n");

		render_tasks(tasks, size, selected);

		ch = getch();

		switch (ch) {
		case ASCII_SPACE: {
			tasks[selected].completed = !tasks[selected].completed;
			break;
		}
		case ASCII_C:
		case ASCII_c:
			add_task(&tasks, &size);
			break;
		case ASCII_D:
		case ASCII_d:
			remove_task(&tasks, &size, selected);
			break;
		case ASCII_U:
		case ASCII_u: {
			update_task(&tasks, selected);
			break;
		}
		case ASCII_W:
		case ASCII_w:
			selected = (selected - 1 + size) % size;
			break;
		case ASCII_S:
		case ASCII_s:
			selected = (selected + 1) % size;
			break;
		case ASCII_Q:
		case ASCII_q:
			free(tasks);
			return 0;
		default:
			break;
		}
	}

	return 0;
}

void add_task(Task **tasks, int *size)
{
	*tasks = realloc(*tasks, (*size + 1) * sizeof(Task));

	if (*tasks == NULL) {
		printf("Failed add new task | Reason: bad memory allocation \n");
		exit(1);
	}

	printf("Enter new task content:");
	fgets((*tasks)[*size].content, ALLOWED_TASK_LEN, stdin);

	(*tasks)[*size].content[strcspn((*tasks)[*size].content, "\n")] = 0;
	(*tasks)[*size].completed = false;
	(*size)++;
}

void remove_task(Task **tasks, int *size, int id)
{
	for (int j = id; j < *size - 1; j++) {
		(*tasks)[j] = (*tasks)[j + 1];
	}

	*tasks = realloc(*tasks, (*size - 1) * sizeof(Task));

	if (*tasks == NULL && *size - 1 > 0) {
		printf("Failed remove task | Reason: bad memory allocation\n");
		exit(1);
	}

	(*size)--;
}

void update_task(Task **tasks, int id)
{
	printf("Update task content:");
	fgets((*tasks)[id].content, ALLOWED_TASK_LEN, stdin);

	(*tasks)[id].content[strcspn((*tasks)[id].content, "\n")] = 0;
}

void render_tasks(const Task *tasks, int size, int id)
{
	if (size == 0) {
		printf("Todo list is empty\n");
	}

	for (int i = 0; i < size; i++) {
		printf("%s [%s] %s%s\n", (i == id) ? GRAY_BACKGROUND : "",
		       tasks[i].completed ? "*" : " ", tasks[i].content,
		       (i == id) ? RESET_COLOR : "");
	}
}
