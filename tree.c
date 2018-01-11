#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tree.h"

#define FALSE 0
#define TRUE  1

int total_count;

//Grabs the total number of processes for a user for the logfile
int get_process_count(struct node *leaf)
{
	return total_count;
}

int compare(char *a, char *b)
{
	//check to see if the first character of a and b is a letter
	char *temp_a = a;
	char *temp_b = b;

	//if the first spot isn't letter, move until you find one
	while (!isalpha(temp_a[0]))
		temp_a++;

	while (!isalpha(temp_b[0]))
		temp_b++;

	return strncasecmp(temp_a, temp_b, PATH_MAX);

}

int write_leaf(struct node *leaf, int c_sock)
{
	int rc;
	char *process = leaf->name;
	char *count = calloc(100, sizeof(char));
	char *buf = calloc(PATH_MAX, sizeof(char));
	int process_length = strnlen(leaf->name, PATH_MAX);

	sprintf(count, "%d", leaf->count);
	//send the process
	strncpy(buf, process, process_length);
	strncat(buf, " ", sizeof(char));
	strncat(buf, count, (process_length + sizeof(count)));
	strncat(buf, "\n", sizeof(char));
	rc = send(c_sock, buf, strlen(buf), 0);
	if (rc == -1)
		return FALSE;

	free(count);
	free(buf);

	return TRUE;
}

void print_leaf(struct node *leaf, int c_sock, int v_flag)
{
	if (leaf->left)
		print_leaf(leaf->left, c_sock, v_flag);

	write_leaf(leaf, c_sock);
	if (v_flag == TRUE)
		printf("%s %d\n", leaf->name, leaf->count);
	if (leaf->right)
		print_leaf(leaf->right, c_sock, v_flag);
}

void print_tree(struct node *tree, int c_sock, int v_flag)
{
	struct node *root = tree;

	if (root)
		print_leaf(root, c_sock, v_flag);

}

void insert_leaf(struct node **tree, char *leaf)
{
	struct node *probe = *tree;
	struct node *parent;
	struct node *new_leaf = create_leaf(leaf);

	while (probe != NULL) {

		//find insertion point, if compare is greater than 0 go left
		if (compare(probe->name, new_leaf->name) > 0) {
			parent = probe;
			probe = parent->left;
		}
		//if less than 0 got right
		else if (compare(probe->name, new_leaf->name) < 0) {
			parent = probe;
			probe = parent->right;

		} else {
			//if the same, increment count
			(probe->count)++;

			free(new_leaf->name);
			free(new_leaf);
			return;
		}
	}

	if (compare(parent->name, new_leaf->name) > 0)
		parent->left = new_leaf;
	else
		parent->right = new_leaf;
}

struct node *create_leaf(const char *word)
{
	const int word_length = strnlen(word, PATH_MAX) + 1;
	struct node *new_leaf = malloc(sizeof(struct node));

	new_leaf->name = calloc(word_length, sizeof(char));
	strncpy(new_leaf->name, word, word_length);
	new_leaf->left = NULL;
	new_leaf->right = NULL;
	new_leaf->count = 1;
	total_count++;

	return new_leaf;
}

void destroy_tree(struct node *tree)
{
	if (tree) {
		destroy_tree(tree->left);
		destroy_tree(tree->right);
		free(tree->name);
		free(tree);
		total_count  = 0;
	}
}
