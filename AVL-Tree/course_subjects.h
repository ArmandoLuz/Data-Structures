typedef struct courseSubject CourseSubject;

CourseSubject *createSubjectTree();

//----------------------------BUSCA------------------------------------------
CourseSubject *searchInTheSubjectTree(CourseSubject *tree, int id);
//---------------------------------------------------------------------------

//-----------------------------BALANCEAMENTO---------------------------------
void balanceSubjectTree(CourseSubject **tree); 
void leftRotationInTheSubjectTree(CourseSubject **tree); 
void rightRotationInTheSubjectTree(CourseSubject **tree);
int calculeBalancingFactorOfSubjectTree(CourseSubject *tree);
void updateSubjectTreeHeight(CourseSubject **tree);
//---------------------------------------------------------------------------

//----------------------------INSERÇÃO DE DISCIPLINAS------------------------
void **insertInTheSubjectTree(CourseSubject **root, CourseSubject *node);
void insertSubject(CourseSubject **tree, int limitBlocks);
//---------------------------------------------------------------------------

//------------------------------BUSCAS E IMPRENSÕES--------------------------
void printSubject(CourseSubject *node);
void printSubjectsInOrder(CourseSubject *tree);
void printSubjectById(CourseSubject *tree, int id);
void printSubjectsFromABlock(CourseSubject *tree, int nBlock);
void printSubjectsWithTheSameWorkload(CourseSubject *tree, int workload);
//---------------------------------------------------------------------------

//----------------------------REMOÇÃO DE DISCIPLINAS-------------------------
CourseSubject **searchBiggestNodeLeftOfTheSubjectTree(CourseSubject **tree);
void swapToRemoveSubject(CourseSubject **nodeToRemove, CourseSubject **replacementNode);
void removeSubject(CourseSubject **tree, int idSubject);
//---------------------------------------------------------------------------
