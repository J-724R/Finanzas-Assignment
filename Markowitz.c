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
        // printf("Loading File: %s\n", filename);

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

void calculate_expected_returns(double weakly_returns[MAX_WEEKS - 1][N_COINS], double expected_returns[N_COINS]) {
    for (int i = 0; i < N_COINS; i++) {
        expected_returns[i] = 0.0;
        for (int j = 0; j < MAX_WEEKS - 1; j++) {
            expected_returns[i] += weakly_returns[j][i];
        }
        expected_returns[i] /= (MAX_WEEKS - 1);
    }
}

void calculate_cov_matrix(double weakly_returns[MAX_WEEKS - 1][N_COINS], double expected_returns[N_COINS], double cov_matrix[N_COINS][N_COINS]) {
    for (int i = 0; i < N_COINS; i++) {
        for (int j = 0; j < N_COINS; j++) {
            cov_matrix[i][j] = 0.0;
            for (int k = 0; k < MAX_WEEKS - 1; k++) {
                cov_matrix[i][j] += (weakly_returns[k][i] - expected_returns[i])*(weakly_returns[k][j] - expected_returns[j]); 
            }
            cov_matrix[i][j] /= (MAX_WEEKS - 1);
        }
    }
};


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
    }

    WeeklyDataPoint weekly_data[MAX_WEEKS];
    double weakly_returns[MAX_WEEKS - 1][N_COINS];
    
    int result = load_data_from_csv(filenames, weekly_data);
    if (result == 0) {
        printf("Data loaded successfully!\n");

        for (int i = 0; i < N_COINS; i++)
        {
            // printf("\n\n%s Weekly Close Price\n", coin_names[i]);
            for (int j = 0; j < MAX_WEEKS; j++) {
                if (i < MAX_WEEKS - 1){
                    weakly_returns[j][i] = (weekly_data[j + 1].close[i] - weekly_data[j].close[i]) / weekly_data[j].close[i];
                }
                // printf("Week %d = %.4f USD\n", j+1, weekly_data[j].close[i]);
            }
        }

        //Markowitz
        double expected_returns[N_COINS];
        double cov_matrix[N_COINS][N_COINS];
        calculate_expected_returns(weakly_returns, expected_returns);
        calculate_cov_matrix(weakly_returns, expected_returns, cov_matrix);        

        // Steps remaining
        //Optimization with Genetic Algorithm or Quadratic Programming
        //Efficient Frontier

    } else {
        printf("Error loading data.\n");
        return 1;
    }
    return 0;
}