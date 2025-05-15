#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[100];
    char transformed[100];
    char buffer[4];
    
    // Initialize the transformed buffer with 'd'
    transformed[0] = 'd';
    // Initialize the rest with 0
    memset(transformed + 1, 0, sizeof(transformed) - 1);
    
    printf("Please enter key: ");
    scanf("%s", &input);
    
    // Check if input starts with "00"
    if (input[0] != '0' || input[1] != '0') {
        printf("Nope.\n");
        return 1;
    }
    
    // Process input in groups of 3 digits
    int input_pos = 2;  // Start after "00"
    int output_pos = 1; // Start after 'd'
    
    while (input[input_pos] && input[input_pos+1] && input[input_pos+2]) {
        // Get 3 characters from input
        buffer[0] = input[input_pos];
        buffer[1] = input[input_pos+1];
        buffer[2] = input[input_pos+2];
        buffer[3] = '\0';
        
        // Convert to integer
        int ascii_value = atoi(buffer);
        
        // Store as a character in transformed buffer
        transformed[output_pos] = (char)ascii_value;
        
        // Move to next positions
        input_pos += 3;
        output_pos++;
    }

    
    // Null-terminate the transformed string
    transformed[output_pos] = '\0';
    
    
    // Compare with expected password
    if (strcmp(transformed, "delabere") == 0) {
        printf("Good job!\n");
        return 0;
    } else {
        printf("Nope.\n");
        return 1;
    }
}