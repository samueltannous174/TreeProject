#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct AVLnode {
    char Course[100];
    int CreditHours;                //strcuct of node nodes
    char CourseCode[100];
    char department[100];
    char topics[100][100];
    int topicNumber;
    struct AVLnode *Left;
    struct AVLnode *Right;
    int Height;
};

typedef struct AVLnode *AVL_T;  //typedef of the avl


void fileRead(AVL_T root);
void fileWrite(AVL_T root);
AVL_T insert(AVL_T t, AVL_T NewTree);
void printPreOrder(AVL_T t);
void insertUserTree(AVL_T root);
AVL_T findTree(AVL_T tree, char code[]);
void printSpecficNode(AVL_T t);
void printInOrder(AVL_T root);
AVL_T minValueNode(AVL_T node);
int getBalanceFactor(AVL_T N);
void findByDepartment(AVL_T t,char department[100]);
AVL_T deleteByDepartment(AVL_T t, char department[100]);
AVL_T delete(AVL_T root, char code[100]);
void copyNode(AVL_T, AVL_T);
AVL_T deleteCourseStartingWithChar(AVL_T root, char letter);
void printInFile(AVL_T t,FILE *fptr);

int main() {
    AVL_T root = (AVL_T) malloc(sizeof(struct AVLnode));
    root->Left = NULL;             // create the root of the node
    root->Right = NULL;
    root->Height = 0;

    while (1) {
        int chosenNumber;
        printf("please enter number between 1-10: \n"
               "1. Read the file courses.txt file and create the node.\n"
               "2. Insert a new course from the user with all its associated data.\n"
               "3. Find a course and support updating of its information.\n"
               "4. List courses in lexicographic order with their associated\n"
               "information (credit hours, IDs, and topics).\n"
               "5. List all topics associated with a given course.\n"
               "6. List all courses in lexicographic order that belong to the same\n"
               "department.\n"
               "7. Delete a course.\n"
               "8. Delete all courses that start with a specific letter.\n"
               "9. Delete all courses belong to a given department.\n"
               "10. Save all words in file offered_courses.txt\n");
        scanf("%d", &chosenNumber);
        if (chosenNumber == 1) {
            fileRead(root);
            printPreOrder(root);


        }
        if (chosenNumber == 2) {
            insertUserTree(root);
        }
        if (chosenNumber == 3) {
            char code[100];

            printf("please enter the code want to find\n");
            scanf("%s", code);
            if (findTree(root, code) != NULL) {
                AVL_T foundTree = findTree(root, code);      // if the node found print it
                printf(" node found :     ");
                printSpecficNode(foundTree);
                printf("\n");
                int updateChoice;

                printf(" if you want to update its information press 1 if you dont want press 0: \n");
                scanf("%d", &updateChoice);

                if (updateChoice == 1) {                  // if 1 chosen to update the old one will be deleted and new one inserted according that  the course code will be change
                    delete(foundTree,foundTree->CourseCode);
                    insertUserTree(root);
                    printf("updated node:     ");


                }
                if (updateChoice == 0) {
                    printf("");
                }
            } else{
                printf("node not found\n");    // not found the tree wanted
            }
        }
        if (chosenNumber == 4) {
            printInOrder(root); // print with lexicographic order

        }

        if (chosenNumber == 5) {
            char code[100];
            printf("please enter the course code you want to print its topics\n");
            scanf("%s", code);
            AVL_T foundTree = findTree(root, code);
            printf("the topics of this course : \n");
            if (foundTree != NULL) {
                for (int i = 0; i < foundTree->topicNumber; ++i) {
                    printf("%s  \n", foundTree->topics[i]);      // print all topics of specific    course
                }
            }
            else{
                printf("node not found\n");
            }

        }
        if (chosenNumber == 6) {
            char department[100];
            printf("please enter the department you want to print its courses\n");
            gets( department);
            gets( department);
            findByDepartment(root,department);

        }
        if (chosenNumber == 7) {
            printf("please enter course code of the course want to delete \n");
            char code[100];
            gets(code);
            gets(code);
            delete(root, code);
        }

        if (chosenNumber == 8) {
            char letter;
            printf("enter starting letter from course to delete\n");
            scanf("%c", &letter);
            scanf("%c", &letter);
            root = deleteCourseStartingWithChar(root, letter);
        }
        if (chosenNumber == 9) {

            char department[100];
            printf("please enter the department you want to delete its courses\n");
            gets( department);
            gets( department);
            root = deleteByDepartment(root,department);

        }
        if (chosenNumber == 10) {
            fileWrite(root);

        }
    }
}
void fileWrite(AVL_T t){
    FILE *fptr;

    // opening file in writing mode
    fptr = fopen("program.txt", "w");

    // exiting program
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }
    printInFile(t,fptr);
    fclose(fptr);
}
void printInFile(AVL_T t,FILE *fptr){
    if(t==NULL){
        return;
    }
    fprintf(fptr, "%s %d %s %s     ", t->Course, t->CreditHours, t->CourseCode, t->department );
    for (int i = 0; i <t->topicNumber ; ++i) {
        fprintf(fptr,"%s    ",t->topics[i]);
    }
    fprintf(fptr,"\n");
    printInFile(t->Left,fptr);
    printInFile( t->Right,fptr);
}
void  fileRead(AVL_T root) {
    FILE *fptr;
    char tempFileLine[1000];
    fptr = fopen("courses.txt", "r");
    if (fptr == NULL) {                         //open the file and getting all courses of the file
        printf("there is no  file with that name");
        return;
    }
    char *tempToken;
    int nodeNumber = 0;   //to know the number of the current node and know the first node to make it the root

    while (fgets(tempFileLine, 200, fptr) != NULL) {
        if (strcmp(tempFileLine, "") == 0 || strcmp(tempFileLine, "\n") == 0) {   //reading every line of the file
            break;
        }

        AVL_T node = (AVL_T) malloc(sizeof(struct AVLnode));            // node to save every line properties
        node->Right = NULL;
        node->Left = NULL;


        strcpy(node->Course, strtok(tempFileLine, ":"));     // split all properties to save it in nodes
        node->CreditHours = atoi(strtok(NULL, "#"));
        strcpy(node->CourseCode, strtok(NULL, "#"));
        strcpy(node->department, strtok(NULL, "/"));
        int currentTopic = 0;                        // to get all topics number

        tempToken = strtok(NULL, ",");
        while (tempToken != NULL) {
            strcpy(node->topics[currentTopic], tempToken);
            currentTopic++;
            tempToken = strtok(NULL, ",");
        }
        node->topicNumber = currentTopic;
        if (nodeNumber == 0) {      // if its the first node make it the root
            strcpy(root->Course, node->Course);
            root->CreditHours = node->CreditHours;
            strcpy(root->CourseCode, node->CourseCode);
            strcpy(root->department, node->department);

            for (int i = 0; i < currentTopic; ++i) {

                strcpy(root->topics[i], node->topics[i]);
            }
            root->topicNumber = node->topicNumber;

        } else {
            insert(root, node);    //insert the nodes to the root if its not the first line
        }

        nodeNumber++; //increase  the number of node

    }
}

int getHeight(AVL_T t) {
    if (t == NULL) {
        return -1;
    }
    return t->Height;
}

int MAX(int x, int y) {
    if (x > y)
        return x;
    if (y > x)
        return y;
    return -1;
}

AVL_T singleRotateLeft(AVL_T x) {
    AVL_T y;
    y = x->Right;
    x->Right = y->Left;
    y->Left = x;

    x->Height = 1 + MAX(getHeight(x->Left), getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x), getHeight(y->Right));

    return y;
}



////////////////////////////////////////////////
AVL_T singleRotateRight(AVL_T y) {
    AVL_T x;
    x = y->Left;
    y->Left = x->Right;
    x->Right = y;

    y->Height = 1 + MAX(getHeight(y->Right), getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y), getHeight(x->Left));

    return x;
}

AVL_T doubleRotateLeft(AVL_T K3) {
    K3->Left = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}

AVL_T doubleRotateRight(AVL_T K3) {
    K3->Right = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}

AVL_T insert(AVL_T t, AVL_T newTree) {
    if (t == NULL) {
        t = newTree;
    }
    if (strcmp(newTree->CourseCode, t->CourseCode) < 0) {
        t->Left = insert(t->Left, newTree);
        if (getHeight(t->Left) - getHeight(t->Right) >= 2) {
            if (t->CourseCode < t->Left->CourseCode)
                t = singleRotateRight(t);
            else
                t = doubleRotateLeft(t);
        }
    } else if (strcmp(newTree->CourseCode, t->CourseCode) > 0) {
        t->Right = insert(t->Right, newTree);
        if (getHeight(t->Right) - getHeight(t->Left) >= 2) {
            if (newTree->CourseCode > t->Right->CourseCode)
                t = singleRotateLeft(t);
            else
                t = doubleRotateRight(t);
        }
    }

    t->Height = 1 + MAX(getHeight(t->Right), getHeight(t->Left));

    return t;
}

void insertUserTree(AVL_T root) {

    AVL_T newTree = (AVL_T) malloc(sizeof(struct AVLnode));
    newTree->Left = NULL;      //getting all the properties in new node
    newTree->Right = NULL;
    newTree->Height = 0;
    printf("please enter course name \n");
    gets(newTree->Course);
    gets(newTree->Course);

    printf("please enter course credit \n");
    scanf("%d", &newTree->CreditHours);
    printf("please enter course code \n");
    scanf("%s", newTree->CourseCode);
    printf("please enter course department  \n");
    gets(newTree->department);
    gets(newTree->department);

    printf("please enter course topics number \n");
    scanf("%d", &newTree->topicNumber);

    for (int i = 0; i < newTree->topicNumber; ++i) {
        printf(" please enter course topics %d name: \n", i);
        scanf("%s",newTree->topics[i]);


    }
    insert(root, newTree); //add the node to the main tree
    printf("the node is added to the tree\n");

}

AVL_T findTree(AVL_T tree, char code[]) {

    if (tree == NULL) {
        return NULL;
    }
    if (strcmp(code, tree->CourseCode) < 0) //greater than x move to right
        return (findTree(tree->Left, code));
    else //Less than x move to left
    if (strcmp(code, tree->CourseCode) > 0)
        return (findTree(tree->Right, code));
    else
        return tree;

}

void printPreOrder(AVL_T t) {
    if (t == NULL) {
        return;
    }

    printf("%s %d %s %s   \n", t->Course, t->CreditHours, t->CourseCode, t->department);
    printf("its topics :  ");
    for (int i = 0; i < t->topicNumber; ++i) {
        printf("%s\t", t->topics[i]);
    }
    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("\n");


    printPreOrder(t->Left);
    printPreOrder(t->Right);

}

void printSpecficNode(AVL_T t) {
    if (t == NULL) {
        return;
    }
    printf("%s %d %s %s \n", t->Course, t->CreditHours, t->CourseCode, t->department);
    printf("its topics :    ");
    for (int i = 0; i < t->topicNumber; ++i) {
        printf("%s", t->topics[i]);
    }
    printf("\n");

}
void findByDepartment(AVL_T t ,char department[100]) {
    if (t == NULL) {              //move in all tree lexicographic order and if the node department as the same department wanted print it
        return;
    }
    findByDepartment(t->Left,department);


    if(strcmp(t -> department, department) == 0) {   //checking if equal
        printSpecficNode(t);

    }
    findByDepartment(t->Right, department);
}

void copyNode(AVL_T src, AVL_T dst) {
    strcpy(dst->Course, src->Course);
    dst->CreditHours = src->CreditHours;
    strcpy( dst->CourseCode, src->CourseCode);
    strcpy( dst->department, src->department);
    dst->topicNumber = src->topicNumber;

    for (int i = 0; i < dst->topicNumber; ++i) {
        strcpy(dst->topics[i], src->topics[i]);
    }
}

AVL_T delete(AVL_T root, char code[100]) {    //function to delete specfic node according to the code


    if (root == NULL)
        return root;


    if (strcmp(code, root->CourseCode) < 0)
        root->Left = delete(root->Left, code);
    else if (strcmp(code, root->CourseCode) > 0) {
        root->Right = delete(root->Right, code);
    } else {
        if ((root->Left == NULL) || (root->Right == NULL)) {
            AVL_T temp = root->Left ?
                         root->Left :
                         root->Right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVL_T temp = minValueNode(root -> Right);
            copyNode(temp, root);
            root->Right = delete(root->Right, temp->CourseCode);

        }
        if (root == NULL)
            return root;

    }
    root->Height = MAX(getHeight(root->Left), getHeight(root));
    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->Left) >= 0)
        return singleRotateRight(root);
// Left Right Case
    if (balance > 1 && getBalanceFactor(root->Left) < 0) {
        root->Left = singleRotateLeft(root->Left);
        return singleRotateRight(root);
    }
// Right Right Case
    if (balance < -1 && getBalanceFactor(root->Right) <= 0)
        return singleRotateLeft(root);
// Right Left Case
    if (balance < -1 && getBalanceFactor(root->Right) > 0) {
        root->Right = singleRotateRight(root->Right);
        return singleRotateLeft(root);
    }
    return root;
}

int getBalanceFactor(AVL_T N) {
    if (N == NULL)
        return 0;
    return getHeight(N->Left) - getHeight(N->Right);
}

AVL_T minValueNode(AVL_T node) {
    AVL_T current = node;
    while (current->Left != NULL)
        current = current->Left;
    return current;
}

AVL_T deleteCourseStartingWithChar(AVL_T root, char letter) {
    if(root==NULL){
        return NULL;
    }
    while (root != NULL && root->CourseCode[0]==letter){ //// check the root, and keep checking it until it is not needed to be deleted
        root = delete(root, root -> CourseCode);
    }
    if(root != NULL) {
        root -> Left = deleteCourseStartingWithChar(root -> Left, letter);
        root -> Right = deleteCourseStartingWithChar(root -> Right, letter);  //use left  and right subtree after deleting from it
    }
    return root;
}

void printInOrder(AVL_T root){


    if ( root == NULL)
        return ;

    printInOrder(root->Left);

    printf("%s %d %s %s \n", root->Course, root->CreditHours, root->CourseCode, root->department);
    printf("its topics :    ");
    for (int i = 0; i < root->topicNumber; ++i) {
        printf("%s", root->topics[i]);
    }
    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("\n");

    printInOrder(root->Right);
}
AVL_T deleteByDepartment(AVL_T root, char department[100]) {
    if (root == NULL) {
        return NULL;
    }
    while(root != NULL && strcmp(root -> department, department) == 0) { // check the root, and keep checking it until it is not needed to be deleted
        root = delete(root, root -> CourseCode);
    }

    if(root != NULL) {
        root -> Left = deleteByDepartment(root -> Left, department);     //use left  and right subtree after deleting from it
        root -> Right = deleteByDepartment(root -> Right, department);
    }
    return root;
}