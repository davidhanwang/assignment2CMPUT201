#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 10001

char* LCS(char* string1, char* string2){
  char* lcs_actual = malloc(MAX_LEN * sizeof(char));

  int n = strlen(string1);
  int m = strlen(string2);

  int** s_count = (int**)malloc(MAX_LEN * sizeof(int*));

  for (size_t i = 0; i < MAX_LEN; i++) {
    s_count[i] = (int*)malloc(MAX_LEN * sizeof(int));
  }

  int larger = 0;
  int smaller = 0;
  char* larger_string = malloc(MAX_LEN * sizeof(char));
  char* smaller_string = malloc(MAX_LEN * sizeof(char));

  // identify which string is longer, and assign shorter string to rows, longer to columns
  if (n > m) {
    larger = n;
    smaller = m;
    strcpy(larger_string, string1);
    strcpy(smaller_string, string2);
  }
  else {
    larger = m;
    smaller = n;
    strcpy(larger_string, string2);
    strcpy(smaller_string, string1);
  }

  for (int i = 0; i <= smaller; i++) {
    for (int j = 0; j <= larger; j++) {
        s_count[i][j] = 0;
    }
  }

  // perform LCS comparison according to DP algorithm
  for (int i = 1; i <= smaller; i++) {
    for (int j = 1; j <= larger; j++) {
      if (smaller_string[i-1] == larger_string[j-1]) {
        s_count[i][j] = s_count[i-1][j-1] + 1;
      }
      else if(s_count[i-1][j] >= s_count[i][j-1]){
        s_count[i][j] = s_count[i-1][j];
      }
      else {
        s_count[i][j] = s_count[i][j-1];
      }
    }
  }

  int lcs_length = s_count[smaller][larger];
  int lcs_counter = lcs_length - 1;
  int row_pos = smaller;
  int column_pos = larger;

  // backtrack along DP table
  while (row_pos > 0 && column_pos > 0) {
    if (smaller_string[row_pos-1] == larger_string[column_pos-1]) {
      column_pos = column_pos - 1;
      row_pos = row_pos - 1;
      lcs_actual[lcs_counter] = larger_string[column_pos];
      lcs_counter = lcs_counter - 1;
    }
    else if (s_count[row_pos-1][column_pos] > s_count[row_pos][column_pos-1]) {
      row_pos = row_pos - 1;
    }
    else {
      column_pos = column_pos - 1;
    }
  }

  // verify that LCS comparison was successful
  printf("LCS complete\n");
  return lcs_actual;
}


char* LPS(char* string){
  int n = strlen(string) - 1;
  int m = strlen(string);

  char* lps_actual = malloc(MAX_LEN * sizeof(char));
  char* string_prime = malloc(MAX_LEN * sizeof(char));;
  strcpy(string_prime, string);
  char* string_reverse = malloc(MAX_LEN * sizeof(char));

  // Reverse string in question
  for (size_t i = 0; i < m; i++) {
    string_reverse[i] = string_prime[n];
    n = n - 1;
  }

  lps_actual = LCS(string_prime, string_reverse);

  return lps_actual;
}


char* LTS(char* string){
  int string_length = strlen(string);
  char* lts_actual_left = malloc(MAX_LEN * sizeof(char));
  char* lts_actual_left_max = malloc(MAX_LEN * sizeof(char));
  char* lts_actual_right = malloc(MAX_LEN * sizeof(char));
  char* lts_actual_right_max = malloc(MAX_LEN * sizeof(char));
  char* lts_actual = malloc(MAX_LEN * sizeof(char));
  char temp_char;

  // counter variable to keep track of running time
  int iteration = 0;

  int n = string_length / 2;

  char* left_sub_string = malloc(MAX_LEN * sizeof(char));
  char* right_sub_string = malloc(MAX_LEN * sizeof(char));

  strcpy(left_sub_string, string);
  strcpy(right_sub_string, string);

  temp_char = left_sub_string[n];
  left_sub_string[n] = '\0';
  right_sub_string = right_sub_string + n + 1;

  lts_actual_right = LCS(left_sub_string, right_sub_string);

  strcpy(lts_actual_right_max, lts_actual_right);

  // compare left substring with right while moving the pointer to the right
  while(strlen(lts_actual_right) < strlen(right_sub_string)) {
    lts_actual_right = LCS(left_sub_string, right_sub_string);
    if (strlen(lts_actual_right) > strlen(lts_actual_right_max)) {
      strcpy(lts_actual_right_max, lts_actual_right);
    }
    right_sub_string = right_sub_string + 1;
    left_sub_string[n] = temp_char;
    n = n + 1;
    temp_char = left_sub_string[n];
    left_sub_string[n] = '\0';
    printf("iteration = %d\n", iteration);
    iteration = iteration + 1;
  }

  strcpy(left_sub_string, string);
  strcpy(right_sub_string, string);

  n = string_length / 2;

  left_sub_string[n] = '\0';
  right_sub_string = right_sub_string + n;

  strcpy(lts_actual_left, LCS(left_sub_string, right_sub_string));
  strcpy(lts_actual_left_max, lts_actual_left);

  // compare left substring with right while moving the pointer to the left
  while(strlen(lts_actual_left) < strlen(left_sub_string)) {
    lts_actual_left = LCS(left_sub_string, right_sub_string);
    if (strlen(lts_actual_left) > strlen(lts_actual_left_max)) {
      strcpy(lts_actual_left_max, lts_actual_left);
    }
    right_sub_string = right_sub_string - 1;
    n = n - 1;
    left_sub_string[n] = '\0';
    printf("iteration = %d\n", iteration);
    iteration = iteration + 1;
  }

  // compare left and right tandem subsequence lengths
  if (strlen(lts_actual_left_max) < strlen(lts_actual_right_max)) {
    strcpy(lts_actual, lts_actual_right_max);
  }
  else {
    strcpy(lts_actual, lts_actual_left_max);
  }

  char* lts_actual_duplicate = malloc(MAX_LEN * sizeof(char));

  strcpy(lts_actual_duplicate, lts_actual);

  strcat(lts_actual, lts_actual_duplicate);

  return lts_actual;
}


// Driver function
int main(int argc, char const *argv[]) {
  char* s1 = malloc(MAX_LEN * sizeof(char));
  char* s2 = malloc(MAX_LEN * sizeof(char));

  // verify that input is valid according to decimal alphabet
  int first_entry_is_valid = 0;
  int second_entry_is_valid = 0;

  while (first_entry_is_valid == 0 || second_entry_is_valid == 0) {

    first_entry_is_valid = 1;
    second_entry_is_valid = 1;

    printf("Enter first sequence: ");
    scanf("%s", s1);

    for (int i = 0; i < strlen(s1); i++) {
      if (isdigit(s1[i]) == 0) {
        printf("Error, non-digit character detected!\n");
        first_entry_is_valid = 0;
        break;
      }
    }

    printf("Enter second sequence: ");
    scanf("%s", s2);

    for (int i = 0; i < strlen(s2); i++) {
      if (isdigit(s2[i]) == 0) {
        printf("Error, non-digit character detected!\n");
        second_entry_is_valid = 0;
        break;
      }
    }
  }

  int string_length1 = strlen(s1);
  int string_length2 = strlen(s2);

  printf("The first sequence (length = %d) is:\n", string_length1);
  printf("%s\n", s1);

  char* lts1 = LTS(s1);
  int length1 = strlen(lts1);
  printf("# an LTS (length = %d) for the first sequence is:\n", length1);
  printf("%s\n", lts1);

  printf("The second sequence (length = %d) is:\n", string_length2);
  printf("%s\n", s2);

  char* lps2 = LPS(s2);
  int length2 = strlen(lps2);
  printf("# an LPS (length = %d) for the second sequence is:\n", length2);
  printf("%s\n", lps2);

  return 0;
}
