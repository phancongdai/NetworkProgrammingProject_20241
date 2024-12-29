#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 512
char answer[1024];
// Function to interact with Ollama and get a response for the user input
char* call_ollama(const char *prompt) {
    char command[MAX_INPUT_LENGTH + 100]; // Space for command and the prompt
    snprintf(command, sizeof(command), "ollama run llama3.1:8b \"%s\"", prompt);

    // Run the command and capture the output
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run Ollama command");
        exit(1);
    }

    char response[MAX_INPUT_LENGTH];
    strcpy(answer, "Assistant: ");
    // printf("Assistant: ");
    while (fgets(response, sizeof(response), fp) != NULL) {
        // printf("%s", response);
        strcat(answer, response);
    }
    fclose(fp);
    return answer;
}

int main() {
    char user_input[MAX_INPUT_LENGTH];
    // char response[1024];
    printf("Assistant is ready! Type 'exit' to quit.\n");

    while (1) {
        printf("You: ");
        fgets(user_input, sizeof(user_input), stdin);

        // Remove the trailing newline character
        user_input[strcspn(user_input, "\n")] = '\0';

        if (strcmp(user_input, "exit") == 0) {
            break;
        }
        // strcpy(response, call_ollama(user_input));
        // Call Ollama with the user input as the prompt
        printf("%s\n", call_ollama(user_input));
    }

    return 0;
}