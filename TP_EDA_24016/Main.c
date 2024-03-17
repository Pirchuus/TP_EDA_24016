#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


#pragma region Structures
/// <summary>
/// Structure to represent a node in the matrix.
/// </summary>
typedef struct Node 
{
    int value;
    struct Node* right; // Pointer to the next node in the same row
    struct Node* down;  // Pointer to the next node in the same column
} Node;

/// <summary>
/// Structure to represent the matrix as a linked list.
/// </summary>
typedef struct 
{
    Node* head; // Pointer to the first node
    int rows;   // Number of rows
    int cols;   // Number of columns
} MatrixLinkedList;
#pragma endregion


#pragma region Node
/// <summary>
/// Function to create a new node.
/// </summary>
/// <param name="value"></param>
/// <returns></returns>
Node* createNode(int value) 
{
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) 
    {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    newNode->value = value;
    newNode->right = NULL;
    newNode->down = NULL;

    return newNode;
}
#pragma endregion


#pragma region Matrix
/// <summary>
/// Function to create a new matrix.
/// </summary>
/// <param name="rows"></param>
/// <param name="cols"></param>
/// <returns></returns>
MatrixLinkedList* createMatrix(int rows, int cols) 
{
    MatrixLinkedList* matrix = (MatrixLinkedList*)malloc(sizeof(MatrixLinkedList));
    matrix->rows = rows;
    matrix->cols = cols;
    Node* previousRow = NULL;
    Node* currentRow = NULL;
    Node* tempNode = NULL;

    for (int i = 0; i < rows; i++) 
    {    
        currentRow = createNode(0); // Create the first node of the row
        
        if (i == 0) 
        {
            matrix->head = currentRow; // First node of first row is the head
        }
        else 
        {
            previousRow->down = currentRow; // Link the previous row to the current row
        }
        
        tempNode = currentRow;

        for (int j = 1; j < cols; j++) 
        {
            tempNode->right = createNode(0); // Create and link the rest of the nodes in the row
            tempNode = tempNode->right;
        }
        previousRow = currentRow; // Move to the next row
    }
    return matrix;
}

/// <summary>
/// Function to adjust the size of the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="newRows"></param>
/// <param name="newCols"></param>
void adjustMatrixSize(MatrixLinkedList* matrix, int newRows, int newCols) 
{

    if (newRows < 0 || newCols < 0) 
    {	
        printf("Invalid matrix dimensions.\n");
		return;
	}

    // Create a new matrix with specified dimensions
    Node* previousRow = NULL;
    Node* currentRow = NULL;

    for (int i = 0; i < newRows; i++) 
    {    
        Node* tempNode = NULL;
        
        for (int j = 0; j < newCols; j++) 
        {
            if (j == 0) 
            {
                currentRow = createNode(0); // Create the first node of the row
                if (i == 0) 
                {
                    matrix->head = currentRow; // First node of first row is the head
                }
                else 
                {            
                    previousRow->down = currentRow;
                }
               
                tempNode = currentRow;
            }
            else 
            {
                tempNode->right = createNode(0);
                tempNode = tempNode->right;
            }
        }
        previousRow = currentRow;
    }
    matrix->rows = newRows;
    matrix->cols = newCols;
}

/// <summary>
/// Function to load a matrix from a file.
/// </summary>
/// <param name="matrix"></param>
/// <param name="filename"></param>
void loadMatrixFromFile(MatrixLinkedList* matrix, const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    int rows = 0, cols = 0, value, ch;
    bool firstChar = true;

    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) 
    {
        if (firstChar) 
        {
            cols = 1;
            firstChar = false;
        }

        if (ch == ';') 
        {
            cols++;
        }

        if (ch == '\n')
        {
            rows++;
            firstChar = true; // Reset the flag for the next line
        }
    }

    // Increment the row count if the last line does not end with a newline character
    if (!firstChar) 
    {
        rows++;
    }

    rewind(file); // Return to the beginning of the file

    // Adjust the size of the matrix
    adjustMatrixSize(matrix, rows, cols);

    // Load the values from the file into the matrix
    Node* rowPtr = matrix->head;

    for (int i = 0; i < rows; i++)
    {
        Node* colPtr = rowPtr;
    
        for (int j = 0; j < cols; j++)
        {
            if (fscanf(file, "%d;", &value) == 1) // Read the value from the file
            {
                colPtr->value = value;
                colPtr = colPtr->right;
            }
        }
        rowPtr = rowPtr->down;
    }
    fclose(file);
}

/// <summary>
/// Function to get the length of a number.
/// </summary>
/// <param name="number"></param>
/// <returns></returns>
int getNumberLength(int number) 
{    
    int length = 0;

    if (number == 0) return 1;
    
    if (number < 0) length = 1; // Add one for the negative sign
    
    while (number != 0) 
    {    
        length++;
        number /= 10;
    }
    return length;
}

/// <summary>
/// Function to get the maximum length of the numbers in the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
int getMaxNumberLength(MatrixLinkedList* matrix) 
{
    
    int maxLength = 0;
    Node* rowPtr = matrix->head;
    
    while (rowPtr != NULL) 
    {    
        Node* colPtr = rowPtr;
        
        while (colPtr != NULL) 
        {    
            int currentLength = getNumberLength(colPtr->value);
            
            if (currentLength > maxLength) 
            {
                maxLength = currentLength;
            }
            colPtr = colPtr->right;
        }
        rowPtr = rowPtr->down;
    }
    return maxLength;
}

/// <summary>
/// Function to print the matrix.
/// </summary>
/// <param name="matrix"></param>
void printMatrix(MatrixLinkedList* matrix) 
{
    if (matrix == NULL || matrix->head == NULL) 
    {
        printf("The matrix is empty or not initialized.\n");
        return;
    }

    int maxLength = getMaxNumberLength(matrix) + 1; // +1 for space between numbers
    Node* rowPtr = matrix->head;

    for (int i = 0; i < matrix->rows; i++)
    {
        Node* colPtr = rowPtr;

        for (int j = 0; j < matrix->cols; j++) 
        {
            if(colPtr != NULL) // Verifica se colPtr não é NULL antes de tentar acessar o valor
            {
                printf("%*d ", maxLength, colPtr->value); // Use a field width specifier
                colPtr = colPtr->right;
            }
            else
            {
                printf("%*s ", maxLength, " "); // Imprime espaço se colPtr for NULL
            }
        }
        printf("\n");
        rowPtr = rowPtr->down;
    }
}
#pragma endregion


#pragma region Value
/// <summary>
/// Function to change the value of a node in the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="row"></param>
/// <param name="col"></param>
/// <param name="value"></param>
void changeValue(MatrixLinkedList* matrix, int row, int col, int value) 
{
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) 
    {
        printf("Invalid row or column index.\n");
        return;
    }

    Node* rowPtr = matrix->head;
    for (int i = 0; i < row; i++) 
    {
        rowPtr = rowPtr->down;
    }

    Node* colPtr = rowPtr;
    for (int j = 0; j < col; j++) 
    {
        colPtr = colPtr->right;
    }

    colPtr->value = value;
}

/// <summary>
/// Function to get the max sum of the matrix using a backtracking approach
/// </summary>
/// <param name="row"></param>
/// <param name="usedCols"></param>
/// <param name="numRows"></param>
/// <param name="numCols"></param>
/// <param name="rowIndex"></param>
/// <param name="currentSum"></param>
/// <param name="bestSum"></param>
/// <returns></returns>
int maxSumBT(Node* row, int usedCols, int numRows, int numCols, int rowIndex, int currentSum, int* bestSum) 
{
    if (rowIndex == numRows) 
    {
        if (currentSum > *bestSum) 
        {
            // Update the best sum
            *bestSum = currentSum;
        }
        return 0;
    }

    int colIndex = 0;
    
    // Iterate through the nodes in the current row
    for (Node* colNode = row; colNode != NULL && colIndex < numCols; colNode = colNode->right, ++colIndex) 
    {
        // If the current column has not been used
        if (!(usedCols & (1 << colIndex)))
        {
            // Recursively call the function for the next row
            maxSumBT(row->down, usedCols | (1 << colIndex), numRows, numCols, rowIndex + 1, currentSum + colNode->value, bestSum);
        }
    }
    return *bestSum;
}

/// <summary>
/// Function to get the max sum of the matrix but only can use one value for each row and column
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
int maxSum(MatrixLinkedList* matrix) 
{
    int bestSum = INT_MIN;
    maxSumBT(matrix->head, 0, matrix->rows, matrix->cols, 0, 0, &bestSum);
    return bestSum;
}
#pragma endregion


#pragma region Row
/// <summary>
/// Function to add a new row to the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="rowIndex"></param>
void insertRow(MatrixLinkedList* matrix, int rowIndex) 
{
    if (rowIndex < 0 || rowIndex > matrix->rows) 
    {
		printf("Invalid row index.\n");
		return;
	}

	Node* rowPtr = matrix->head;
	Node* previousRow = NULL;
	Node* currentRow = NULL;

    for (int i = 0; i < rowIndex; i++) 
    {
		previousRow = rowPtr;
		rowPtr = rowPtr->down;
	}

    if (rowIndex == 0) 
    {
		currentRow = createNode(0); // Create the first node of the row
		currentRow->down = rowPtr;
		matrix->head = currentRow;
	}
    else 
    {
		currentRow = createNode(0); // Create the first node of the row
		currentRow->down = rowPtr;
		previousRow->down = currentRow;
	}

	Node* temp = currentRow;
	Node* nextRow = NULL;

    for (int i = 0; i < matrix->cols; i++) 
    {
		nextRow = createNode(0); // Create and link the rest of the nodes in the row
		temp->right = nextRow;
		temp = nextRow;
	}

	matrix->rows++;
}

/// <summary>
/// Function to remove a row from the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="rowIndex"></param>
void removeRow(MatrixLinkedList* matrix, int rowIndex)
{
    if (rowIndex < 0 || rowIndex >= matrix->rows)
    {
        printf("\n\nInvalid row index.\n\n");
        return;
    }

    Node* currentRow = NULL;
    Node* previousRow = NULL;
    Node* temp = NULL;

    if (rowIndex == 0) 
    {
        currentRow = matrix->head; // The row to be removed
        matrix->head = currentRow->down; // Update the head of the matrix
    }
    else 
    {
        previousRow = matrix->head;
        
        for (int i = 0; i < rowIndex - 1; i++)
        {
            previousRow = previousRow->down;
        }
        
        currentRow = previousRow->down; // The row to be removed
        previousRow->down = currentRow->down; // Link previous row to the next row
    }

    // Iterate through the entire row to free each node
    while (currentRow != NULL) 
    {
        temp = currentRow;
        currentRow = currentRow->right; // Move to the next node in the row
        free(temp); // Free the current node
    }

    matrix->rows--; // Decrement the row count
}
#pragma endregion


#pragma region Column
/// <summary>
/// Function to add a new column to the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="columnIndex"></param>
void insertColumn(MatrixLinkedList* matrix, int columnIndex) 
{
    if (columnIndex < 0 || columnIndex > matrix->cols) 
    {
        printf("Invalid column index.\n");
        return;
    }

    // Insertion in the first column
    if (columnIndex == 0) 
    {
        Node* newHeadColumn = NULL;
        Node* lastNewNode = NULL;
        Node* currentRow = matrix->head;

        for (int i = 0; i < matrix->rows; i++) 
        {
            Node* newNode = createNode(0); // Create a new node for the first column

            if (i == 0) 
            {
                newHeadColumn = newNode; // The new node is the head of the new column
            }
            else 
            {
                lastNewNode->down = newNode; // Connect the new node to the previous node in the column
            }

            newNode->right = currentRow; // The new node points to the current node in the row
            lastNewNode = newNode; // Update the last new node
            currentRow = currentRow->down; // Move to the next row
        }

        matrix->head = newHeadColumn; // Update the head of the matrix
    }
    else 
    {
        // Insrtion in subsequent columns
        Node* currentRow = matrix->head;

        for (int i = 0; i < matrix->rows; i++) 
        {
            Node* newNode = createNode(0);

            if (columnIndex > 0) 
            {
                Node* prevNode = currentRow;
                for (int j = 0; j < columnIndex - 1; j++) 
                {
                    prevNode = prevNode->right; // Move to the node before the insertion point
                }
                newNode->right = prevNode->right; // New node points to the node after the insertion point
                prevNode->right = newNode; // Node before the insertion point points to the new node
            }

            // Move to the next row
            currentRow = currentRow->down;
        }
    }

    matrix->cols++; // Increment the column count
}

/// <summary>
/// Function to remove a column from the matrix.
/// </summary>
/// <param name="matrix"></param>
/// <param name="columnIndex"></param>
void removeColumn(MatrixLinkedList* matrix, int columnIndex) 
{
    if (columnIndex < 0 || columnIndex >= matrix->cols) 
    {
        printf("Invalid column index.\n");
        return;
    }

    Node* currentRow = matrix->head;
    Node* toBeFreed = NULL;

    // Remove the first column
    if (columnIndex == 0) 
    {
        while (currentRow != NULL) 
        {
            toBeFreed = currentRow; // Node to be removed
            currentRow = currentRow->right; // Update the current row to the next row
            free(toBeFreed); // Free the node
            matrix->head = currentRow; // Update the head of the matrix

            // Move to the next row
            if (currentRow != NULL) 
            {
                currentRow = currentRow->down;
            }
        }
    }
    else 
    {
        // Remove subsequent columns
        while (currentRow != NULL) 
        {
            Node* prevNode = currentRow;
            for (int i = 0; i < columnIndex - 1; i++) 
            {
                prevNode = prevNode->right; // Move to the node before the one to be removed
            }

            toBeFreed = prevNode->right; // Node to be removed
            if (toBeFreed != NULL) 
            {
                prevNode->right = toBeFreed->right; // Link the previous node to the next node
                free(toBeFreed); // Free the node
            }

            currentRow = currentRow->down; // Move to the next row
        }
    }

    matrix->cols--; // Decrement the column count
}
#pragma endregion


#pragma region Main
int main() {

    MatrixLinkedList* matrix = createMatrix(0, 0);

    loadMatrixFromFile(matrix, "Matrix.txt");
    printf("Matrix:\n");
    printMatrix(matrix);

    //// Change the value of the node at row 1, column 1 to 1000; 
    //changeValue(matrix, 0, 0, 1000); // A column and row index of 0 corresponds to the first column and row
    //printf("\nMatrix Changed:\n");
    //printMatrix(matrix);

    //// Add a new row and a new column
    //insertRow(matrix, 0);
    //insertColumn(matrix, 0);
    //printf("\n\n");
    //printMatrix(matrix);

    //// Remove one row and one column
    //removeRow(matrix, 0);
    //removeColumn(matrix, 0); // -> Não é consigo remover a primeira coluna
    //printf("\nMatrix:\n");
    //printMatrix(matrix);

    // Max sum of the matrix
    printf("\n\nMax sum of the matrix: %d\n", maxSum(matrix));

    return 0;
}
#pragma endregion