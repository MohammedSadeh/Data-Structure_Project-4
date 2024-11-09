/*
Name : Mohammed Jamil Saada
ID : 1221972
Section : 1
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define INF INT_MAX

int NumOfCourses = 0;

// struct for each building
struct Building{
    char* building;
    int distance;
    struct Building* next;
};
typedef struct Building* B;

//struct to vertix [building with list to the adjacent list]
struct List{
    char* build_name;
    struct List* next;
    B Buildings;
};
typedef struct List* L;

//struct for the prerequisites courses for such course
struct CoursesPrerequiest{
    char* course;
    struct CoursesPrerequiest* next;
};
typedef struct CoursesPrerequiest* CPre;

//struct for a course [vertix]
struct Course{
    char* course;
    int inDegree;
    int Visited;
    struct Course* next;
    CPre Courses;
};
typedef struct Course* course;

// struct for the queue
struct queue{
char* token;
struct queue* next;
struct queue* front;
struct queue* rear;
};
typedef struct queue* ptq;

//struct for each cell in the heap
struct Cell{
    char* build_name;
    int distance;
};
typedef struct Cell cell;

//struct for the Heap [minHeap]
struct minHeap{
    int size;
    int max_size;
    cell* heapArray;
};
typedef struct minHeap* Heap;

//struct for the Dijkstra Matrix, which contains: build_name [vertix], known, the cost, parent [path]
struct DijkstraMatrix{
    char* build_name;
    int known;
    int cost;
    char* path;
};
typedef struct DijkstraMatrix DMatrix;

//Functions Prototype :-
//***********************************************************************************
B CreateBuildings();
void InsertToBuildings(B build,char* building, int distance);
L SearchNode(L list,char* building);
L CreateList();
void InsertToList(L list,char* building);
L LoadBuildings();
course CreateCourse();
CPre CreateCoursePre();
void InsertToCourses(course list,char* c_name);
void InsertToPreCourses(CPre list,char* c_name);
course LoadCourses();
course SearchCourse(course list,char* c_name);
course SearchPreCourse(CPre list,char* c_name);
course LoadCourses();
ptq createQueue();
void EnQueue(ptq Q, char* tok);
ptq DeQueue(ptq Q);
CPre SearchAdjacent(CPre C, char* token);
void topoSort(course C, ptq Q, int x);
CPre Topological(course C);
void printSortedCourses(CPre list);
Heap CreateHeap(int maxSize);
int parent(int pos);
int rightChild(int pos);
int isLeaf(int pos, Heap H);
Heap swap(int pos1, int pos2, Heap H);
Heap insertToHeap(char* name, int distance, Heap H);
cell deleteFromHeap(Heap H);
int findNumofVertices(L list);
Heap minHeapify(int pos, Heap H);
B Dijkstra(L list, char* Source, char* Distenation);
void PrintPath(B path);
int CalculateDistance(B path, char* Distenation);
void displayMenu();
//****************************************************************************
int main()
{
    printf("Welcom to Project #4 - Data Structure Course !!\n");
    printf("********* GRAPH PROJECT *********\n\n");
    L buildings = NULL;
    course Courses = NULL;
    B path = NULL;
    CPre SortedCourses = NULL;
    char Source[20],Destination[20];
    int option = 0;
    displayMenu();
    scanf("%d",&option);
    int sorted = 0;
    while(option != 1 && option != 6){
        printf("You Must Load The Files Firstly..\n");
        displayMenu();
        scanf("%d",&option);
    }
    while(option != 6){
        switch(option){
            case 1 : buildings = LoadBuildings();
                     Courses = LoadCourses();
                     sorted = 0;
                     printf("The Files Loaded Successfully..\n");
                     break;
            case 2 : printf("Enter two buildings separated by SPACE as [Source   Destination]\n");
                     scanf("%s",Source);
                     scanf("%s",Destination);
                     printf("----------------------------------------------------------\n");
                     if(SearchNode(buildings,Source) == NULL){
                        printf("The source building is not exist..\n");
                        break;
                     }
                     else if(SearchNode(buildings,Destination) == NULL){
                        printf("The Destination building is not exist..\n ");
                        break;
                     }
                     path = Dijkstra(buildings,Source,Destination);
                     int distance = CalculateDistance(path,Destination);
                     if(distance != INF)
                        printf("The Shortest Distance between %s and %s is ==> %d\n",Source,Destination,distance);
                    printf("----------------------------------------------------------\n");
                     break;
            case 3 : printf("Enter two buildings separated by SPACE as [Source   Destination]\n");
                     scanf("%s",Source);
                     scanf("%s",Destination);
                     if(SearchNode(buildings,Source) == NULL){
                        printf("The source building is not exist..\n");
                        break;
                     }
                     else if(SearchNode(buildings,Destination) == NULL){
                        printf("The Destination building is not exist..\n ");
                        break;
                     }
                     printf("----------------------------------------------------------\n");
                     path = Dijkstra(buildings,Source,Destination);
                     distance = CalculateDistance(path,Destination);
                     if(distance != INF){
                        printf("The Shortest Distance between %s and %s is ==> %d\n",Source,Destination,distance);
                        printf("By This Path :\n");
                     }
                     PrintPath(path);
                     printf("----------------------------------------------------------\n");
                     break;
            case 4 : if(sorted == 0){
                        SortedCourses = Topological(Courses);
                        printf("The Courses are sorted by Topological Algorithm..\n");
                        sorted = 1;
                     }
                     else
                        printf("The courses are already sorted..\n");
                     break;
            case 5 : if(SortedCourses == NULL){
                        printf("You Must Sort the Courses before Print them, choose option 4 Firstly..\n");
                        break;
                     }
                     printf("----------------------------------------------------------\n");
                     printSortedCourses(SortedCourses);
                     printf("----------------------------------------------------------\n");
                     break;
            default : printf("Invalid Option, Try Again ...\n");
        }
        displayMenu();
        scanf("%d",&option);
    }
        printf("Thank You For using My Application..\n");
        printf("Exit From The Application...\n");

        return 0;
}


//function to display the menu
void displayMenu(){
    printf("------ Choose the Option ------\n");
    printf("1.  Load the two files.\n");
    printf("2.  Calculate the shortest distance between two buildings.\n");
    printf("3.  Print the shortest route between two buildings and the total distance.\n");
    printf("4.  Sort the courses using the topological sort.\n");
    printf("5.  Print the sorted courses.\n");
    printf("6.  Exit the application.\n");
}

//function to create the heap
Heap CreateHeap(int maxSize){
    Heap H = (Heap)malloc(sizeof(struct minHeap));
    H->heapArray = (cell*)malloc(sizeof(cell)*(maxSize+1));

    if(H == NULL){
        printf("Out of memory..\n");
        return NULL;
    }
    H->max_size = maxSize+1;
    H->size = 0;
    H->heapArray[0].build_name = NULL;
    H->heapArray[0].distance = -1;

    return H;
}

//function to find the parent of a node in the heap
int parent(int pos){
    return (pos/2);
}

//function to find the left child for such node in heap
int leftChild(int pos){
    return (pos*2);
}

//function to find the right child for such node in heap
int rightChild(int pos){
    return ((pos*2) + 1);
}

//function that return true if the node in heap is leaf or false if not leaf
int isLeaf(int pos, Heap H){
    if(leftChild(pos) <= H->size || rightChild(pos) <= H->size)
        return 0;
    return 1;
}

//function to swap two nodes in the heap
Heap swap(int pos1, int pos2, Heap H){
    cell temp;
    temp.build_name = strdup(H->heapArray[pos1].build_name);
    temp.distance = H->heapArray[pos1].distance;

    H->heapArray[pos1].build_name = strdup(H->heapArray[pos2].build_name);
    H->heapArray[pos1].distance = H->heapArray[pos2].distance;

    H->heapArray[pos2].build_name = strdup(temp.build_name);
    H->heapArray[pos2].distance = temp.distance;

    return H;
}

//function to insert new node to the heap
Heap insertToHeap(char* name, int distance, Heap H){
    if(H->size >= H->max_size)
        return H;
    int current = ++H->size;
    H->heapArray[current].build_name = name;
    H->heapArray[current].distance = distance;

    while(H->heapArray[current].distance < H->heapArray[parent(current)].distance){
        H = swap(current,parent(current),H);
        current = parent(current);
    }
    return H;
}

//function that delete node from the heap and minheapify the heap then return it's value
cell deleteFromHeap(Heap H){
    if(H->size == 0){
        printf("The Heap is empty!!\n");
        return;
    }
    cell temp;
    temp.build_name = strdup(H->heapArray[1].build_name);
    temp.distance = H->heapArray[1].distance;
    int current = 1;
    while(!isLeaf(current,H)){
        if(rightChild(current) <= H->size && leftChild(current) <= H->size){
            if(H->heapArray[leftChild(current)].distance < H->heapArray[rightChild(current)].distance){
                H = swap(current,leftChild(current),H);
                current = leftChild(current);
            }
            else{
                H = swap(current,rightChild(current),H);
                current = rightChild(current);
            }
        }
        else if(leftChild(current) <= H->size){
            H = swap(current,leftChild(current),H);
            current = leftChild(current);
        }
        else{
            H = swap(current,rightChild(current),H);
            current = rightChild(current);
        }
    }
    while(current < H->size){
        H = swap(current,current+1,H);
        current = current + 1;
    }

    H->size--;
    return temp;
}


//function to minHeapify the heap to be valid minHeap
Heap minHeapify(int pos, Heap H){
    if(!isLeaf(pos,H)){
        if(leftChild(pos) <= H->size && rightChild(pos) <= H->size){
            if(H->heapArray[pos].distance > H->heapArray[leftChild(pos)].distance || H->heapArray[pos].distance > H->heapArray[rightChild(pos)].distance){
                if(H->heapArray[leftChild(pos)].distance < H->heapArray[rightChild(pos)].distance){
                    H = swap(pos,leftChild(pos),H);
                    H = minHeapify(leftChild(pos),H);
                }
                else {
                    H = swap(pos,rightChild(pos),H);
                    H = minHeapify(rightChild(pos),H);
                }
            }
        }
        else if(leftChild(pos) <= H->size && H->heapArray[leftChild(pos)].distance < H->heapArray[pos].distance){
            H = swap(pos,leftChild(pos),H);
            H = minHeapify(leftChild(pos),H);
        }
        else if(rightChild(pos) <= H->size && H->heapArray[rightChild(pos)].distance < H->heapArray[pos].distance){
            H = swap(pos,rightChild(pos),H);
            H = minHeapify(rightChild(pos),H);
        }
    }
    return H;
}


//function that create an empty linked list of buildings
B CreateBuildings(){
    B build = (B)malloc(sizeof(struct Building));
    if(build == NULL){
        printf("Out of the Space..\n");
        return NULL;
    }
    build->next = NULL;
    return build;
}

//function that create an empty linked list of list of buildings
L CreateList(){
    L list = (L)malloc(sizeof(struct List));
    if(list == NULL){
        printf("Out of the Space..\n");
        return NULL;
    }
    list->next = NULL;
    return list;
}

//function to insert to building linked list
void InsertToBuildings(B build,char* building, int distance){
    //create the new node
    B NEW = (B)malloc(sizeof(struct Building));
    if(NEW == NULL)
        printf("Out of Space..\n");
    else{
        NEW->next = NULL;
        NEW->building = strdup(building);
        NEW->distance = distance;
        B ptr = build;
        //find the last node to insert after it
        while(ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = NEW;
    }
}

//function to insert to the list of buildings
void InsertToList(L list,char* building){
    //create the new node
    L NEW = (L)malloc(sizeof(struct List));
    if(NEW == NULL)
        printf("Out of Space..\n");
    else{
        NEW->next = NULL;
        NEW->build_name = strdup(building);
        NEW->Buildings = CreateBuildings();
        L ptr = list;
        //find the last node to insert after it
        while(ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = NEW;
    }
}

//function to create an empty linked list of courses
course CreateCourse(){
    course C = (course)malloc(sizeof(struct Course));
    if(C == NULL){
        printf("Out of the Space..\n");
        return NULL;
    }
    C->next = NULL;
    return C;
}

//function to create an empty linked list of prerequisites courses
CPre CreateCoursePre(){
    CPre C = (CPre)malloc(sizeof(struct CoursesPrerequiest));
    if(C == NULL){
        printf("Out of the Space..\n");
        return NULL;
    }
    C->next = NULL;
    return C;
}

//function to insert to linked list of courses
void InsertToCourses(course list,char* c_name){
    //create the new node
    course NEW = (course)malloc(sizeof(struct Course));
    if(NEW == NULL)
        printf("Out of Space..\n");
    else{
        NEW->next = NULL;
        NEW->course = strdup(c_name);
        NEW->inDegree = 0;
        NEW->Visited = 0;
        NEW->Courses = CreateCoursePre();
        course ptr = list;
        //find the last node to insert after it
        while(ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = NEW;
        NumOfCourses++;
    }
}

//function to insert to linked list of prerequisites of courses
void InsertToPreCourses(CPre list,char* c_name){
    //create the new node
    CPre NEW = (CPre)malloc(sizeof(struct CoursesPrerequiest));
    if(NEW == NULL)
        printf("Out of Space..\n");
    else{
        NEW->next = NULL;
        NEW->course = strdup(c_name);
        CPre ptr = list;
        //find the last node to insert after it
        while(ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = NEW;
    }
}

//function to create an empty queue
ptq createQueue(){
    ptq T = (ptq)malloc(sizeof(struct queue));
    if(T==NULL){
        printf("Queue can not be defined..\n");
        return NULL;
        }
    else {
        T->next = NULL;
        T->front = T->next;
        T->rear = T->next;
        return T;
        }
}

//function to enqueue node to queue
void EnQueue(ptq Q, char* tok){
    if(Q == NULL)
        printf("Queue not exist..\n");
    else {
            ptq New = (ptq)malloc(sizeof(struct queue));
            New->token = strdup(tok);
            New->front = NULL;
            New->rear = NULL;
            ptq temp = Q->rear;
            if(temp != NULL){
                temp->next = New;
                New->next = NULL;
                Q->rear = New;
                Q->front = Q->next;
            }
            else {
                Q->next = New;
                New->next = NULL;
                Q->front = New;
                Q->rear = New;
            }
    }

}

//function to dequeue node from queue
ptq DeQueue(ptq Q){
    if(Q == NULL){
        printf("Queue not exist..\n");
        return NULL;
    }
    else {
        if(Q->front == NULL){
            return NULL;
        }
        else {
            ptq temp = Q->front;
            Q->next = temp->next;
            Q->front = Q->next;
            if(Q->front==NULL)
                Q->rear = NULL;
            temp->next = NULL;
            return temp;
        }
    }
}

//function to search for node in building linked list
L SearchNode(L list,char* building){
    L ptr = list->next;
    while(ptr != NULL){
        if(strcasecmp(ptr->build_name,building) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

//function to search about course in courses linked list
course SearchCourse(course list,char* c_name){
    course ptr = list->next;
    while(ptr != NULL){
        if(strcasecmp(ptr->course,c_name) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

//function to search about course in Prerequisites courses linked list
course SearchPreCourse(CPre list,char* c_name){
    course ptr = list->next;
    while(ptr != NULL){
        if(strcasecmp(ptr->course,c_name) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}

//function to load the building from the file
//and implementation of the graph using contingency list - linked list of linked list -
L LoadBuildings(){
    L list = CreateList();
    FILE* input = fopen("input_buildings.txt","r");
    if(input == NULL)
        printf("The File Can't Be Open..\n");
    else{
        char myString[100];
        while(fgets(myString,100,input) != NULL){
            char* b1 = strtok(myString,"#");
            char* b2 = strtok(NULL,"#");
            int dis = atoi(strtok(NULL,"#"));
            L temp = SearchNode(list,b1);
            if(temp == NULL)
                InsertToList(list,b1);
            if(SearchNode(list,b2) == NULL)
                InsertToList(list,b2);
            temp = SearchNode(list,b1);
            InsertToBuildings(temp->Buildings,b2,dis);
        }
        fclose(input);
    }
        return list;
}

//function to load the courses from the file
//and implementation of the graph using contingency list - linked list of linked list -
course LoadCourses(){
    course list = CreateCourse();
    FILE* input = fopen("input_courses.txt","r");
    if(input == NULL)
        printf("The File Can't Be Open..\n");
    else{
        char myString[100];
        while(fgets(myString,100,input) != NULL){
            char* c = strtok(myString,"#");
            if(c[strlen(c)-1] == '\n')
                c[strlen(c)-1] = '\0' ;
            course temp = SearchCourse(list,c);
            if(temp == NULL)
                InsertToCourses(list,c);
            temp = SearchCourse(list,c);
            c = strtok(NULL,"#");
            if(c[strlen(c)-1] == '\n')
                c[strlen(c)-1] = '\0' ;
            while(c != NULL){
                if(SearchCourse(list,c) == NULL)
                    InsertToCourses(list,c);
                if(SearchPreCourse(temp->Courses,c) == NULL){
                    InsertToPreCourses(temp->Courses,c);
                    temp->inDegree++;
                }
                c = strtok(NULL,"#");
                if(c != NULL && c[strlen(c)-1] == '\n')
                    c[strlen(c)-1] = '\0' ;
            }
        }
        fclose(input);
    }
        return list;
}

//function to search the adjacent of the vertix in the graph
CPre SearchAdjacent(CPre C, char* token){
    CPre ptr = C->next;
    while(ptr != NULL){
        if(strcasecmp(ptr->course,token) == 0)
            break;
        ptr = ptr->next;
    }
    return ptr;
}

//function to sort the courses using topological algorithm
void topoSort(course C, ptq Q, int x){
    if(x!=0){
        course ptr = C->next;
        while(ptr != NULL){
            if(ptr->inDegree == 0 && ptr->Visited == 0) //the InDegree is 0 and not visited
                break;
            ptr = ptr->next;
        }
        if(ptr == NULL){
            printf("The Graph has a Cycle!!\n"); //if there is no vertix with  inDegree 0, then the graph is cyclic
            return;
        }
        EnQueue(Q,ptr->course); //add the vertix with inDegree zero to the Queue
        ptr->Visited = 1;      //change this vertix to Visited (true)
        course temp = C->next;
        while(temp != NULL){   //find the vertices that adjacent to the one that added to the queue
            if(SearchAdjacent(temp->Courses,ptr->course)!=NULL)
                temp->inDegree--;    //decrement their inDegree by one
            temp = temp->next;
        }
        topoSort(C,Q,x-1);  //then repeat this by number of vertices
    }

}
//function that adjust the topological sort function
CPre Topological(course C){
    int counter = 0;
    course crs = C->next;
    while(crs != NULL){  //find the number of courses
        counter++;
        crs = crs->next;
    }

    ptq Q = createQueue(); //create the queue
    CPre SortedList = CreateCoursePre();
    topoSort(C,Q,counter); //topological sort for this courses
    return SortedList;
}

//function to print the courses as they sorted by topological
void printSortedCourses(CPre list){
    CPre temp = list->next;
    while(temp != NULL){
        printf("%s\n",temp->course);
        temp = temp->next;
    }
}

//function that find the number of vertices in the building graph (number pf buildings)
int findNumofVertices(L list){
    L ptr = list->next;
    int counter = 0;
    while(ptr != NULL){
        counter++;
        ptr = ptr->next;
    }
    return counter;
}

//function to apply Dijkstra algorithm to find the shortest path between two buildings
B Dijkstra(L list, char* Source, char* Distenation){
    int size = findNumofVertices(list);
    char* dest = strdup(Distenation);
    DMatrix D[size];
    Heap H = CreateHeap(size);
    L ptr = list->next;
    int i = 0;
    L src = SearchNode(list,Source);
    cell temp;
    B adjacent;
    int ParentCost = 0;
    int newCost = 0;
    B SortedList = CreateBuildings();
    if(src == NULL){
        printf("Error.., the Source is not exist..\n");
        return NULL;
    }
    while(ptr != NULL){
        //add all buildings to DijkstraMatrix and to the MinHeap with initialize all its values as follows
        if(ptr == src){
            H = insertToHeap(ptr->build_name,0,H);
            D[i].build_name = ptr->build_name;
            D[i].known = 0;
            D[i].cost = 0;
            D[i].path = Source;
        }
        else {
            H = insertToHeap(ptr->build_name,INF,H);
            D[i].build_name = ptr->build_name;
            D[i].known = 0;
            D[i].cost = INF;
            D[i].path = NULL;
        }

        ptr = ptr->next;
        i++;
    }

    while(H->size > 0){

        temp = deleteFromHeap(H); // get the minimum

        for(int h = 0; h < size; h++){
            if(strcasecmp(D[h].build_name,temp.build_name) == 0 && D[h].known == 0){
                ParentCost = D[h].cost;
                D[h].known = 1;
                InsertToBuildings(SortedList,temp.build_name,ParentCost);
                break;
            }
        }

        adjacent = SearchNode(list,temp.build_name)->Buildings->next;
        while(adjacent != NULL){
            for(int k = 0; k < size; k++){
                if(strcasecmp(D[k].build_name,adjacent->building) == 0 && D[k].known == 0){
                    newCost = ParentCost + adjacent->distance;
                        if(newCost < D[k].cost && D[k].known == 0){
                            D[k].cost = newCost;
                            D[k].path = strdup(temp.build_name);
                            for(int s = 1; s <= H->size; s++){
                                if(strcasecmp(H->heapArray[s].build_name,adjacent->building) == 0){
                                    H->heapArray[s].distance = newCost;
                                    for(int w = 1; w <= H->size; w++)
                                        H = minHeapify(w,H);
                                    break;
                                }
                            }
                        }
                    break;
                }
            }
            adjacent = adjacent->next;
        }
    }
        B Path = CreateBuildings();


        int counter = size;
        while(strcasecmp(Distenation,Source) != 0 && counter > 0 ){
            for(int l = 0; l < size; l++){
                if(strcasecmp(Distenation,D[l].build_name) == 0){
                    B tmp = (B)malloc(sizeof(struct Building));
                    tmp->building = strdup(D[l].build_name);
                    tmp->distance = D[l].cost;
                    tmp->next = Path->next;
                    Path->next = tmp;
                    Distenation = strdup(D[l].path);
                    break;
                }
            }
            counter--;
        }

        if(strcasecmp(Distenation,Source) != 0){
            printf("You can't arrive from %s to %s..\n",Source,dest);
            return NULL;
        }

        B tmp = (B)malloc(sizeof(struct Building));
        tmp->building = strdup(Source);
        tmp->distance = 0;
        tmp->next = Path->next;
        Path->next = tmp;

    return Path;
}

//function to print the shortest path between two buildings
void PrintPath(B path){
    if(path != NULL){
        B ptr = path->next;
        while(ptr != NULL){
            printf("%s ",ptr->building);
            ptr = ptr->next;
            if(ptr != NULL)
                printf("==> ");
        }
        printf("\n");
    }
}

//function that calculate the shortest distance between two buildings
int CalculateDistance(B path, char* Distenation){
    if(path != NULL){
        B ptr = path->next;
        while(ptr != NULL){
            if(strcasecmp(Distenation,ptr->building) == 0)
                return ptr->distance;
            ptr = ptr->next;
        }
    }
    return INF;
}
