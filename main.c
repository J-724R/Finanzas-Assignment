#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WEEKS 60
#define MAX_LINE_LENGTH 1024

typedef struct {
    double close; // We'll primarily use the closing price
} WeeklyDataPoint;

int load_data_from_csv(const char *filename, WeeklyDataPoint *data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1; 
    }

    char line[MAX_LINE_LENGTH];    
    fgets(line, MAX_LINE_LENGTH, file); // Skip the header line

    int week = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && week < MAX_WEEKS) {
        char *token;
        int column = 0;
        
        token = strtok(line, ";"); // First csv column
        
        while (token != NULL) {
            if(column == 8) {
                data[week].close = atof(token);
                break; 
            }
            token = strtok(NULL, ";");
            column++;
        }
        week++;
    }

    fclose(file);
    return 0;
}

int main() {
    WeeklyDataPoint weekly_data[MAX_WEEKS];
    if (load_data_from_csv("./Historical_Last_13Month/Bitcoin_weekly_historical_data_coinmarketcap.csv", weekly_data) == 0) {
        printf("Data loaded successfully!\n");
        for (int i = 0; i < MAX_WEEKS; i++) {
            printf("Week %d: %.2f\n",i + 1, weekly_data[i].close);
        }
        // ... (Proceed to the next steps)


    } else {
        printf("Error loading data.\n");
        return 1;
    }

    return 0;
}