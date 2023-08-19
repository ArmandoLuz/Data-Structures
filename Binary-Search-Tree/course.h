typedef struct course Course;

Course *createCourseTree();

//----------------------------BUSCA------------------------------------------
Course **searchInTheCourseTree(Course **tree, int id);
//---------------------------------------------------------------------------

//----------------------------INSERÇÃO DE CURSO------------------------------
void insertCourse(Course **tree, int id);
//---------------------------------------------------------------------------

//------------------------INSERÇÃO DE DISCIPLINAS----------------------------
void insertCourseSubject(Course *tree, int idCourse);
//---------------------------------------------------------------------------

//------------------------------BUSCAS E IMPRENSÕES--------------------------
void printCourse(Course *node);
void printCourseInOrder(Course *tree);
void printCourseById(Course *tree, int id);
void printCoursesWithTheSameAmountOfBlocks(Course *tree, int nBlocks);
void printCourseSubject(Course *tree, int idCourse, int idSubject);
void printCourseSubjectsInOrder(Course *tree, int idCourse);
void printSubjectsFromABlockOfACourse(Course *tree, int idCourse, int nBlock);
void printSubjectsOfACourseWithTheSameWorkload(Course *tree, int idCourse, int workload);
//---------------------------------------------------------------------------

//----------------------------REMOÇÃO DE CURSOS------------------------------
Course **searchBiggestNodeLeftOfTheCourseTree(Course **tree);
void swapToRemoveCourse(Course **nodeToRemove, Course **replacementNode);
void removeCourse(Course **tree, int idCourse);
//---------------------------------------------------------------------------

//-----------------------------REMOÇÃO DE DISCIPLINA-------------------------
void removeSubjectFromACourse(Course *tree, int idCourse, int idSubject);
//---------------------------------------------------------------------------