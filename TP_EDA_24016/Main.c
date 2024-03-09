#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

    // Conta a primeira coluna caso o arquivo comece imediatamente com números
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
            firstChar = true; // Reseta para verdadeiro para contar colunas corretamente após uma quebra de linha
        }
    }

    // Ajuste para considerar a última linha se o arquivo não terminar com '\n'
    if (!firstChar) 
    {
        rows++;
    }

    rewind(file); // Retorna ao início do arquivo para carregar os dados

    // Ajusta o tamanho da matriz para refletir o conteúdo do arquivo
    adjustMatrixSize(matrix, rows, cols);

    // Carrega os dados
    Node* rowPtr = matrix->head;

    for (int i = 0; i < rows; i++)
    {
        Node* colPtr = rowPtr;
    
        for (int j = 0; j < cols; j++)
        {
            if (fscanf(file, "%d;", &value) == 1)// Assegura que lê um valor antes de atribuir 
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
		printf("Invalid row index.\n");
		return;
	}

	Node* rowPtr = matrix->head;
	Node* previousRow = NULL;

    for (int i = 0; i < rowIndex; i++) 
    {
		previousRow = rowPtr;
		rowPtr = rowPtr->down;
	}

    if (rowIndex == 0) 
    {
		matrix->head = rowPtr->down;
	}
    else 
    {
		previousRow->down = rowPtr->down;
	}

	free(rowPtr);
	matrix->rows--;
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

    Node* currentRow = matrix->head;
    Node* aboveNewNode = NULL; // This will track the new node above the current one being inserted

    for (int i = 0; i < matrix->rows; i++) 
    {
        Node* newNode = createNode(0); // Create the new node for this row

        if (columnIndex == 0) 
        {
            // Inserting at the beginning of the row
            newNode->right = currentRow;
            if (i == 0) matrix->head = newNode; // Update the head if it's the first row
            else aboveNewNode->down = newNode; // Link the new node below the previous new node
        }
        else 
        {
            // Inserting at or after columnIndex
            Node* prevNode = currentRow;
            for (int j = 0; j < columnIndex - 1 && prevNode->right != NULL; j++) 
            {
                prevNode = prevNode->right; // Navigate to the node before insertion point
            }

            // Insert the new node into the column
            newNode->right = prevNode->right;
            prevNode->right = newNode;

            if (aboveNewNode != NULL) aboveNewNode->down = newNode; // Link from above if not the first row
        }

        aboveNewNode = newNode; // Update the aboveNewNode for the next iteration
        if (currentRow != NULL) currentRow = currentRow->down; // Move to the next row
    }

    for (Node* temp = matrix->head; columnIndex > 0 && temp != NULL; columnIndex--, temp = temp->right) 
    {
        // At the end of each row iteration, we should have reached the right side of the new column.
        // Now, we iterate down to link the `down` pointers of the new column.
        Node* columnNode = temp;
        while (columnNode != NULL && columnNode->right != NULL && columnNode->right->down != NULL)
        {
            columnNode->right->down = columnNode->right->down->right; // Correctly link down pointers
            columnNode = columnNode->down;
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
    if (matrix == NULL || columnIndex < 0 || columnIndex >= matrix->cols) 
    {
        printf("Invalid column index or null matrix.\n");
        return;
    }

    Node* currentRow = matrix->head;
    Node* toBeFreed = NULL;

    while (currentRow != NULL) 
    {
        if (columnIndex == 0) 
        {
            // Remove the first node in the row
            toBeFreed = currentRow;
            currentRow = currentRow->right; // Move to the next node
            free(toBeFreed); // Free the node

            if (matrix->head == toBeFreed)
            {
                // If the head was removed, update the head pointer
                matrix->head = currentRow;
            }
        }
        else
        {
            Node* prevNode = currentRow;
            
            for (int i = 0; i < columnIndex - 1; i++)
            {
                prevNode = prevNode->right;
            }
            
            toBeFreed = prevNode->right;
            prevNode->right = toBeFreed ? toBeFreed->right : NULL;
            free(toBeFreed); // Free the node
        }

        // Move to the next row
        Node* nextRow = currentRow ? currentRow->down : NULL;
        
        if (columnIndex == 0 && nextRow != NULL) 
        {
            // If the first node was removed, link the `down` pointer of the previous row to the next row
            currentRow = nextRow;
        }
        else 
        {
            // If not, move to the next node in the same row
            currentRow = currentRow ? currentRow->down : NULL;
        }
    }
    matrix->cols--; // Decrement the column count
}
#pragma endregion


// Sum the max value of the matrix but only can use one value for each row and column e mostra os valores que foram usados
int maxSum(MatrixLinkedList* matrix) {
    int sum = 0;
	int* usedRows = (int*)malloc(matrix->rows * sizeof(int));
	int* usedCols = (int*)malloc(matrix->cols * sizeof(int));

	for (int i = 0; i < matrix->rows; i++) {
		usedRows[i] = 0;
	}

	for (int i = 0; i < matrix->cols; i++) {
		usedCols[i] = 0;
	}

	Node* rowPtr = matrix->head;
	Node* colPtr = NULL;

	for (int i = 0; i < matrix->rows; i++) {
		colPtr = rowPtr;
		int max = 0;
		int maxIndex = 0;

		for (int j = 0; j < matrix->cols; j++) {
			if (colPtr->value > max && usedCols[j] == 0 && usedRows[i] == 0) {
				max = colPtr->value;
				maxIndex = j;
			}
			colPtr = colPtr->right;
		}

		usedCols[maxIndex] = 1;
		usedRows[i] = 1;
		sum += max;
		printf("\nRow: %d, Column: %d, Value: %d", i, maxIndex, max);
		rowPtr = rowPtr->down;
	}

	free(usedRows);
	free(usedCols);
	return sum;
}


#pragma region Main
int main() {

    MatrixLinkedList* matrix = createMatrix(0, 0);

    loadMatrixFromFile(matrix, "Matrix.txt");
    printf("Matrix:\n");
    printMatrix(matrix);

    // Change the value of the node at row 2, column 2 to 100; (2;2); (1;4); (2;4); (3;0); (3;2); (3;4); (4;0); (4;2); (4;3); (4;4)
    changeValue(matrix, 4, 4, 100000); // A column and row index of 2 corresponds to the third column and row
    printf("\nMatrix Changed:\n");
    printMatrix(matrix);

    // Add a new row and a new
    /*insertRow(matrix, 4);
    insertColumn(matrix, 3);
    printf("\n\n");
    printMatrix(matrix);*/

    // Remove one row and one column
    /*removeRow(matrix, 2);
    removeColumn(matrix, 2);
    printf("\nMatrix:\n");
    printMatrix(matrix);*/

    // Max sum of the matrix
    printf("\n\nMax sum of the matrix: %d\n", maxSum(matrix));

    return 0;
}
#pragma endregion