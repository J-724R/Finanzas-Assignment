#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_WEEKS 60
#define MAX_LINE_LENGTH 1024
#define N_COINS 8

typedef struct {
    double close[N_COINS]; // We'll primarily use the closing price
} WeeklyDataPoint;

int load_data_from_csv(char filenames[N_COINS][512], WeeklyDataPoint *data) {
    int current_week = 0;

    for (int i = 0; i < N_COINS; i++) {
        const char *filename = filenames[i];
        printf("Loading File: %s\n", filename);

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
                    data[week].close[i] = atof(token);
                    break; 
                }
                token = strtok(NULL, ";");
                column++;
            }
            week++;
        }
        fclose(file);
    } 
    return 0;
}

int main() {
    const char *coin_names[] = {
        "Aptos", "Bitcoin", "BNB", "Cardano", "Ethereum", "Solana", "Sui", "XRP"
    };

    char filenames[N_COINS][512]; 
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return 1;
    }

    for (int i = 0; i < N_COINS; i++) {
        snprintf(filenames[i], sizeof(filenames[i]), "%s/Historical_Last_13Month/%s_weekly_historical_data_coinmarketcap.csv",
                 cwd, coin_names[i]);
        printf("Filename: %s\n", filenames[i]);
    }

    WeeklyDataPoint weekly_data[MAX_WEEKS];
    double weakly_returns[MAX_WEEKS - 1][N_COINS];
    
    int result = load_data_from_csv(filenames, weekly_data);
    if (result == 0) {
        printf("Data loaded successfully!\n");

        for (int i = 0; i < N_COINS; i++)
        {
            printf("\n\n%s Weekly Close Price\n", coin_names[i]);
            for (int j = 0; j < MAX_WEEKS; j++) {
                if (i < MAX_WEEKS - 1){
                    weakly_returns[i][j] = (weekly_data[i + 1].close[j] - weekly_data[i].close[j]) / weekly_data[i].close[j];
                }

                printf("Week %d = %.4f USD\n", j+1, weekly_data[j].close[i]);
                /* code */
            }
        }
    } else {
        printf("Error loading data.\n");
        return 1;
    }
    return 0;
}