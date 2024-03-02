#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* right; // Pointer to the next node in the same row
    struct Node* down;  // Pointer to the next node in the same column
} Node;

typedef struct {
    Node* head; // Pointer to the first node
    int rows;   // Number of rows
    int cols;   // Number of columns
} MatrixLinkedList;


// Function to create a new node.
Node* createNode(int value) {

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    newNode->value = value;
    newNode->right = NULL;
    newNode->down = NULL;

    return newNode;
}

#pragma region Matrix

// Function to create a new matrix.
MatrixLinkedList* createMatrix(int rows, int cols) {

    MatrixLinkedList* matrix = (MatrixLinkedList*)malloc(sizeof(MatrixLinkedList));
    matrix->rows = rows;
    matrix->cols = cols;

    Node* previousRow = NULL;
    Node* currentRow = NULL;
    Node* tempNode = NULL;

    for (int i = 0; i < rows; i++) {
        currentRow = createNode(0); // Create the first node of the row
        if (i == 0) {
            matrix->head = currentRow; // First node of first row is the head
        }
        else {
            previousRow->down = currentRow; // Link the previous row to the current row
        }
        tempNode = currentRow;

        for (int j = 1; j < cols; j++) {
            tempNode->right = createNode(0); // Create and link the rest of the nodes in the row
            tempNode = tempNode->right;
        }

        previousRow = currentRow; // Move to the next row
    }

    return matrix;
}

// Function to load a matrix from a file.
void loadMatrixFromFile(MatrixLinkedList* matrix, const char* filename) {

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("File not found\n");
        return;
    }

    int num;
    char separator;
    Node* temp = matrix->head;
    Node* rowStart = temp;

    while (fscanf(file, "%d%c", &num, &separator) != EOF) {
        temp->value = num;
        if (separator == ';') {
            if (temp->right != NULL) temp = temp->right;
        }
        else { // New line
            if (rowStart->down != NULL) {
                rowStart = rowStart->down;
                temp = rowStart;
            }
        }
    }

    fclose(file);
}

// Function to print the matrix.
void printMatrix(MatrixLinkedList* matrix) {

    if (matrix == NULL || matrix->head == NULL) {

        printf("The matrix is empty or not initialized.\n");
        return;
    }

    Node* rowPtr = matrix->head; // Start with the first row

    for (int i = 0; i < matrix->rows; i++) { // Iterate over rows

        Node* colPtr = rowPtr; // Start with the first column in the current row
        for (int j = 0; j < matrix->cols; j++) { // Iterate over columns

            printf("%d\t", colPtr->value); // Print the value with a tab for spacing
            colPtr = colPtr->right; // Move to the next column
        }

        printf("\n"); // New line after finishing the columns in the current row
        rowPtr = rowPtr->down; // Move to the next row
    }
}

#pragma endregion

#pragma region Value
// Function to change the value of a specific node in the matrix.
void changeValue(MatrixLinkedList* matrix, int row, int col, int value) {

    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) {
        printf("Invalid row or column index.\n");
        return;
    }

    Node* rowPtr = matrix->head;
    for (int i = 0; i < row; i++) {
        rowPtr = rowPtr->down;
    }

    Node* colPtr = rowPtr;
    for (int j = 0; j < col; j++) {
        colPtr = colPtr->right;
    }

    colPtr->value = value;
}
#pragma endregion

#pragma region Row
// Function to add new rows to the matrix.
void insertRow(MatrixLinkedList* matrix, int rowIndex) {

    if (rowIndex < 0 || rowIndex > matrix->rows) {
		printf("Invalid row index.\n");
		return;
	}

	Node* rowPtr = matrix->head;
	Node* previousRow = NULL;
	Node* currentRow = NULL;

    for (int i = 0; i < rowIndex; i++) {
		previousRow = rowPtr;
		rowPtr = rowPtr->down;
	}

    if (rowIndex == 0) {
		currentRow = createNode(0); // Create the first node of the row
		currentRow->down = rowPtr;
		matrix->head = currentRow;
	}
    else {
		currentRow = createNode(0); // Create the first node of the row
		currentRow->down = rowPtr;
		previousRow->down = currentRow;
	}

	Node* temp = currentRow;
	Node* nextRow = NULL;

    for (int i = 0; i < matrix->cols; i++) {
		nextRow = createNode(0); // Create and link the rest of the nodes in the row
		temp->right = nextRow;
		temp = nextRow;
	}

	matrix->rows++;
}

// Funtion to remove a row from the matrix.
void removeRow(MatrixLinkedList* matrix, int rowIndex) {

    if (rowIndex < 0 || rowIndex >= matrix->rows) {
		printf("Invalid row index.\n");
		return;
	}

	Node* rowPtr = matrix->head;
	Node* previousRow = NULL;

    for (int i = 0; i < rowIndex; i++) {
		previousRow = rowPtr;
		rowPtr = rowPtr->down;
	}

    if (rowIndex == 0) {
		matrix->head = rowPtr->down;
	}
    else {
		previousRow->down = rowPtr->down;
	}

	free(rowPtr);
	matrix->rows--;
}
#pragma endregion

#pragma region Column

//// Function to add new columns to the matrix.
//void insertColumn(MatrixLinkedList* matrix, int colIndex) {
//    if (colIndex < 0 || colIndex > matrix->cols) {
//        printf("Invalid column index.\n");
//        return;
//    }
//
//    Node* colPtr = matrix->head;
//    Node* previousCol = NULL;
//    Node* currentCol = NULL;
//
//    for (int i = 0; i < colIndex; i++) {
//        if (colPtr == NULL) {
//            printf("Invalid column index.\n");
//            return;
//        }
//        previousCol = colPtr;
//        colPtr = colPtr->right;
//    }
//
//    // Create the new column node
//    currentCol = createNode(0);
//
//    // Insert the new column into the matrix
//    if (colIndex == 0) {
//        currentCol->right = matrix->head;
//        matrix->head = currentCol;
//    }
//    else {
//        currentCol->right = colPtr;
//        previousCol->right = currentCol;
//    }
//
//    // Link the new column nodes to the existing rows
//    Node* temp = currentCol;
//    Node* rowPtr = matrix->head;
//    while (rowPtr != NULL && rowPtr->down != NULL) {
//        Node* newRowNode = createNode(0);
//        temp->down = newRowNode;
//        temp = temp->down;
//        rowPtr = rowPtr->down;
//    }
//
//    matrix->cols++;
//}
//
//// Function to remove a column from the matrix.
//void removeColumn(MatrixLinkedList* matrix, int colIndex) {
//    if (colIndex < 0 || colIndex >= matrix->cols) {
//		printf("Invalid column index.\n");
//		return;
//	}
//
//	Node* colPtr = matrix->head;
//	Node* previousCol = NULL;
//
//    for (int i = 0; i < colIndex; i++) {
//        if (colPtr == NULL) {
//			printf("Invalid column index.\n");
//			return;
//		}
//		previousCol = colPtr;
//		colPtr = colPtr->right;
//	}
//
//    if (colIndex == 0) {
//		matrix->head = colPtr->right;
//	}
//    else {
//		previousCol->right = colPtr->right;
//	}
//
//	free(colPtr);
//	matrix->cols--;
//}

#pragma endregion // error


int main() {

    MatrixLinkedList* matrix = createMatrix(5,5);

    loadMatrixFromFile(matrix, "Matrix.txt");
    printf("Matrix:\n");
    printMatrix(matrix);

    // Change the value of the node at row 2, column 2 to 100
    changeValue(matrix, 2, 2, 100); // A column and row index of 2 corresponds to the third column and row

    printf("\nMatrix Changed:\n");
    printMatrix(matrix);

    // Add a new row at index 2 and a new column at index 3
    insertRow(matrix, 2);
    removeRow(matrix, 1);
    //insertColumn(matrix, 2);

    printf("\nMatrix with new row and new column:\n");
    printMatrix(matrix);


    return 0;
}