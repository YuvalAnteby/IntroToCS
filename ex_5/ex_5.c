#include <stdio.h>
#include "ex_5.h"
#include "string.h"
#include "stdlib.h"
/*
 * Name: Yuval Anteby
 * ID: 212152896
 * Assigment: 5
 */

int main() {
    //Allocate memory for the workers array
    Worker **workersArray = (Worker **) malloc(sizeof(Worker*) * MAX_WORKERS);
    if (workersArray == NULL) {
        exit(MEM_ERROR);
    }
    //Allocate memory for the projects array
    Project **projectsArray = (Project **) malloc(sizeof(Project*) * MAX_PROJECTS);
    if (projectsArray == NULL) {
        for (int i = 0; i < MAX_WORKERS; ++i) {
            freeWorkerMem(workersArray[i]);
        }
        free(workersArray);
        exit(MEM_ERROR);
    }
    int workerCount = 0, projectCount = 0;
    //Initialize the arrays with default values
    initializeArrays(workersArray, projectsArray);
    //Start the main menu of the program
    mainMenu(workersArray, &workerCount, projectsArray, &projectCount);
    return 0;
}

/* Function to initialize the arrays by setting pointers to nulls and counters to zeros.
 * Receives the workers and projects arrays and gets the pointers to workers and projects counters */
void initializeArrays(Worker *workers[], Project *projects[]) {
    //Initialize the worker array by putting 0 in every worker project count and set name, projects pointers as null
    for (int i = 0; i < MAX_WORKERS; ++i) {
        workers[i] = NULL;
    }
    //Initialize the project array by putting 0 in every worker project count and set name, projects pointers as null
    for (int i = 0; i < MAX_PROJECTS; ++i) {
        projects[i] = NULL;
    }
}

/* Function to run the main menu. Will keep running until the user exit the program.
 * Receives the pointers of workers and projects array and pointers to the workers and projects count */
void mainMenu(Worker *workersArray[], int *workerCount, Project *projectsArray[], int *projectCount) {
    const int DEFAULT_ZERO = 0;
    int selection = DEFAULT_ZERO;
    while (selection != EXIT) {
        printf("Welcome!\nPlease select the desired option:\n1. Join the company\n2. Open a new project\n"
               "3. Join an existing project\n4. See what projects are in work\n5. See which people are in what projects"
               "\n6. Work on existing project\n7. Leave the company\n8. Exit\nEnter your choice: ");
        scanf("%d", &selection);
        switch (selection) {
            case JOIN_COMPANY:
                if (*workerCount <= (int) MAX_WORKERS) {
                    joinCompany(workersArray, workerCount, projectsArray, *projectCount);
                } else {
                    printf("The company is at full capacity.\n");
                }
                break;
            case OPEN_NEW_PROJECT:
                if (isWorkersEmpty(*workerCount)) {
                    printf("There are no workers in the company yet, please join the company first.\n");
                } else if (*projectCount >= (int) MAX_PROJECTS) {
                    printf("Maximum number of projects reached.\n");
                } else {
                    openNewProject(workersArray, *workerCount, projectsArray, projectCount);
                }

                break;
            case JOIN_EXISTING_PROJECT:
                if (isWorkersEmpty(*workerCount)) {
                    printf("There are no workers in the company yet, please join the company first.\n");
                } else if (isProjectsEmpty(*projectCount)){
                    printf("There are no projects in the company yet, please create one first.\n");
                } else{
                    workerJoinsProject(workersArray, *workerCount, projectsArray, *projectCount);
                }
                break;
            case SEE_PROJECTS_IN_WORK:
                if (!isProjectsEmpty(*projectCount))
                    displayAllProjects(projectsArray, *projectCount);
                else
                    printf("There are no projects to display.\n");
                break;
            case SEE_PEOPLE_IN_PROJECTS:
                if (isWorkersEmpty(*workerCount))
                    printf("There are no workers to display.\n");
                else
                    displayAllWorkers(workersArray, *workerCount);
                break;
            case WORK_ON_PROJECT:
                if (isWorkersEmpty(*workerCount)) {
                    printf("There are no workers in the company yet, please join the company first.\n");
                } else if (isProjectsEmpty(*projectCount)) {
                    printf("There are no projects in the company yet, please create a project first.\n");
                } else {
                    workOnProject(workersArray, *workerCount, projectsArray, *projectCount);
                }
                break;
            case LEAVE_COMPANY:
                if (isWorkersEmpty(*workerCount)) {
                    printf("There are no workers in the company yet, please join the company first.\n");
                } else {
                    leaveCompany(workersArray, workerCount, projectsArray, projectCount);
                }
                break;
            case EXIT:
                freeAllAllocations(workersArray, *workerCount, projectsArray, *projectCount);
                printf("Exiting...");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                selection = DEFAULT_ZERO;
                break;
        }
    }
}

/* Q1 - Function to add a new worker to the company. Will set the projects pointer to null and zero the project count
 * as default.
 * Receives pointer to workers array and to the count of workers, the projects array and projects counts value */
void joinCompany(Worker *workers[], int *workerCount, Project *projects[], int projectCount) {
    const int INCREMENT = 1, DEFAULT_PROJECTS = 0;
    //Get the name of the worker from the user
    printf("Enter the name of the new worker: ");
    clearInputBuffer();
    char *workerName = getDynamicString();
    if (workerName == NULL){
        //malloc for workerName failed
        freeAllAllocations(workers, *workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    if (isStringEmpty(workerName)){
        printf("Invalid worker name\n");
        free(workerName);
        return;
    }
    //If there is a worker with the same name - free the memory allocated and return to main menu
    if (isWorkerNameExists(workers, *workerCount, workerName)) {
        free(workerName);
        return;
    }
    //Create a pointer of worker -  set its name as the user provided, zero the count and nullify the project pointer
    Worker *newWorker = (Worker*) malloc(sizeof(Worker));
    if (newWorker == NULL) {
        free(workerName);
        freeAllAllocations(workers, *workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    newWorker->name = workerName;
    newWorker->projectCount = DEFAULT_PROJECTS;
    newWorker->projects = NULL;
    //Update the size of the array elements (counter)
    workers[*workerCount] = newWorker;
    *workerCount = *workerCount + INCREMENT;
}

// Function to clear the buffer from the newline char (\n)
void clearInputBuffer(){
    //Clear all chars up to new line
    scanf("%*[^\n]");
    //Clear the new line char
    scanf("%*c");
}

/* Function gets a string of unknown length, will reallocate enough memory as needed for the length of the name
 * returns the pointer to the string. Returns NULL if allocating or reallocating of memory failed */
char *getDynamicString() {
     const int ONE = 1;
    int charIndex = 0, strByteSize = sizeof(char);
    char *stringNamePointer = NULL, *tempCharPointer = NULL, tempChar;
    //Keep getting chars until reaching a new line by the user input
    while (scanf("%c", &tempChar)== ONE && tempChar != '\n' && tempChar != '\r') {
        tempCharPointer = (char*) realloc(stringNamePointer, strByteSize);
        if (tempCharPointer == NULL){
            //realloc failed
            free(stringNamePointer);
            return NULL;
        }
        //increment the index and byte size and move the temp pointer value to the real pointer variable
        stringNamePointer = tempCharPointer;
        stringNamePointer[charIndex] = tempChar;
        strByteSize = strByteSize + (int) sizeof(char);
        charIndex++;
        tempCharPointer = NULL;
    }
    //Add the null byte to the end of the string
    strByteSize = strByteSize + (int) sizeof(char);
    tempCharPointer = (char*) realloc(stringNamePointer, strByteSize);
    if (tempCharPointer == NULL){
        //realloc failed
        free(stringNamePointer);
        return NULL;
    }
    //Set the last char as the null byte
    stringNamePointer = tempCharPointer;
    stringNamePointer[charIndex] = '\0';
    return stringNamePointer;
}

/* Function checks if the string (char array) is empty
 * Returns 1 if empty, 0 if length is more than 1. */
int isStringEmpty(const char *str) {
    const int MIN_STR_LENGTH = 1, VALID = 0, INVALID = 1;
    if (((int)strlen(str)) > MIN_STR_LENGTH)
        return VALID;
    return INVALID;
}

/* Function checks if a worker with the same name already exist (case-sensitive)
 * Receives pointers to workers array, worker count value the name of the new worker */
int isWorkerNameExists(Worker *workers[], int workerCount, const char *name) {
    const int EQUAL_STRINGS = 0, VALID = 0, INVALID = 1;
    //Loop through the workers array and check if the names are identical
    for (int i = 0; i < workerCount; ++i) {
        if (strcmp(name, workers[i]->name) == EQUAL_STRINGS) {
            printf("A worker with this name already exists.\n"
                   "There is not enough space in this company for both of us\n");
            return INVALID;
        }
    }
    return VALID;
}

/* Function checks if there are workers in the company by using the workers counter
 * Receives the value of the workers count
 * Returns 1 if no workers exist, 0 if there is at least one worker */
int isWorkersEmpty(int workersCount) {
    const int DEFAULT_ZERO = 0, VALID = 0, INVALID = 1;
    if (workersCount > DEFAULT_ZERO)
        return VALID;
    return INVALID;
}

/* Q2 - Creates a new project and adds the worker who created the project to the project's workers (and adds the
 * project to the worker's projects)
 * Receives pointers to workers and projects arrays, worker count value and pointer to project count */
void openNewProject(Worker *workers[], int workerCount, Project *projects[], int *projectCount) {
    const int ONE = 1, DEFAULT_WORKERS = 1, STARTING_INDEX = 0;
    int selectedWorker = 0;
    char *projectName = NULL;
    //Display the workers to the user in order to choose from
    printf("Who are you? Choose a number:\n");
    displayWorkers(workers, workerCount);
    printf("Enter your choice: ");
    scanf("%d", &selectedWorker);
    //If the worker chosen is not of the options provided return to the main menu
    if (selectedWorker > workerCount || selectedWorker < ONE) {
        printf("Invalid choice\n");
        return;
    }
    printf("Enter the name of the new project: ");
    clearInputBuffer();
    projectName = getDynamicString();
    if (projectName == NULL) {
        //malloc for projectName failed
        freeAllAllocations(workers, workerCount, projects, *projectCount);
        exit(MEM_ERROR);
    }
    //If there is a project with the same name - free the memory allocated and return to main menu
    if (isStringEmpty(projectName) || isProjectNameExists(projects, *projectCount, projectName)) {
        free(projectName);
        printf("Invalid project name\n");
        return;
    }
    //Create an array of pointers to workers (for the new project) and set the first worker as the one chosen
    Worker **workersInProject = (Worker**) malloc(sizeof(Worker*));
    if (workersInProject == NULL) {
        //Malloc failed
        free(projectName);
        freeAllAllocations(workers, workerCount, projects, *projectCount);
        exit(MEM_ERROR);
    }
    workersInProject[STARTING_INDEX] = workers[selectedWorker - ONE];
    //Create a new project and set its default values and the name the user provided
    Project *newProject = (Project*) malloc(sizeof(Project));
    if (newProject == NULL){
        //Malloc failed
        free(projectName);
        free(workersInProject);
        freeAllAllocations(workers, workerCount, projects, *projectCount);
        exit(MEM_ERROR);
    }
    //Set the values for the new project
    newProject->workerCount = DEFAULT_WORKERS;
    newProject->name = projectName;
    newProject->workers = workersInProject;
    newProject->features = NULL;
    //Update the project count and set the new project in the correct index
    projects[*projectCount] = newProject;
    *projectCount = *projectCount + ONE;
    //Add the new project to the worker by dynamically allocating memory (for projects array in worker)
    Project **tempProjectsPointer = NULL;
    int byteSize =  (*projectCount + ONE) * ((int) sizeof(Project*));
    //If the user has no previous projects - create an array using malloc, if not increase the size
    if (workers[selectedWorker - ONE]->projects != NULL){
        tempProjectsPointer = (Project**) realloc(workers[selectedWorker - ONE]->projects,byteSize);
    }else{
        tempProjectsPointer = (Project**) malloc(byteSize);
    }
    if (tempProjectsPointer == NULL){
        //malloc or realloc failed for the temp pointer
        free(projectName);
        free(newProject);
        free(workersInProject);
        freeAllAllocations(workers, workerCount, projects, *projectCount);
        exit(MEM_ERROR);
    }
    //Set the project array in the correct worker, set the new project in it and update the project counter (of worker)
    int workerProjectCount = workers[selectedWorker - ONE]->projectCount;
    workers[selectedWorker - ONE]->projects = tempProjectsPointer;
    workers[selectedWorker - ONE]->projects[workerProjectCount] = newProject;
    workers[selectedWorker - ONE]->projectCount = workers[selectedWorker - ONE]->projectCount + ONE;
}


/* Function to print all workers from the workers array with an index (starting from 1 when printed)
 * Receives pointer workers array and value of the workers count */
void displayWorkers(Worker *workers[], int workersCount) {
    const int ONE = 1;
    for (int i = 0; i < workersCount; ++i) {
        printf("%d. %s\n", (i + ONE), workers[i]->name);
    }
}

/* Function checks if a project with the same name already exist (case-sensitive)
 * Receives pointer to project array, project count value the name of the new project */
int isProjectNameExists(Project *projects[], int projectCount, const char *name) {
    const int EQUAL_STRINGS = 0, VALID = 0, INVALID = 1;
    for (int i = 0; i < projectCount; ++i) {
        if (strcmp(projects[i]->name, name) == EQUAL_STRINGS) {
            printf("A project with this name already exists.\nBetter be more creative\n");
            return INVALID;
        }
    }
    return VALID;
}

/* Function checks if there are projects in the company by using the projects counter
 * Receives the value of the projects count
 * Returns 1 if no projects exist, 0 if there is at least one project */
int isProjectsEmpty(int projectsCount) {
    const int DEFAULT_ZERO = 0, VALID = 0, INVALID = 1;
    if (projectsCount > DEFAULT_ZERO)
        return VALID;
    return INVALID;
}

/* Q3 - Function lets the user choose an existing user to join an existing project (in case it's valid)
 * Receives pointers array of workers and projects, and values of worker count and project count */
void workerJoinsProject(Worker *workers[], int workerCount, Project *projects[], int projectCount) {
    const int ONE = 1, MAX_PROJECT_WORKERS = 4;
    int selectedWorker, selectedProject;
    //Display the workers for the user to choose from
    printf("Select a worker to join a project:\n");
    displayWorkers(workers, workerCount);
    printf("Enter the worker's number: ");
    scanf("%d", &selectedWorker);
    //Make sure the worker selected is in the boundaries
    if (selectedWorker < ONE || selectedWorker > workerCount) {
        printf("Invalid choice of worker\n");
        return;
    }
    //Display the project and the amount of workers working in each of them
    printf("Select a project to join:\n");
    for (int i = 0; i < projectCount; ++i) {
        printf("%d. %s (Workers: %d)\n", (i + ONE), projects[i]->name, projects[i]->workerCount);
    }
    //Might be changed to: Enter the project's number:\n
    printf("Enter the project's number: ");
    scanf("%d", &selectedProject);
    //Make sure the selected project is in the boundaries
    if (selectedProject < ONE || selectedProject > projectCount) {
        printf("Invalid project selection.\n");
        return;
    }
    //Make sure the Project selected isn't full
    if (projects[selectedProject - ONE]->workerCount >= MAX_PROJECT_WORKERS) {
        printf("Project is full.\n");
        return;
    }
    addWorkerToProject(workers[selectedWorker - ONE], projects[selectedProject - ONE], workerCount,
                       projectCount, workers, projects);
}

/* Function add a worker to a project. Will update the relevant arrays and counters in both project and worker
 * instances.
 * Receives pointers to a worker and a project, the workers and projects counts values and pointers to workers and
 * projects arrays */
void addWorkerToProject(Worker *worker, Project *project, int workerCount, int projectCount, Worker *workers[],
                        Project *projects[]) {
    const int ONE = 1;
    ///Add the worker to the project's workers array
    Worker **tempWorkersPointer = NULL;
    //Resize the workers array in the project
    int newByteSize = ((int) sizeof(Worker)) * (project->workerCount + ONE);
    tempWorkersPointer = (Worker**) realloc(project->workers, newByteSize);
    if (tempWorkersPointer == NULL) {
        //Realloc failed
        freeAllAllocations(workers, workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    project->workers = tempWorkersPointer;
    tempWorkersPointer = NULL;

    //Update the workers count and add the new worker to the project's workers array
    project->workers[project->workerCount] = worker;
    project->workerCount = project->workerCount + ONE;

    ///Add the project to the worker's projects array
    Project **tempProjectsPointer = NULL;
    newByteSize = ((int) sizeof(Project)) * (worker->projectCount + ONE);
    //In case there were no projects for this worker - create a new array for him
    if (worker->projects != NULL) {
        tempProjectsPointer = (Project **) realloc(worker->projects, newByteSize);
    } else {
        tempProjectsPointer = (Project **) malloc(newByteSize);
    }
    if (tempProjectsPointer == NULL) {
        //Malloc/Realloc failed
        freeAllAllocations(workers, workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    //Update the projects count and add the new project to the array
    worker->projects = tempProjectsPointer;
    tempProjectsPointer = NULL;
    worker->projects[worker->projectCount] = project;
    worker->projectCount = worker->projectCount + ONE;
}

/* Q4 - Function to display all projects (including all workers in the project and features).
 * Receives pointer to projects array and value of the project count */
void displayAllProjects(Project *projects[], int projectCount) {
    for (int i = 0; i < projectCount; ++i) {
        printf("Project: %s\n   Workers in this project:\n", projects[i]->name);
        for (int j = 0; j < projects[i]->workerCount; ++j) {
            printf("- %s\n", projects[i]->workers[j]->name);
        }
        printf("    Features of the project:\n");
        if (projects[i]->features == NULL) {
            printf("     No features\n");
        } else {
            displayAllFeatures(projects[i]->features);
        }
    }
    printf("\n");
}

/* Function to print recursively all the names of the features.
 * Receives the pointer to the first node */
void displayAllFeatures(FeatureNode *head) {
    printf(" -%s\n", head->feature);
    if (head->next != NULL) {
        displayAllFeatures(head->next);
    } else {
        printf("\n");
    }
}

/* Q5 - Function to display all workers (including the projects they are a part of)
 * Receives the pointer to workers array and the value of worker count */
void displayAllWorkers(Worker *workers[], int workerCount) {
    const int EMPTY_PROJECTS = 0;
    for (int i = 0; i < workerCount; ++i) {
        printf("Worker: %s\n", workers[i]->name);
        //Make sure there are projects for this worker
        if (workers[i]->projectCount == EMPTY_PROJECTS) {
            printf("    Not involved in any projects.\n");
        } else {
            printf("Projects this worker is involved in:\n");
            //Display the names of the projects
            for (int j = 0; j < workers[i]->projectCount; ++j) {
                printf("        - %s\n", workers[i]->projects[j]->name);
            }
        }
        printf("\n");
    }
}


/* Q6 - Let a user add or remove features from a project he is working on.
 * Receives pointers workers array and the worker count value */
void workOnProject(Worker *workers[], int workerCount, Project *projects[], int projectCount) {
    const int ONE = 1, EMPTY_PROJECTS = 0, EQUAL_STRINGS = 0, EXISTS = 0, EMPTY = 2;
    int selection;
    printf("Select a worker:\n");
    displayWorkers(workers, workerCount);
    printf("Enter the worker's number: ");
    scanf("%d", &selection);
    //Make sure the worker selected is in the boundaries
    if (selection < ONE || selection > workerCount) {
        printf("Invalid choice of worker\n");
        return;
    }
    //Create an instance of the pointer to the chosen worker and check if he got any projects
    Worker *selectedWorker = workers[selection - ONE];
    if (selectedWorker->projectCount == EMPTY_PROJECTS) {
        printf("This worker is not involved in any projects.\n");
        return;
    }
    //Display all the worker's projects
    printf("Select a project:\n");
    for (int i = 0; i < selectedWorker->projectCount; ++i) {
        printf("%d. %s\n", (i + ONE), selectedWorker->projects[i]->name);
    }
    printf("Enter the project number: ");
    scanf("%d", &selection);
    //Make sure the project selected is in the boundaries
    if (selection < ONE || selection > selectedWorker->projectCount) {
        printf("Invalid choice of project\n");
        return;
    }
    //Create an instance of the pointer to the chosen project
    printf("Do you want to add or remove a feature? (add/remove): ");
    clearInputBuffer();
    char *userAction = getDynamicString();
    if (userAction == NULL){
        //realloc for userAction failed
        freeAllAllocations(workers, workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    //Check if the user wants to add a feature
    if (strcmp("add", userAction) == EQUAL_STRINGS || strcmp("add\r", userAction) == EQUAL_STRINGS ||
        strcmp("add\n", userAction) == EQUAL_STRINGS) {
        int addResult = addFeatureToProject(projects[selection-ONE], workers, projects, workerCount,
                                            projectCount);
        if (addResult == MEM_ERROR) {
            free(userAction);
            freeAllAllocations(workers, workerCount, projects, projectCount);
            exit(MEM_ERROR);
        } else if (addResult == EXISTS) {
            printf("This feature already exists.\n");
            free(userAction);
            return;
        } else if (addResult == EMPTY){
            printf("Invalid feature name\n");
            free(userAction);
            return;
        }
        //Check if the user chose to remove a feature
    } else if (strcmp("remove", userAction) == EQUAL_STRINGS || strcmp("remove\r", userAction) == EQUAL_STRINGS ||
               strcmp("remove\n", userAction) == EQUAL_STRINGS) {
        //Make sure there are any features to remove, if there are use the function to remove one
        if (projects[selection-ONE]->features == NULL) {
            printf("There are no features to remove.\n");
            free(userAction);
            return;
        }
        removeFeatureFromProject(projects[selection-ONE]);
    } else {
        //Any other action isn't supported
        printf("Invalid choice.\n");
    }
    free(userAction);
}

/* Function creates a new node of feature for the desired project.
 * Receives a pointer to a project the user would like to add the feature to, pointer to workers and projects array,
 * the values of workers and projects count.
 * Returns 0 if there is already such feature, 2 if the string was empty and 1 if it was added successfully */
int addFeatureToProject(Project *project, Worker *workers[], Project *projects[], int workerCount, int projectCount) {
    const int EXISTS = 0, VALID = 1, EMPTY = 2;
    printf("Enter the new feature: ");
    char *newFeatureName = getDynamicString();
    if (newFeatureName == NULL){
        //realloc for newFeatureName failed
        freeAllAllocations(workers, workerCount, projects, projectCount);
        exit(MEM_ERROR);
    }
    //Check if there is already a feature with the same name for this project
    if (isFeatureExists(project->features, newFeatureName)) {
        free(newFeatureName);
        return EXISTS;
    }
    if (isStringEmpty(newFeatureName)){
        free(newFeatureName);
        return EMPTY;
    }

    //Calculate the byte size required for malloc
    int byteSize = (int) (strlen(newFeatureName) + sizeof(FeatureNode));
    FeatureNode *newFeaturePointer = NULL;
    newFeaturePointer = (FeatureNode *) malloc(byteSize);
    if (newFeaturePointer == NULL) {
        //Malloc failed
        return MEM_ERROR;
    }
    //Set the values for the feature variables
    newFeaturePointer->feature = newFeatureName;
    newFeaturePointer->next = NULL;
    //If there are no features create the first node.
    if (project->features == NULL) {
        project->features = newFeaturePointer;
    } else {
        //If there are features add the new one at the end.
        FeatureNode *lastFeaturePointer = project->features;
        while (lastFeaturePointer->next != NULL) {
            lastFeaturePointer = lastFeaturePointer->next;
        }
        //Set it as the next feature in the linked list
        lastFeaturePointer->next = newFeaturePointer;
    }
    return VALID;
}

/* Go through the linked list of the features recursively and check if a feature with the provided name already exists.
 * Receives the first node of the linked list and the name we want to check
 * Return 0 if no features has the same name, return 1 if there is such name for another feature. */
int isFeatureExists(FeatureNode *head, const char *featureName) {
    const int EQUAL_STRINGS = 0, EXISTS = 1, VALID = 0;
    if (head == NULL)
        return VALID;
    if (strcmp(head->feature, featureName) == EQUAL_STRINGS)
        return EXISTS;
    return isFeatureExists(head->next, featureName);
}

/* Remove the feature a user selects and free the memory that was allocated for it and it's variables
 * Receives the project a user chose that contains features. */
void removeFeatureFromProject(Project *project) {
    const int ONE = 1;
    //Display all features belonging to the project
    printf("Select a feature to remove:\n");
    FeatureNode *featuresPointer = project->features, *tempFeaturesPointer = NULL;
    int index = 1, selection;
    while (featuresPointer != NULL) {
        printf("%d. %s\n", index, featuresPointer->feature);
        index++;
        featuresPointer = featuresPointer->next;
    }
    printf("Enter your choice: ");
    scanf("%d", &selection);
    //Make sure the feature is in the boundaries
    if (selection < ONE || selection > index) {
        printf("Invalid choice of feature\n");
        return;
    }
    //reset the pointer to the first node
    featuresPointer = project->features;
    //Get the one node before the one selected
    for (int i = 1; i < selection; ++i) {
        tempFeaturesPointer = featuresPointer;
        featuresPointer = featuresPointer->next;
    }
    // Remove the selected node from the list
    if (tempFeaturesPointer == NULL) { // If the first element is being removed
        project->features = featuresPointer->next;
    } else {
        tempFeaturesPointer->next = featuresPointer->next;
    }
    // Free the memory of the removed node
    free(featuresPointer->feature);
    free(featuresPointer);
}

/* Q7 - Remove a worker from the company. If the worker had a project he worked in - remove him from the project.
 * In case it was the only worker of the project - remove the project as well (and free allocated memory).
 * Receives a pointer to worker array, pointer to the worker count, Projects array and pointer to the project count. */
void leaveCompany(Worker *workers[], int *workerCount, Project *projects[], int *projectCount) {
    int selection;
    const int ONE = 1, EQUAL_STRINGS = 0;
    printf("Select a worker to leave the company:\n");
    for (int i = 0; i < *workerCount; ++i) {
        printf("%d. %s\n", (i + ONE), workers[i]->name);
    }
    printf("Enter the worker's number: ");
    scanf("%d", &selection);
    //Get the relevant pointers to the worker and his projects
    if (selection < ONE || selection > *workerCount) {
        printf("Invalid worker selection.\n");
        return;
    }
    //If the user didn't work on any projects remove him
    if (workers[selection - ONE]->projectCount < ONE) {
        //Reorganize the workers array
        Worker *tempWorkerPointer = workers[selection - ONE];
        for (int i = (selection - ONE); i < (*workerCount - ONE); i++) {
            workers[i] = workers[i + ONE];
        }
        freeWorkerMem(tempWorkerPointer);
        *workerCount = *workerCount - ONE;
        return;
    }
    //If the user worked on project remove them from the projects
    for (int i = 0; i < workers[selection - ONE]->projectCount; ++i) {
        //remove the entire project if this worker was the only one working on it
        if (workers[selection - ONE]->projects[i]->workerCount == ONE) {
            Project *tempProject = workers[selection - ONE]->projects[i];
            //Reorganize projects array and update the project count
            for (int j = (selection - ONE); j < (*projectCount - ONE); j++) {
                projects[i] = projects[i + ONE];
            }
            freeProject(tempProject);
            *projectCount = *projectCount - ONE;
        } else {
            for (int j = 0; j < workers[selection - ONE]->projects[i]->workerCount; ++j) {
                if (strcmp(workers[selection - ONE]->name, workers[selection - ONE]->projects[i]->workers[j]->name)
                    == EQUAL_STRINGS) {
                    //Remove all instances of this worker in the project, reorganize the array and update the count
                    for (int k = j; k < workers[selection - ONE]->projects[i]->workerCount; k++) {
                        workers[selection - ONE]->projects[i]->workers[k] =
                                workers[selection - ONE]->projects[i]->workers[k + ONE];
                    }
                    //Nullify the last worker since we moved it back one index and update the workers count
                    workers[selection-ONE]->projects[i]->workers[workers[selection-ONE]->projects[i]->workerCount-ONE]
                            = NULL;
                    workers[selection-ONE]->projects[i]->workerCount = workers[selection-ONE]->projects[i]->workerCount
                                                                       - ONE;
                    break;
                }
            }
        }
    }
    //Reorganize the workers array
    Worker *tempWorkerPointer = workers[selection - ONE];

    for (int i = (selection - ONE); i < (*workerCount - ONE); i++) {
        workers[i] = workers[i + ONE];
    }
    freeWorkerMem(tempWorkerPointer);
    *workerCount = *workerCount - ONE;
}

/* Function frees memory allocated for a project and its sub variables.
 * Receives a pointer to a project we would like to free */
void freeProject(Project *project) {
    //In case project wasn't initialized correctly and managed to slip through the other checks
    if (project == NULL)
        return;
    //Free all the features of the project
    if (project->features != NULL)
        freeFeatures(project->features);
    //Free all the workers relevant to the project
    /*for (int i = 0; i < project->workerCount; ++i) {
        freeWorkerMem(project->workers[i]);
    }*/
    //Free the worker array itself
    if (project->workers != NULL)
        free(project->workers);
    //Free the dynamically allocated name
    if (project->name != NULL)
        free(project->name);
    //Free the project itself
    free(project);
}

/* Function frees memory allocated for ALL features. (recursively)
 * Receives the pointer to the first node */
void freeFeatures(FeatureNode *head) {
    //Check if there are features to free
    if (head == NULL)
        return;
    if (head->next != NULL)
        freeFeatures(head->next);
    if (head->feature != NULL)
        free(head->feature);
    free(head);
}

/* free the memory relevant to the worker instance
 * Receives a pointer to a worker. */
void freeWorkerMem(Worker *worker) {
    //In case worker wasn't initialized correctly and managed to slip through the other checks
    if (worker == NULL)
        return;
    //Free the name of the worker
    if (worker->name != NULL)
        free(worker->name);
    //Free the project array of the worker
    if (worker->projects != NULL)
        free(worker->projects);
    //Free the worker itself from the array
    free(worker);
}

/* Q8 - Free all the memory that we allocated or reallocated during the runtime of the program.
 * Receives the pointers to the workers array and projects array, gets the values of worker count and project count. */
void freeAllAllocations(Worker *workers[], int workerCount, Project *projects[], int projectCount) {
    //Free all projects and features
    for (int i = 0; i < projectCount; ++i) {
        if (projects[i] != NULL)
            freeProject(projects[i]);
    }
    //Free all workers (assuming projects indeed have been freed successfully)
    for (int i = 0; i < workerCount; ++i) {
        if (workers[i] != NULL)
            freeWorkerMem(workers[i]);
    }
    free(workers);
    free(projects);
}

