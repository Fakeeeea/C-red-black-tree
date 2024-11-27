#include <stdio.h>
#include <stdlib.h>

#define BLACK 0
#define RED 1

#define LEFT 0
#define RIGHT 1

typedef struct Node{
    int value;
    struct Node *left, *right, *parent;
    int color;
}Node;

Node* createNode(Node* parent, int valueToSet);
int addNode(Node** root, int valueToAdd);
int deleteNode(Node** root, int valueToDelete);
Node* inOrderSuccessor(Node* startingPoint);
int rotateLeft(Node* toRotate, Node** root);
int rotateRight(Node* toRotate, Node** root);
void fixInsertion(Node** root, Node* tofix);
void fixDeletion(Node** root, Node* tofix);
void Tree_inOrder(Node *n);
void deleteSpecificNode(Node**root, Node* toDelete);
void print_info(Node *n);

int main()
{
    Node *root = NULL;
    int value;
    char c;

    do
    {
        printf("\ni to insert new value\n");
        printf("d to delete a value \n");
        printf("p to print the tree \n");
        printf("q to quit\n");
        scanf("\n%c",&c);

        switch(c)
        {
            case 'i':
                printf("Write value to insert: ");
                scanf("%d",&value);
                addNode(&root, value);
                break;
            case 'd':
                printf("Write value to delete: ");
                scanf("%d",&value);
                deleteNode(&root, value);
                break;
            case 'p':
                printf("Printing tree in order: \n");
                Tree_inOrder(root);
                break;
            case 'q':
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }while(c != 'q');

    return 0;
}

int addNode(Node** root, int valueToAdd)
{
    Node *tmp = *root;
    Node *parent = NULL;

    while(tmp != NULL)
    {
        parent = tmp;

        if(tmp->value < valueToAdd)
        {
            tmp = tmp->right;
        }
        else if(tmp->value > valueToAdd)
        {
            tmp = tmp->left;
        }
        else
        {
            return -1; //value already exists
        }
    }

    if(parent != NULL)
    {
        if(parent->value < valueToAdd)
        {
            parent->right = createNode(parent, valueToAdd);
            tmp = parent->right;
        }
        else
        {
            parent->left = createNode(parent, valueToAdd);
            tmp = parent->left;
        }
    }
    else
    {
        *root = createNode(NULL, valueToAdd);
        tmp = *root;
    }

    fixInsertion(root, tmp);

    return 1;
}

void fixInsertion(Node** root, Node* tofix)
{
    int swap;
    Node * uncle;

    if(tofix->parent == NULL)
    {
        (*root)->color = BLACK;
        return;
    }

    if(tofix->parent->color == BLACK || tofix->parent->parent == NULL)
    {
        return;
    }

    if(tofix->parent->parent->left != tofix->parent)
    {
        uncle = tofix->parent->parent->left;
    }
    else
    {
        uncle = tofix->parent->parent->right;
    }

    if(uncle != NULL && uncle->color == RED)
    {
        uncle->color = BLACK;
        tofix->parent->color = BLACK;
        tofix->parent->parent->color = RED;
        tofix = tofix->parent->parent;
    }
    else
    {
        if((tofix->parent && tofix->parent->parent) && (tofix->parent == tofix->parent->parent->right && tofix == tofix->parent->right))
        {
            swap = tofix->parent->parent->color;
            tofix->parent->parent->color = tofix->parent->color;
            tofix->parent->color = swap;
            rotateLeft(tofix->parent->parent,root);
        }

        if((tofix->parent && tofix->parent->parent) && (tofix->parent == tofix->parent->parent->left && tofix == tofix->parent->right))
        {
            swap = tofix->parent->parent->color;
            tofix->parent->parent->color = tofix->parent->color;
            tofix->parent->color = swap;
            rotateLeft(tofix->parent,root);
            if(tofix->parent->parent)
                rotateRight(tofix->parent->parent,root);
            else
                rotateRight(tofix->parent,root);
        }

        if ((tofix->parent && tofix->parent->parent) && (tofix->parent == tofix->parent->parent->left && tofix == tofix->parent->left))
        {
            swap = tofix->parent->parent->color;
            tofix->parent->parent->color = tofix->parent->color;
            tofix->parent->color = swap;
            rotateRight(tofix->parent->parent,root);
        }

        if ((tofix->parent && tofix->parent->parent) && (tofix->parent == tofix->parent->parent->right && tofix == tofix->parent->left))
        {
            swap = tofix->parent->parent->color;
            tofix->parent->parent->color = tofix->parent->color;
            tofix->parent->color = swap;
            rotateRight(tofix->parent,root);
            if(tofix->parent->parent)
                rotateLeft(tofix->parent->parent,root);
            else
                rotateLeft(tofix->parent,root);
        }
    }

    fixInsertion(root, tofix);
}

Node* createNode(Node* parent, int valueToSet)
{
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;
    newNode->value = valueToSet;
    newNode->color = RED;

    return newNode;
}

int deleteNode(Node** root, int valueToDelete)
{
    Node* toDelete = *root;

    while(toDelete != NULL)
    {
        if(toDelete->value < valueToDelete)
        {
            toDelete = toDelete->right;
        }
        else if(toDelete->value > valueToDelete)
        {
            toDelete = toDelete->left;
        }
        else
        {
            break;
        }
    }

    if(toDelete == NULL)
        return -1;

    deleteSpecificNode(root, toDelete);
    return 1;
}

//I am sorry to anyone who will have to go through this section of code
void deleteSpecificNode(Node**root, Node* toDelete)
{
    Node *inOrder;

    //simple case: both children
    if(toDelete->left != NULL && toDelete->right != NULL)
    {
        inOrder = inOrderSuccessor(toDelete->right);
        toDelete->value = inOrder->value;
        deleteSpecificNode(root, inOrder);
        return;
    }

    //simple case: no children
    if(toDelete->left == NULL && toDelete->right == NULL)
    {
        if(toDelete->parent == NULL)
        {
            free(toDelete);
            *root = NULL;
            return;
        }

        if(toDelete->color == RED)
        {
            if(toDelete->parent->left == toDelete)
            {
                toDelete->parent->left = NULL;
            }
            else
            {
                toDelete->parent->right = NULL;
            }
            free(toDelete);
            return;
        }
        else
        {
            if(toDelete->parent->left == toDelete)
            {
                toDelete->parent->left = NULL;
            }
            else
            {
                toDelete->parent->right = NULL;
            }
            fixDeletion(root, toDelete);
            free(toDelete);
            return;
        }
    }

    //simple case: 1 child
    if(toDelete->right == NULL)
    {
        toDelete->value = toDelete->left->value;
        toDelete->color = BLACK;
        free(toDelete->left);
        toDelete->left = NULL;
    }
    else
    {
        toDelete->value = toDelete->right->value;
        toDelete->color = BLACK;
        free(toDelete->right);
        toDelete->right = NULL;
    }
}

//and also this section
void fixDeletion(Node** root, Node* tofix)
{
    int location;
    Node * parent = tofix->parent;
    Node * sibling = NULL;
    Node * near = NULL;
    Node * far = NULL;

    if(parent->left == tofix)
    {
        location = LEFT;
        sibling = parent->right;
    }
    else
    {
        location = RIGHT;
        sibling = parent->left;
    }

    if(sibling != NULL)
    {
        if(location == LEFT)
        {
            near = sibling->right;
            far = sibling->left;
        }
        else
        {
            near = sibling->left;
            far = sibling->right;
        }
    }

    if(sibling == NULL || sibling->color == BLACK)
    {
        //sibling is black, children are black
        if(sibling != NULL && ((sibling->left == NULL || sibling->left->color == BLACK) && (sibling->right == NULL || sibling->right->color == BLACK)))
        {
            //case 2: parent black, sibling black, children black
            //else case 4: parent red, sibling black, children black
            if(parent->color == BLACK)
            {
                sibling->color = RED;
                fixDeletion(root, parent);
                return;
            }
            else
            {
                sibling->color = RED;
                parent->color = BLACK;
                return;
            }
        }
        else
        {
            //case 5: sibling is black and so is the far child. Close child is red
            if((near != NULL && near->color == RED) && (far == NULL || far->color == BLACK))
            {
                if(location == RIGHT)
                {
                    rotateLeft(sibling, root);
                }
                else
                {
                    rotateRight(sibling,root);
                }

                sibling->color = RED;
                near->color = BLACK;
                fixDeletion(root, tofix);
                return;
            }

            //case 6: sibling is black and so is the close child. Far child is red
            if((far != NULL && far->color == RED) && (near == NULL || near->color == BLACK))
            {
                if(location == RIGHT)
                {
                    rotateRight(parent, root);
                }
                else
                {
                    rotateLeft(parent, root);
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                far->color = BLACK;
                return;
            }
        }
    }
    else
    {
        //case 3: sibling red, everything else is black
        if(location == RIGHT)
        {
            rotateRight(parent, root);
        }
        else
        {
            rotateLeft(parent, root);
        }

        parent->color = RED;
        sibling->color = BLACK;

        fixDeletion(root, tofix);
        return;
    }
}



Node* inOrderSuccessor(Node* startingPoint)
{
    Node *current = startingPoint;
    while(current->left != NULL)
        current = current->left;
    return current;
}

int rotateRight(Node* toRotate, Node** root)
{
    Node *tmp = toRotate;
    Node *pivot = NULL;

    if(tmp == NULL || tmp->left == NULL)
        return -1;

    pivot = tmp->left;
    tmp->left = pivot->right;

    if(tmp->left != NULL)
        tmp->left->parent = tmp;

    pivot->right = tmp;
    pivot->parent = tmp->parent;
    tmp->parent = pivot;

    if(pivot->parent != NULL)
    {
        if(pivot->parent->value < pivot->value)
        {
            pivot->parent->right = pivot;
        }
        else
        {
            pivot->parent->left = pivot;
        }
    }
    else
    {
        *root = pivot;
    }
    return 1;
}

int rotateLeft(Node* toRotate, Node** root)
{
    Node *tmp = toRotate;
    Node *pivot = NULL;

    if(tmp == NULL || tmp->right == NULL)
        return -1;

    pivot = tmp->right;
    tmp->right = pivot->left;

    if(tmp->right != NULL)
        tmp->right->parent = tmp;

    pivot->left = tmp;
    pivot->parent = tmp->parent;
    tmp->parent = pivot;

    if(pivot->parent != NULL)
    {
        if(pivot->parent->value < pivot->value)
        {
            pivot->parent->right = pivot;
        }
        else
        {
            pivot->parent->left = pivot;
        }
    }
    else
    {
        *root = pivot;
    }
    return 1;
}

void Tree_inOrder(Node *n)
{
    if(n==NULL)
        return;

    Tree_inOrder(n->left);
    print_info(n);
    Tree_inOrder(n->right);
}

void print_info(Node *n)
{
    printf("Value: %d\n", n->value);
    printf("Parent, left, right : %d - %d - %d\n",(n->parent != NULL ? n->parent->value : 0),(n->left != NULL ? n->left->value : 0),(n->right != NULL ? n->right->value : 0));
    printf("Color: %d\n\n", n->color);
}
