/* ==========================================================
   TRAIN MANAGEMENT SYSTEM - COMPLETE PROJECT
   Combines Review 1 + Review 2 + Review 3 in full, unmodified.

   Review 1 : Stack (Detached Coaches) + Queue (Waiting Passengers)
   Review 2 : Linked List (Station Route)
   Review 3 : Tree (BST - Train Records) + Graph (BFS - Rail Network)
              + Searching (Linear Search) + Hashing (Passenger Table)
   ========================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 10
#define HASH_SIZE 10

/* ============================================================
   REVIEW 1 : STACK FOR DETACHED COACHES
   ============================================================ */

struct Stack {
    char coach[50];
    struct Stack *next;
};

struct Stack *top = NULL;

/* Push coach into stack */
void push(char coach[]) {
    struct Stack *newNode;

    newNode = (struct Stack *)malloc(sizeof(struct Stack));

    strcpy(newNode->coach, coach);
    newNode->next = top;
    top = newNode;

    printf("Coach %s detached and pushed into stack.\n", coach);
}

/* Pop coach from stack */
void pop() {
    struct Stack *temp;

    if (top == NULL) {
        printf("No detached coaches available.\n");
        return;
    }

    temp = top;

    printf("Coach %s restored from stack.\n", temp->coach);

    top = top->next;
    free(temp);
}

/* Display stack */
void displayStack() {
    struct Stack *temp = top;

    if (top == NULL) {
        printf("Detached coach stack is empty.\n");
        return;
    }

    printf("\nDetached Coach Stack (TOP -> BOTTOM):\n");

    while (temp != NULL) {
        printf("%s\n", temp->coach);
        temp = temp->next;
    }
}


/* ============================================================
   REVIEW 1 : QUEUE FOR PASSENGERS
   ============================================================ */

struct Queue {
    char passenger[50];
    struct Queue *next;
};

struct Queue *front = NULL;
struct Queue *rear = NULL;

/* Enqueue passenger */
void enqueue(char passenger[]) {
    struct Queue *newNode;

    newNode = (struct Queue *)malloc(sizeof(struct Queue));

    strcpy(newNode->passenger, passenger);
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }

    printf("Passenger %s added to waiting queue.\n", passenger);
}

/* Dequeue passenger */
void dequeue() {
    struct Queue *temp;

    if (front == NULL) {
        printf("Passenger queue is empty.\n");
        return;
    }

    temp = front;

    printf("Passenger %s is now boarding.\n",
           temp->passenger);

    front = front->next;

    if (front == NULL)
        rear = NULL;

    free(temp);
}

/* Display queue */
void displayQueue() {
    struct Queue *temp = front;

    if (front == NULL) {
        printf("Passenger queue is empty.\n");
        return;
    }

    printf("\nPassenger Queue (FRONT -> REAR):\n");

    while (temp != NULL) {
        printf("%s -> ", temp->passenger);
        temp = temp->next;
    }

    printf("END\n");
}


/* ============================================================
   REVIEW 2 : LINKED LIST FOR STATION ROUTE
   ============================================================ */

struct StationNode {
    char station[50];
    struct StationNode *next;
};
struct StationNode *routeHead = NULL;

/* Insert a station at the end of the route list */
void insertStation(char station[]) {
    struct StationNode *newNode = (struct StationNode *)malloc(sizeof(struct StationNode));
    strcpy(newNode->station, station);
    newNode->next = NULL;

    if (routeHead == NULL) {
        routeHead = newNode;
    } else {
        struct StationNode *temp = routeHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
    printf("Station %s added to route.\n", station);
}

/* Display the complete route from start to end */
void displayRoute() {
    struct StationNode *temp = routeHead;
    if (temp == NULL) { printf("Route list is empty.\n"); return; }
    printf("\nTrain Route (Start -> End):\n");
    while (temp != NULL) {
        printf("%s -> ", temp->station);
        temp = temp->next;
    }
    printf("END\n");
}

/* Delete a station from the route by name */
void deleteStation(char station[]) {
    struct StationNode *temp = routeHead, *prev = NULL;

    if (temp != NULL && strcmp(temp->station, station) == 0) {
        routeHead = temp->next;
        free(temp);
        printf("Station %s removed from route.\n", station);
        return;
    }

    while (temp != NULL && strcmp(temp->station, station) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) { printf("Station %s not found in route.\n", station); return; }

    prev->next = temp->next;
    free(temp);
    printf("Station %s removed from route.\n", station);
}


/* ============================================================
   REVIEW 3 : TREE (BST) FOR TRAIN RECORDS
   ============================================================ */

struct TrainNode {
    int trainNo;
    char trainName[50];
    struct TrainNode *left, *right;
};

struct TrainNode* insertTrain(struct TrainNode *root, int no, char name[]) {
    if (root == NULL) {
        struct TrainNode *newNode = (struct TrainNode *)malloc(sizeof(struct TrainNode));
        newNode->trainNo = no;
        strcpy(newNode->trainName, name);
        newNode->left = newNode->right = NULL;
        printf("Train %d (%s) added to records.\n", no, name);
        return newNode;
    }
    if (no < root->trainNo) root->left = insertTrain(root->left, no, name);
    else if (no > root->trainNo) root->right = insertTrain(root->right, no, name);
    return root;
}

void inorderDisplay(struct TrainNode *root) {
    if (root == NULL) return;
    inorderDisplay(root->left);
    printf("Train No: %d | Name: %s\n", root->trainNo, root->trainName);
    inorderDisplay(root->right);
}


/* ============================================================
   REVIEW 3 : GRAPH (BFS) FOR RAILWAY NETWORK
   ============================================================ */

int adjMatrix[MAX_STATIONS][MAX_STATIONS];
char stationNames[MAX_STATIONS][50];
int stationCount = 0;

int getStationIndex(char name[]) {
    for (int i = 0; i < stationCount; i++)
        if (strcmp(stationNames[i], name) == 0) return i;
    strcpy(stationNames[stationCount], name);
    return stationCount++;
}

void addEdge(char a[], char b[]) {
    int i = getStationIndex(a);
    int j = getStationIndex(b);
    adjMatrix[i][j] = 1;
    adjMatrix[j][i] = 1;
    printf("Rail link added between %s and %s.\n", a, b);
}

void bfsTraversal(char start[]) {
    int visited[MAX_STATIONS] = {0};
    int queueArr[MAX_STATIONS], qf = 0, qr = 0;
    int startIdx = getStationIndex(start);

    visited[startIdx] = 1;
    queueArr[qr++] = startIdx;

    printf("\nBFS Reachable Stations from %s:\n", start);
    while (qf < qr) {
        int curr = queueArr[qf++];
        printf("%s ", stationNames[curr]);
        for (int i = 0; i < stationCount; i++) {
            if (adjMatrix[curr][i] == 1 && !visited[i]) {
                visited[i] = 1;
                queueArr[qr++] = i;
            }
        }
    }
    printf("\n");
}


/* ============================================================
   REVIEW 3 : SEARCHING - LINEAR SEARCH ON PASSENGER LIST
   ============================================================ */

char passengerList[50][50];
int passengerCount = 0;

void addPassengerToList(char name[]) {
    strcpy(passengerList[passengerCount++], name);
    printf("Passenger %s added to list.\n", name);
}

void searchPassenger(char name[]) {
    for (int i = 0; i < passengerCount; i++) {
        if (strcmp(passengerList[i], name) == 0) {
            printf("Passenger %s found at position %d.\n", name, i + 1);
            return;
        }
    }
    printf("Passenger %s not found in list.\n", name);
}


/* ============================================================
   REVIEW 3 : HASHING - PASSENGER LOOKUP TABLE (CHAINING)
   ============================================================ */

struct HashNode {
    char passenger[50];
    char seatNo[10];
    struct HashNode *next;
};
struct HashNode *hashTable[HASH_SIZE];

int hashFunction(char name[]) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) sum += name[i];
    return sum % HASH_SIZE;
}

void insertHash(char name[], char seat[]) {
    int idx = hashFunction(name);
    struct HashNode *newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
    strcpy(newNode->passenger, name);
    strcpy(newNode->seatNo, seat);
    newNode->next = hashTable[idx];
    hashTable[idx] = newNode;
    printf("Passenger %s hashed to bucket %d with seat %s.\n", name, idx, seat);
}

void searchHash(char name[]) {
    int idx = hashFunction(name);
    struct HashNode *temp = hashTable[idx];
    while (temp != NULL) {
        if (strcmp(temp->passenger, name) == 0) {
            printf("Found in bucket %d -> Passenger: %s | Seat: %s\n", idx, temp->passenger, temp->seatNo);
            return;
        }
        temp = temp->next;
    }
    printf("Passenger %s not found in hash table.\n", name);
}


/* ============================================================
   MAIN FUNCTION - COMBINED MENU FOR ALL THREE REVIEWS
   ============================================================ */

int main() {

    int choice, trainNo;
    char name[50], name2[50], seat[10];
    struct TrainNode *trainRoot = NULL;

    do {
        printf("\n===================================================\n");
        printf("            TRAIN MANAGEMENT SYSTEM\n");
        printf("===================================================\n");
        printf(" 0. Exit\n");
        printf(" -- Review 1 : Stack & Queue --\n");
        printf(" 1. Detach Coach (Push)\n");
        printf(" 2. Restore Coach (Pop)\n");
        printf(" 3. Display Detached Coaches\n");
        printf(" 4. Add Passenger to Queue (Enqueue)\n");
        printf(" 5. Board Passenger Queue (Dequeue)\n");
        printf(" 6. Display Passenger Queue\n");
        printf(" -- Review 2 : Linked List --\n");
        printf(" 7. Add Station to Route\n");
        printf(" 8. Display Route\n");
        printf(" 9. Remove Station from Route\n");
        printf(" -- Review 3 : Tree / Graph / Search / Hash --\n");
        printf("10. Add Train Record (BST)\n");
        printf("11. Display Train Records (Inorder)\n");
        printf("12. Add Rail Link (Graph)\n");
        printf("13. BFS Traversal from Station\n");
        printf("14. Add Passenger to List\n");
        printf("15. Search Passenger (Linear Search)\n");
        printf("16. Add Passenger to Hash Table\n");
        printf("17. Search Passenger in Hash Table\n");
        printf("===================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 0:
                printf("\nProgram ended successfully.\n");
                break;

            /* ---- Review 1 : Stack ---- */
            case 1:
                printf("Enter coach name: ");
                scanf("%s", name);
                push(name);
                break;

            case 2:
                pop();
                break;

            case 3:
                printf("option 3 selected\n");
                displayStack();
                break;

            /* ---- Review 1 : Queue ---- */
            case 4:
                printf("Enter passenger name: ");
                scanf("%s", name);
                enqueue(name);
                break;

            case 5:
                dequeue();
                break;

            case 6:
                displayQueue();
                break;

            /* ---- Review 2 : Linked List ---- */
            case 7:
                printf("Enter station name: ");
                scanf("%s", name);
                insertStation(name);
                break;

            case 8:
                displayRoute();
                break;

            case 9:
                printf("Enter station name to remove: ");
                scanf("%s", name);
                deleteStation(name);
                break;

            /* ---- Review 3 : Tree (BST) ---- */
            case 10:
                printf("Enter train number: ");
                scanf("%d", &trainNo);
                printf("Enter train name: ");
                scanf("%s", name);
                trainRoot = insertTrain(trainRoot, trainNo, name);
                break;

            case 11:
                inorderDisplay(trainRoot);
                break;

            /* ---- Review 3 : Graph (BFS) ---- */
            case 12:
                printf("Enter first station: ");
                scanf("%s", name);
                printf("Enter second station: ");
                scanf("%s", name2);
                addEdge(name, name2);
                break;

            case 13:
                printf("Enter starting station: ");
                scanf("%s", name);
                bfsTraversal(name);
                break;

            /* ---- Review 3 : Searching ---- */
            case 14:
                printf("Enter passenger name: ");
                scanf("%s", name);
                addPassengerToList(name);
                break;

            case 15:
                printf("Enter passenger name to search: ");
                scanf("%s", name);
                searchPassenger(name);
                break;

            /* ---- Review 3 : Hashing ---- */
            case 16:
                printf("Enter passenger name: ");
                scanf("%s", name);
                printf("Enter seat number: ");
                scanf("%s", seat);
                insertHash(name, seat);
                break;

            case 17:
                printf("Enter passenger name to search: ");
                scanf("%s", name);
                searchHash(name);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);

    return 0;
}
