/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 5
 */
#define MAX_WORKERS 50
#define MAX_PROJECTS 100
#define JOIN_COMPANY 1
#define OPEN_NEW_PROJECT 2
#define JOIN_EXISTING_PROJECT 3
#define SEE_PROJECTS_IN_WORK 4
#define SEE_PEOPLE_IN_PROJECTS 5
#define WORK_ON_PROJECT 6
#define LEAVE_COMPANY 7
#define EXIT 8
#define MEM_ERROR (-1)

// Forward declarations
typedef struct Worker Worker;
typedef struct FeatureNode FeatureNode;
typedef struct Project Project;

struct Worker
{
    char *name;
    Project **projects;
    int projectCount;
};

struct FeatureNode
{
    char *feature;
    struct FeatureNode *next;
};

struct Project
{
    char *name;
    Worker **workers;
    int workerCount;
    FeatureNode *features;
};

void initializeArrays(Worker* workers[], Project* projects[]);
void mainMenu(Worker* workersArray[], int* workersCount, Project* projectsArray[], int* projectCount);
void clearInputBuffer(void);
char* getDynamicString(void);
int isStringEmpty(const char *str);
int isWorkerNameExists(Worker* workers[], int workerCount, const char *name);
int isWorkersEmpty(int workersCount);
int isProjectsEmpty(int projectsCount);
void joinCompany(Worker* workers[], int *workerCount, Project* projects[], int projectCount);
int isProjectNameExists(Project* projects[], int projectCount, const char *name);
void openNewProject(Worker* workers[], int workerCount, Project* projects[], int *projectCount);
void workerJoinsProject(Worker* workers[], int workerCount, Project* projects[], int projectCount);
void displayAllWorkers(Worker* workers[], int workerCount);
void addWorkerToProject(Worker *worker, Project *project, int workerCount, int projectCount, Worker* workers[],
                        Project* projects[]);
void displayAllProjects(Project* projects[], int projectCount);
int isFeatureExists(FeatureNode *head, const char *featureName);
void displayWorkers(Worker* workers[], int workerCount);
void workOnProject(Worker* workers[], int workerCount, Project* projects[], int projectCount);
int addFeatureToProject(Project *project, Worker* workers[], Project* projects[], int workerCount, int projectCount);
void removeFeatureFromProject(Project *project);
void leaveCompany(Worker *workers[], int *workerCount, Project *projects[], int *projectCount);
void freeProject(Project *project);
void freeFeatures(FeatureNode *head);
void freeAllAllocations(Worker *workers[], int workerCount, Project *projects[], int projectCount);
void freeWorkerMem(Worker *worker);
void displayAllFeatures(FeatureNode *head);