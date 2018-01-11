
struct node {
	char *name;
	struct node *left;
	struct node *right;
	int count;
};

int get_process_count(struct node *tree);
int contains_dot(char *a);
int compare(char *a, char*b);
int is_higher(const struct node *first, const struct node *second);
struct node *create_leaf(const char *word);
int write_leaf(struct node *leaf, int c_sock);
void print_tree(struct node *root, int c_sock, int v_flag);
void print_leaf(struct node *leaf, int c_sock, int v_flag);
void insert_leaf(struct node **tree, char *leaf);
void destroy_tree(struct node *tree);
