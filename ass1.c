/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Luong An Khang (ID: 1301266)
   Dated:     15/09/2023 (DD/MM/YYYY)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE    500    /* max number of digits per integer value */
#define LINELEN    999    /* maximum length of any input line */
#define NVARS    26    /* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR    (-1)    /* error return value from some functions */
#define PROMPT    "> "    /* the prompt string for interactive input */

/* error message for overflow*/
#define OVERFLOW_MSG "Overflow: Exceed the int size of 500 digits"
/* error message for zero division */
#define ZERO_DIV_MSG "Zero division: Cannot divide by zero" 

#define PRINT    '?'    /* the print operator */
#define ASSIGN    '='    /* the assignment operator */
#define PLUS    '+'    /* the addition operator */
#define MULT    '*'    /* the multiplication operator */
#define POWR    '^'    /* the power-of operator */
#define DIVS    '/'    /* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0    /* integer 0 */
#define INT_ONE 1    /* integer 1 */
#define INT_TEN  10    /* integer 10 */
#define NUMBER_BASE 10 /* The base of number, for passing carry in +/- */

#define INVALID_LEN (-1) /* length of invalid longint_t */
#define ONE_LEN 1 /* length of one in longint_t */
#define ZERO_LEN 0 /* length of zero in longint_t */
#define MAX_POW 1660 /* maximum power for base value >= 2 */
#define MAX_POW_LEN 4 /* maximum length of power */
#define TABLE_MULT_NUM 9 /* number of multiples in a 1-9 multiply table */

/* The multiply table index is off by 1 (eg: 1st multiple at 0 index)*/
#define TABLE_ONE_OFF 1

#define EQUAL 0 /* used for comparison function */

/* Placeholder typedef for skeleton code
*/
typedef int longint_t[INTSIZE + 1];
#define LONG_ZERO {0}
#define LONG_ONE  {1, 1}


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
    longint_t second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t var);
void do_assign(longint_t var1, longint_t var2);
void do_plus(longint_t var1, longint_t var2);
void do_multiply(longint_t var1, longint_t var2);
void do_exp(longint_t var1, longint_t var2);
void do_divide(longint_t var1, longint_t var2);
void zero_vars(longint_t vars[]);
void parse_num(char *rhs, longint_t new_num);

/* Helper functions */

/* For multiply */
void do_multiply_single(longint_t var, int n);
void compute_multiply_table(longint_t table[], longint_t var);
void right_shift_number(longint_t var, int n);

/* For exponentiation */
int convert_to_int(longint_t var);
void do_exp_int(longint_t var1, int var2);

/* For division */
/* compute_multiply_table will be reused for division */
void reverse_assign(longint_t var, longint_t rev_var);
void add_one_digit(longint_t var, int n);
void greater_subtract(longint_t var1, longint_t var2);
int comparison(longint_t var1, longint_t var2);
int divide_single(longint_t var1, longint_t var2_table[]);



/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
    char line[LINELEN+1] = {0};
    longint_t vars[NVARS];

    zero_vars(vars);
    print_prompt();
    while (read_line(line, LINELEN)) {
        if (strlen(line) > 0) {
            /* non empty line, so process it */
            process_line(vars, line);
        }
        print_prompt();
    }

    print_tadaa();
    return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
    if (isatty(fileno(stdin))) {
        fprintf(stderr, "> ");
        fflush(stderr);
    }
}

void
print_tadaa() {
    /* all done, so pack up bat and ball and head home,
       getting the exact final lines right is a bit tedious,
       because input might be coming from a file and output
       might be going to a file */
    if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
        printf("\n");
    }
    printf("ta daa!!!\n");
    if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
        fprintf(stderr, "\n");
    }
}

void
print_error(char *message) {
    /* need to write an error message to the right place(s)
    */
    if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
        fprintf(stderr, "%s\n", message);
        fflush(stderr);
    }
    if (!isatty(fileno(stdout))) {
        printf("%s\n", message);
    }
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
    int i=0, c;
    while (((c=getchar())!=EOF) && (c!='\n')) {
        if (i<maxlen && !isspace(c)) {
            line[i++] = c;
        }
    }
    line[i] = '\0';
    /* then, if the input is coming from a file or the output
       is going to a file, it is helpful to echo the input line
       and record what the command was */
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
        printf("%s%s\n", PROMPT, line);
    }
    return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
    int varnum, optype, status;
    longint_t second_value;

    /* determine the LHS variable, it
       must be first character in compacted line
    */
    varnum = to_varnum(line[0]);
    if (varnum==ERROR) {
        print_error("invalid LHS variable");
        return;
    }

    /* more testing for validity 
    */
    if (strlen(line)<2) {
        print_error("no operator supplied");
        return;
    }

    /* determine the operation to be performed, it
       must be second character of compacted line
    */
    optype = line[1];
    if (strchr(ALLOPS, optype) == NULL) {
        print_error("unknown operator");
        return;
    }

    /* determine the RHS argument (if one is required),
       it must start in the third character of compacted line
    */
    if (optype != PRINT) {
        if (strlen(line)<3) {
            print_error("no RHS supplied");
            return;
        }
        status = get_second_value(vars, line+2, second_value);
        if (status==ERROR) {
            print_error("RHS argument is invalid");
            return;
        }
    }

    /* finally, do the actual operation
    */

    if (optype == PRINT) {
        do_print(varnum, vars[varnum]);
    } else if (optype == ASSIGN) {
        do_assign(vars[varnum], second_value);
    } else if (optype == PLUS) {
        do_plus(vars[varnum], second_value);
    // you will need to add further operators here
    } else if (optype == MULT) {
        do_multiply(vars[varnum], second_value);
    } else if (optype == POWR) {
        do_exp(vars[varnum], second_value);
    } else if (optype == DIVS) {
        do_divide(vars[varnum], second_value);
    } else {
        print_error("operation not available yet");
        return;
    }
    return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
    int varnum;
    varnum = ident - CH_A;
    if (0<=varnum && varnum<NVARS) {
        return varnum;
    } else {
        return ERROR;
    }
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
            longint_t second_value) {
    char *p;
    int varnum2;
    if (isdigit(*rhsarg)) {
        /* first character is a digit, so RHS is a number
           now check the rest of RHS for validity */
        for (p=rhsarg+1; *p; p++) {
            if (!isdigit(*p)) {
                /* nope, found an illegal character */
                return ERROR;
            }
        }
        /* nothing wrong, ok to convert */
        parse_num(rhsarg, second_value);
        return !ERROR;
    } else {
        /* argument is not a number, so should be a variable */
        varnum2 = to_varnum(*rhsarg);
        if (varnum2==ERROR || strlen(rhsarg)!=1) {
            /* nope, not a variable either */
            return ERROR;
        }
        /* and finally, get that variable's value */
        do_assign(second_value, vars[varnum2]);
        return !ERROR;
    }
    return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
    int i;
    longint_t zero = LONG_ZERO;
    for (i=0; i<NVARS; i++) {
        do_assign(vars[i], zero);
    }
    return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Convert string to internal form of an array
   storing the length of the number in index 0, and
   the digits in reversed orders from index 1.
   Note: 0 will have length ZERO_LEN, and 
   overflowed number will have length INVALID_LEN 
*/
void
parse_num(char *rhs, longint_t new_num) {
    int len_str = strlen(rhs);
    
    /* Handle leading 0 
    by searching for first non-zero position */ 
    int start = len_str;
    for (int i=0; i<len_str; i++) {
        if (rhs[i] != CH_ZERO) {
            start = i;
            break;
        }
    }


    /* Find actual length without leading 0 */
    int len_var = len_str - start;

    /* Guard overflow */ 
    if (len_var > INTSIZE) {
        printf(OVERFLOW_MSG);
        exit(EXIT_FAILURE);
    }

    new_num[0] = len_var;

    /* Handle zero (which has unique length) */
    if (len_var == ZERO_LEN) {
        return;
    }

    /* Handle non-zero, positive values*/
    for (int i=1; i<=len_var; i++) {
        new_num[i] = rhs[len_str - i] - CH_ZERO;
    }
 
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t var) {
    printf("register %c: ", varnum+CH_A);
    
    int len = var[0];
    /* Handle zero */
    if (len == ZERO_LEN) {
        printf("%c\n", CH_ZERO);
        return;
    }

    /* Handle non-zero */
    for (int i=1; i<=len; i++) {
        printf("%d", var[len + 1 - i]);
        
        /* Handle the thousand "," separator */
        if (i != len && (len - i) % PUT_COMMAS == INT_ZERO) {
            printf("%c", CH_COM);
        }
    }

    printf("\n");

}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2

*/
void
do_assign(longint_t var1, longint_t var2) {
    int len_var2 = var2[0];

    /* Handle 0 by assigning a zero length */
    if (len_var2 == ZERO_LEN) {
        var1[0] = ZERO_LEN;
        return;
    }

    /* Handle non-zero value */
    var1[0] = len_var2;
    for (int i=1; i<=len_var2; i++) {
        var1[i] = var2[i];
    }
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2;

   Note that: the intialiser "zero_vars" sets all locations to 0,
   but technically, all the points beyond the length of the number
   is not quite defined, so guard will be provided against that.
*/
void
do_plus(longint_t var1, longint_t var2) {
    int len_var1 = var1[0];
    int len_var2 = var2[0];

    /* Check if var2 is 0, 
    then nothing needs to change */ 
    if (len_var2 == ZERO_LEN) {
        return;
    }

    int max_len = len_var1;
    if (len_var2 > max_len) {
        max_len = len_var2;
    }

    /* Initialize sum length */
    var1[0] = max_len;

    /* Initialize carry, 
    and set counter outside to
    use for changing length later */
    int carry = INT_ZERO;
    int i;
    for (i=1; i<=max_len; i++) {
        int cur_plus;
        int cur_total;

        /* Guard for different lengths between var_1 and var_2 */
        if (len_var2 < i && i <= len_var1) {
            /* Pass through all of var2, but not var1 */
            cur_plus = var1[i];
        } else if (len_var1 < i && i <= len_var2) {
            /* Pass through all of var1, but not var2 */
            cur_plus = var2[i];
        } else if (i <= len_var1 && i <= len_var2) {
            /* Still inside the length of var1 and var2 */
            cur_plus = var1[i] + var2[i];
        }

        cur_total = cur_plus + carry;
        /* Find the digit */
        var1[i] = cur_total % NUMBER_BASE;
        /* Find the carry */
        carry = cur_total / NUMBER_BASE;
    }

    /* Add the remaining carry to the additional digit 
    at i and increment the length (if applicable) */
    if (carry != INT_ZERO) {
        /* Guard against overflow */
        if (i > INTSIZE) {
            printf(OVERFLOW_MSG);
            exit(EXIT_FAILURE);
        }

        var1[i] = carry;
        var1[0]++;
    }

}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/


/* MULTIPLY */
/* Helper functions:
   Multiply large integer with a single digit value
   that is not zero (from 1 to 9 inclusively). 
*/
void
do_multiply_single(longint_t var, int n) {
    /* Handle 1 by just return the value unchanged */
    if (n == INT_ONE) {
        return;
    }

    /* Handle 2 and above */
    int len_var = var[0];
    
    /* Initialize carry, 
    and set counter outside to
    use for changing length later */
    int carry = INT_ZERO;
    int i;
    for (i=1; i <= len_var; i++) {
        int cur_total;
        cur_total = n * var[i] + carry;
        /* Find each digit */
        var[i] = cur_total % NUMBER_BASE;
        /* Update carry */
        carry = cur_total / NUMBER_BASE;
    }

    /* Add the remaining carry to the additional digit 
    and increment the length */
    if (carry != INT_ZERO) {
        /* Guard against overflow,
        but do not exit, since this might
        be stored as pre-computation. 
        Instead, set an invalid value with length = -1 */
        if (i > INTSIZE) {
            var[0] = INVALID_LEN;
            return;
        }

        var[i] = carry;
        var[0]++;
    }
}



/* Helper function:
   Compute the multiply table of var from 1 to 9.
   Used for both multiply and divide.

   Multiply table of var:
   A table (array) containing the multiples of 
   var from 1 to 9.

   Eg: 
   Multiply table = {var * 1, var * 2, ..., var * 9} 
*/
void 
compute_multiply_table(longint_t table[], longint_t var) {
    for (int n=1; n <= TABLE_MULT_NUM; n++) {
        do_assign(table[n - TABLE_ONE_OFF], var);
        do_multiply_single(table[n - TABLE_ONE_OFF], n);
    }
}



/* Try to to multiply 10^n (n >= 0)
   by shift the value to the right, 
   then padding with zeros at the start;
   Note: The shifting has no effect on 0.
   
   eg: 123 is represented as [3,3,2,1],
   after 2-shift -> [5,0,0,3,2,1] 
*/
void
right_shift_number(longint_t var, int n) {
    int len_var = var[0];

    /* Handle var = zero or n = 0, 
    which does not need shifting, since
    0 * 10^n = 0 and var * 10^0 = var */
    if (len_var == ZERO_LEN || n == INT_ZERO) {
        return;
    }

    /* Handle non-zero*/
    int new_len = len_var + n;
    
    /* Guard overflow */
    if (new_len > INTSIZE) {
        printf(OVERFLOW_MSG);
        exit(EXIT_FAILURE);
    }

    /* Update length */
    var[0] = new_len;
    
    /* Shift existing values by n */
    for (int i=len_var; i >= 1; i--) {
        var[i + n] = var[i]; 
    }

    /* Padding with 0 */
    for (int i=1; i <= n; i++) {
        var[i] = INT_ZERO;
    }    
}



/* Multiply var1 and var2, then assign to
   var1. Use primary school method of 
   multiplication (with a multiply table).

   Complexity:
   With n = Number of digits of variable

   For helper functions:
   - Compute do_multiply_single: O(n)
   - Compute multiply table: O(n)
   (Do multiply single 9 times)
   - Right shift number: O(n)
   - do_assign: O(n)
   - do_plus: O(n)

   For each digit of var2 (which has n digits):
   -  Perform assignment from multiply 
   table: O(n)
   - Shift the value and add to `result variable`:
   O(n)

   Thus, repeating this process n times leading to
   O(n^2) as required.
*/
void
do_multiply(longint_t var1, longint_t var2) {
    /* Compute the multiply table of var2 from 1 to 9.
    Note: the table index is off by 1.
    Eg: table[0] = var2 * 1 */
    longint_t table[TABLE_MULT_NUM];
    compute_multiply_table(table, var1);

    /* Now, we only need to look up the table, 
    with no need to re-compute.
    Primary school method. */

    int len_var2 = var2[0];

    /* Do the multiplication digit-by-digit backward,
    then add the shifted results together */
    longint_t result = LONG_ZERO;
    longint_t temp;

    for (int i=1; i<=len_var2; i++) {
        int cur_digit2 = var2[i];
        /* Skip zero */
        if (cur_digit2 == INT_ZERO) {
            continue;
        }

        /* Lookup the multiplication table,
        and assign to temp */
        do_assign(temp, table[cur_digit2 - TABLE_ONE_OFF]);

        /* Guard against overflow during multiplication */
        if (temp[0] == INVALID_LEN) {
            printf(OVERFLOW_MSG);
            exit(EXIT_FAILURE);
        }
        
        /* Shift appropriately by (i-1) place */
        right_shift_number(temp, i - 1);

        /* Add the shifted value to result */
        do_plus(result, temp);
    }

    /* Place result into var1 */
    do_assign(var1, result);
}

/****************************************************************/
/* EXPONENTIATION */

/* Helper function:
   Convert longint_t var that is <= MAX_POW.
   Return ERROR for invalid var out of the range 
*/
int 
convert_to_int(longint_t var) {
    int len_var = var[0];

    /* Handle var with more than 4 digits */
    if (len_var > MAX_POW_LEN) {
        return ERROR;
    }

    /* Handle var = 0 */
    if (var[0] == ZERO_LEN) {
        return INT_ZERO;
    }

    /* Set var_int to first digit */
    int var_int = var[len_var];
    /* Add each subsequent digits */
    for (int i=len_var - 1; i >= 1; i--) {
        var_int = var_int * NUMBER_BASE + var[i];
    }

    /* Handle overflow */
    if (var_int > MAX_POW) {
        return ERROR;
    }

    return var_int;
}



/* Helper function:
   Find exponentiation by repeated multiplication,
   as required by assignment 
*/
void 
do_exp_int(longint_t var1, int var2) {    
    longint_t temp = LONG_ONE;
    for (int i=1; i <= var2; i++) {
        do_multiply(temp, var1);
    }
    do_assign(var1, temp);
}



/* Calculate var1 ^ var2, and assign the 
   result back to var1, with guard
   preventing overflow.
*/
void
do_exp(longint_t var1, longint_t var2) {
    /* Handle when var1 = 0 or var1 = 1,
    when 0^n = 0 and 1^n = 1 */
    if (var1[0] == ZERO_LEN || (var1[0] == ONE_LEN && var1[1] == INT_ONE)) {
        return;
    }

    /* Convert var2 to int as range permitted */
    int var2_int = convert_to_int(var2);

    /* Handle overflow */
    if (var2_int == ERROR) {
        printf(OVERFLOW_MSG);
        exit(EXIT_FAILURE);
    }

    /* Pass onto the do_exp_int helper */
    do_exp_int(var1, var2_int);
}

/****************************************************************/
/* DIVISION */

/* Helper functions:
   Reverse rev_var, and assign the reversed
   version to var 
*/
void
reverse_assign(longint_t var, longint_t rev_var) {
    int len_rev_var = rev_var[0];
    var[0] = len_rev_var;

    for (int i=1; i <= len_rev_var; i++) {
        var[i] = rev_var[len_rev_var + 1 - i];
    }
}



/* Helper functions:
   Shift 1 place, add 1 digit at the end.
   Note: For zero, no shifting required;
   update length and first digit directly.
*/
void
add_one_digit(longint_t var, int n) {
    /* If var == 0 and n == 0, 
    return early as nothing changes */
    if (var[0] == INT_ZERO && n == INT_ZERO) {
        return;
    }

    /* if var is 0 and n!=0, then simply put n  
    into first place and update length,  
    since the shift ignores 0 (as 0 * 10^n = 0) */
    if (var[0] == INT_ZERO) {
        var[1] = n;
        var[0] = ONE_LEN;
    } else {
        /* Non-zero values */
        right_shift_number(var, INT_ONE);
        var[1] = n;
    }
}



/* Helper function:
   Subtract var2 from var1 then assign to var1. 
   var1 MUST be larger or equal to var 2.

   Guard should be provided before placed into 
   the function; 
*/
void
greater_subtract(longint_t var1, longint_t var2) {
    int len_var1 = var1[0];
    int len_var2 = var2[0];

    /* Handle when var2 is 0 */
    if (len_var2 == INT_ZERO) {
        return;
    }


    /* Iterate through the length of var1 (since it's the largest variable) */
    int max_len = len_var1;
    int new_len = ZERO_LEN;

    int carry = INT_ZERO;
    
    for (int i=1; i <= max_len; i++) {
        int cur_digit;
        int cur_subtract;
        int cur_result;

        /* Since the digits beyond len_var2 in var2 is undefined, 
        need to guard against that */
        if (i > len_var2) {
            cur_subtract = INT_ZERO;

            /* Return early when the remain/carry is 0, and 
            we have finished passing through all of var2 length */
            if (carry == INT_ZERO) {
                return;
            }
        } else {
            cur_subtract = var2[i];
        }

        /* Computer current digit and carry */
        cur_result = var1[i] - cur_subtract - carry;
        if (cur_result < INT_ZERO) {
            cur_digit = cur_result + NUMBER_BASE;
            carry = INT_ONE;
        } else {
            cur_digit = cur_result;
            carry = INT_ZERO;
        }

        var1[i] = cur_digit;

        /* If the digit is not zero, record its position as latest length */
        if (cur_digit != INT_ZERO) {
            new_len = i;
        }
    }

    /* Update new length (equal to position of the last non-zero digit) */
    var1[0] = new_len;
}



/* Helper function:
   Compare two variables of type longint_t.
   Return negative if var1 < var2, 
   0 if equal, and positive if var2 > var1 
*/
int
comparison(longint_t var1, longint_t var2) {
    int len_var1 = var1[0];
    int len_var2 = var2[0];

    /* Compare length difference */
    if (len_var1 != len_var2) {
        return len_var1 - len_var2;
    }

    /* Check for digit difference */
    for (int i=len_var1; i>=1; i--) {
        if (var1[i] != var2[i]) {
            return var1[i] - var2[i];
        }
    }

    /* No difference */
    return EQUAL;
}



/* Helper function:
   Find the quotient var1/ var2, then assign to var1.
   The division var1/var2 must be guaranteed to be from
   1 to 9. This function find the single-digit result. 

   A multiply table (as in multiply) is used to avoid too much 
   re-calculation. Linear search is used:
   Simply go from 9 to 1 to find the first multiple that
   is smaller or equal to var1. If not found, return 0.

   Note: As there are fixed 9 multiples to check, O(1)
   search.

   Complexity: O(n), with n being the length of var1 
   (with the comparison being the limiting factor)
*/
int
divide_single(longint_t var1, longint_t var2_table[]) {
    /* The multiply table array is off by one, since
    index starts at 0. Eg: var2 * 1 is in var2_table[0] */
    for (int i=(TABLE_MULT_NUM - TABLE_ONE_OFF); i>=INT_ZERO; i--) {
        /* Guard if there is an overflow value in the multiply table */
        if (var2_table[i][0] == INVALID_LEN) {
            continue;
        }

        /* Take the first multiplication that is <= var1.
        Return the division result */
        if (comparison(var2_table[i], var1) <= INT_ZERO) {
            return i+TABLE_ONE_OFF;
        }
    }

    /* If no multiple from 1-9 is smaller than var1, 
    then 0 is the answer */
    return INT_ZERO;
}



/* Divide var1 by var2, then assign the result to var1.

   Complexity:
   With n = the length of the longer/ relevant variable:
   - Compute multiply table: O(n)
   - Compute divide_single: O(n)
   - Compute greater_subtract: O(n)
   - Compute add_one_digit: O(n)
   - Compute reverse_assign: O(n)

   Compute multiply table and reverse_assign is done
   once: O(n)

   For roughly each digit of var1, the program:
   - Compute divide_single, greater_subtract and
   add_one_digit: O(n)

   Thus, the total complexity is roughly O(n^2).
*/
void
do_divide(longint_t var1, longint_t var2) {
    int len_var1 = var1[0];
    int len_var2 = var2[0];

    /* Handle zero */
    if (len_var2 == ZERO_LEN) {
        printf(ZERO_DIV_MSG);
        exit(EXIT_FAILURE);
    }

    if (len_var1 == ZERO_LEN){
        return;
    }

    /* If var1 < var2, the result is 0 */
    if (comparison(var1, var2) < INT_ZERO) {
        var1[0] = ZERO_LEN;
        return; 
    }
    
    /* For convenient and efficiency, the quotient will be 
    represented in the forward order to avoid shifting. 
    Eg: 100 -> {3, 1, 0, 0}
    rev_quotient = reversed quotient

    It will be reversed back to normal form after 
    completion of calculation.
    */
    longint_t temp;
    longint_t rev_quotient;

    /* Initialise temp to be the first `len_var2` digits of var1 */
    temp[0] = len_var2;
    for (int i=0; i<len_var2; i++) {
        temp[len_var2 - i] = var1[len_var1 - i];
    }

    /* Initialise rev_quotient */
    rev_quotient[0] = ZERO_LEN;

    /* Compute the multiply table */
    longint_t var2_table[TABLE_MULT_NUM];
    compute_multiply_table(var2_table, var2);

    /* Iterate from digit by digit from len_var2-th digit 
    up until the last digit of var1 */
    int start = len_var1 - len_var2 + 1;

    for (int i = start; i>=1; i--) {
        int cur_result = divide_single(temp, var2_table);

        /* Subtract the division from temp 
        when cur_result is not 0 */
        if (cur_result > INT_ZERO) {
            /* Parameters guarantees to be valid, 
            since temp >= var2 as the quotient (temp/ var2) >= 1
            so no need for guard */
            greater_subtract(temp, var2_table[cur_result - TABLE_ONE_OFF]);
        } 

        /* Add the result as a digit into quotient and update length.
        Check for leading zero (zero digit added to zero length), 
        which we won't add */
        if (rev_quotient[0] != ZERO_LEN || cur_result != INT_ZERO) {
            rev_quotient[0]++; 
            rev_quotient[rev_quotient[0]] = cur_result; 
        }

        /* If not at the end of var1, add 1 more digit to temp */
        if (i > 1) {
            add_one_digit(temp, var1[i-1]);
        }
    }

    /* Finish division, reverse the quotient to backward form,
    then assign to var1 */
    reverse_assign(var1, rev_quotient);
}


/*
    ALGORITHMS ARE FUN
*/
