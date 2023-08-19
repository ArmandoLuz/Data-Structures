typedef struct tree45 Tree45;
typedef struct chainedlist ChainedList;
typedef struct info Info;

Tree45 *insertIn45Tree(Tree45 **root, Info *info, Tree45 *fatherRoot, Info **up);
Tree45 *createNode(Info *info1, Info *info2, Tree45 *childLeft, Tree45 *childMiddleA, Tree45 *childMiddleB, Tree45 *childMiddleC, Tree45 *childRight);
Info *createInfo(int id, int quantity, char *mark, float price, int size, char *type);
void addNode(Tree45 **root, Info *info, Tree45 *childRight);
Tree45 *nodeBreaker(Tree45 **root, Info *info, Info **up);
int insertIfEqual(Tree45 *root, Info *info);
int isLeaf(Tree45 *node);
void printTreeInOrder(Tree45 *root);
void printTreeWithLevel(Tree45 *root, int level);
void saveData(Tree45 *root, FILE *archive);
Info *searchInfo(Tree45 *root, int value);
void printInfo(Info *info);
void balance(Tree45 **root, Tree45 **father);
void swapWithBiggestLeft(Tree45 **root, Tree45 **father, Tree45 **biggestNode, int nInfo);
void removeInfo(Tree45 **root, int id, Tree45 **father);
void sell(Tree45 **root, int id, int quantity);
Info *searchInfoWithTrack(Tree45 *root, int value, int level);




