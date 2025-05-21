#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 25
#define MAX_EMPLOYEES 5

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float salary;
    int order_number;
} Employee;

typedef struct Node {
    Employee data;
    struct Node* next;
} Node;

Node* head = NULL;
int employee_count = 0;

// Function prototypes
void insertAtBeginning(Employee emp);
void insertAtMiddle(Employee emp, int position);
void insertAtEnd(Employee emp);
int isEmpty();
void traverseEmployeesInOrder(Node* current);
void printEmployeesInOrder();
void displayMenu();
int isValidName(const char* str);
void getValidName(char* name);
int getValidInt(const char* prompt);
void saveToFile();

void insertEmployee(int id, const char* name, float salary, int position) {
    Employee newEmployee;
    newEmployee.id = id;
    strncpy(newEmployee.name, name, MAX_NAME_LENGTH - 1);
    newEmployee.name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    newEmployee.salary = salary;
    newEmployee.order_number = ++employee_count;

    if (position <= 1) {
        insertAtBeginning(newEmployee);
    } else if (position >= employee_count) {
        insertAtEnd(newEmployee);
    } else {
        insertAtMiddle(newEmployee, position);
    }
}

void insertAtBeginning(Employee emp) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("\nError: Memory allocation failed.\n");
        return;
    }
    newNode->data = emp;
    newNode->next = head;
    head = newNode;
}

void insertAtMiddle(Employee emp, int position) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("\nError: Memory allocation failed.\n");
        return;
    }
    newNode->data = emp;

    Node* current = head;
    for (int i = 1; current != NULL && i < position - 1; i++) {
        current = current->next;
    }

    if (current != NULL) {
        newNode->next = current->next;
        current->next = newNode;
    } else {
        free(newNode); // If the position is invalid, free the allocated memory
        printf("\nError: Invalid position for insertion.\n");
    }
}

void insertAtEnd(Employee emp) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("\nError: Memory allocation failed.\n");
        return;
    }
    newNode->data = emp;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int isEmpty() {
    return head == NULL;
}

void traverseEmployeesInOrder(Node* current) {
    if (current == NULL) {
        return; // Base case: end of the list
    }
    traverseEmployeesInOrder(current->next); // Recursive call with the next node
    // Now print the current node's data after the recursive call
    printf("\nEmployee #%d's Information:\n", current->data.order_number);
    printf("ID: %d\nName: %s\nSalary: %.2f\n", current->data.id, current->data.name, current->data.salary);
}

void printEmployeesInOrder() {
    if (isEmpty()) {
        printf("\nNo employee information has been entered.\n");
        return;
    }
    // Start the recursive traversal from the head of the list
    traverseEmployeesInOrder(head);
}

void displayMenu() {
    printf("\nEmployee Management System\n");
    printf("1. Create an empty list\n");
    printf("2. Add an employee\n");
    printf("3. Check if the list is empty\n");
    printf("4. Display all employees\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

int isValidName(const char* str) {
    if (*str == '\0') { // Empty string is not a valid name
        return 0;
    }
    while (*str) {
        if (!isalpha((unsigned char)*str) && *str != ' ' && *str != '-') {
            return 0;
        }
        str++;
    }
    return 1;
}

void getValidName(char* name) {
    char buffer[256];
    do {
        printf("Enter employee name: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        if (!isValidName(buffer)) {
            printf("Invalid name. Please enter a valid name (alphabetic characters, spaces, hyphens are allowed).\n");
        }
    } while (!isValidName(buffer));

    strncpy(name, buffer, MAX_NAME_LENGTH);
    name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
}

int getValidInt(const char* prompt) {
    int value;
    char buffer[256];

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            // Input error
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        char* endptr;
        value = (int)strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf("Invalid input. Please enter an integer.\n");
        } else {
            break;
        }
    }
    return value;
}

void saveToFile() {
    FILE* file = fopen("employees_information.txt", "w");
    if (!file) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    Node* current = head;
    while (current) {
        fprintf(file, "Employee #%d's Information\n", current->data.order_number);
        fprintf(file, "ID: %d\nName: %s\nSalary: %.2f\n\n", current->data.id, current->data.name, current->data.salary);
        current = current->next;
    }
    fclose(file);
}

int main() {
    int choice;
    int emp_id;
    char emp_name[MAX_NAME_LENGTH];
    float emp_salary;

    do {
        displayMenu();
        choice = getValidInt("");

        switch (choice) {
            case 1:
                // Assuming we want to reset the list to empty
                while (head != NULL) {
                    Node* temp = head;
                    head = head->next;
                    free(temp);
                }
                employee_count = 0;
                printf("\nAn empty list has been created.\n");
                break;
            case 2:
                emp_id = getValidInt("Enter employee ID: ");
                getValidName(emp_name);
                emp_salary = (float)getValidInt("Enter employee salary: ");
                int position;
                printf("Enter the position to insert the employee (1 for beginning, %d for end, or any position in between): ", employee_count + 1);
                position = getValidInt("");
                insertEmployee(emp_id, emp_name, emp_salary, position);
                break;
            case 3:
                printf("\nThe list is %sempty.\n", isEmpty() ? "" : "not ");
                break;
            case 4:
                printEmployeesInOrder();
                break;
            case 5:
                printf("\nExiting the program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 5);

    saveToFile();

    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}