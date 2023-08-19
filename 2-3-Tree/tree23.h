typedef struct tree23 Tree23;
typedef struct chainedlist ChainedList;
typedef struct info Info;

// ----------------------------------Inserção-----------------------------------------------
Tree23 *insertIn23Tree(Tree23 **root, Info *info, Tree23 *fatherRoot, Info **up);
Tree23 *createNode(Info *info, Tree23 *childLeft, Tree23 *childMiddle, Tree23 *childRight);
Info *createInfo(char *word, int line);
void insertLine(ChainedList **listOfLines, int line);
void addNode(Tree23 **root, Info *info, Tree23 *childRight);
Tree23 *nodeBreaker(Tree23 **root, Info *info, Info **up);
int insertIfEqual(Tree23 *root, Info *info);
// -----------------------------------------------------------------------------------------
int isLeaf(Tree23 *node);
void printLines(ChainedList *node);
void printWord(Info *node);
void printTree(Tree23 *root);
void printTreeWithLevel(Tree23 *root, int level);

// -----------------------------------remoção-----------------------------------------------
void removeLine(ChainedList **node, int line);
Info **searchInfo(Tree23 **root, char *word);
void balance(Tree23 **root, Tree23 **father);
void swapWithBiggestLeft(Tree23 **root, Tree23 **father, Tree23 **biggestNode, int nInfo);
void removeInfo(Tree23 **root, char *word, Tree23 **father);
void removeWordFromALine(Tree23 **root, char *word, int line);
// --------------------------------------------------------------------------------------------
Info **searchInfoWithTrack(Tree23 **root, char *word, int level);


