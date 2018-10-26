#include <stdio.h>
#include <string.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {

   // Simply iterate through chars of pattern and check if there is a mis-match
   while(*pattern != '\0'){
     int num = 0; // counts repeated for +
     char c = *(pattern-1); //what it looks like...
     switch(*pattern){ // Check for 'special chars'

        case '+':
          if(*(pattern-1) == '.'){
            c = *(partial_line-1);
            while (*(partial_line-1) == c){
              partial_line++;
            }
            partial_line--;
          }

          if(c == *(pattern+1)){//If the char after + and before + are the same
            num++; //bare minimum 2 repeats.
            pattern++;
            while(c == *(pattern)){ //While they are the same...
              pattern++;
              num++;//count minimum repetitions for our case
            }
            for(int i = 1; i < num; i++){ // *(pattern-1) is allowed to repeat many times
               // increment our line until the char stops repeating
              if (*partial_line != c){ return 0; }
              partial_line++;
            }
            while (*(partial_line) == c){
              partial_line++;
            }
          }
          else{
            while (*(partial_line) == c){
              partial_line++;
            }
            pattern++;
          }

          break;

        case '.': // simply skip past this index.. doesnt need to be checked.
          while (*pattern == '.'){
            pattern++;
            partial_line++;
            if (*partial_line == '\0'){ return 0; }
            if (*pattern == '\0'){ return 1; }
          }
          break;

        case '\\':
          pattern++; // simply skip to next index.. it will not be a 'special case'.


        default:
          if (*pattern == '?' ) { //checking for empty string in the case of a single ? as the pattern
            if(*(pattern+1)== '\0'){ //if the pattern is ? and the next is null return 0 as an empty string
              return 0;
            }
          }
          if (*partial_line != *pattern && *(pattern+1) == '?') { // optional char is NOT present
            if(*(pattern+2) == '\0'){ return 1; }//if ? is at end of pattern
            if(*(pattern+2) == *(partial_line)) { pattern+=2; }
            else{
              return 0;
            }
          }


          else if(*partial_line == *pattern && *(pattern+1) == '?'){ // Optional char IS present
            if(*(pattern+2) == '\0'){ return 1; }//if ? is at end of pattern

            if(*(pattern+2) == *(partial_line+1)){
              pattern+=2;
              partial_line++;
            }
            else if(*(pattern+2) == *(partial_line)){
              if(*(pattern+2) == *(partial_line+1)) {pattern+=2; partial_line++;}
              else{ pattern+=2; }
            }
            else{return 0;}
          }


          if (*partial_line == *pattern){ // matches, NO QUESTION MARK HERE
            partial_line++;
            pattern++; // increment to the next chars
          }

          else {// NO MATCH NO QUESTION MARK
            return 0;
          }
          break;
      } // end switch

  } // end while

  // If we broke out of the for loop, then we have a matching pattern
  return 1;
}

/**
 * You may assume that all strings are properly null terminated
 * and will not overrun the buffer set by MAXSIZE
 *
 * Implementation of the rgrep matcher function
 */
int rgrep_matches(char *line, char *pattern) {

    // loop strlen-1 times because the null char adds 1 to len
    while(*line != '\0'){
      // this statement sends a partial_line to matches_leading, that starts at char[i] as i increments, partial_line gets shorter.
      if(matches_leading(line, pattern)) { return 1; }
      line++;
    }

    // if we break out of the for loop, that means there is no match.
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PATTERN>\n", argv[0]);
        return 2;
    }

    /* we're not going to worry about long lines */
    char buf[MAXSIZE];

    while (!feof(stdin) && !ferror(stdin)) {
        if (!fgets(buf, sizeof(buf), stdin)) {
            break;
        }
        if (rgrep_matches(buf, argv[1])) {
            fputs(buf, stdout);
            fflush(stdout);
        }
    }

    if (ferror(stdin)) {
        perror(argv[0]);
        return 1;
    }

    return 0;
}
