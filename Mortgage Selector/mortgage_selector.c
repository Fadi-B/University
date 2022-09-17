/*
 * C-Assignment.c
 *
 *  	Created on: 22 Mar 2021
 *      Author: s1969801
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** ###### Data size and buffer constants ###### */
#define DATA_SIZE 50         /* The size of the database to read from */
#define BUFFER_SIZE 80       /* The size of the buffer that will parse the variable bank names of the mortgages */

/** ######  constants for ASCII values  ######*/
#define QUIT 113     /* Decimal value of ASCII character 'q' */
#define EXIT -1      /* Exit condition returned when user specifies 'q' */
#define SPACE 32     /* Decimal value for ASCII character ' ' */
#define NEW_LINE 10  /* Decimal value for ASCII character of \n */
#define DIGIT_0 48   /* Decimal value for ASCII character 0 */
#define DIGIT_9 57	 /* Decimal value for ASCII character 9 */

/** structure to store the mortgage deals and the user input requirements */
struct mortgage{
	char *bank_name;
	int duration;
	float initial_rate;
	float final_rate;
	int loan_to_value;
	int product_fees;
} requirement;          /** Instantiate an instance of this structure exclusively for the user input requirements */

/** ###### The function prototypes ###### */
void init_data(struct mortgage[]);
void prompt_user();
void scan_user_requirements(struct mortgage *);
int compare_mortgage_deals(struct mortgage * , struct mortgage *);
struct mortgage * find_suitable_mortgage_deals(struct mortgage *, struct mortgage *);
void print_contents(struct mortgage *, int);
void sort(struct mortgage *, int);
int examine_quit();
void clear_data(struct mortgage *, int);


/** ###### Global variables ###### */
int DEALS = 0;  		 //Global variable to keep track of the number of suitable mortgage deals
int ACTIVE_DATA = 0; 	 //Global variable to keep track of the number of mortgages parsed

/**
 * Main function that runs the program
 */
int main(void)
{
	struct mortgage *suitable_deals;          /* Declare pointer that will store for the suitable deals found */
	struct mortgage *data = malloc(DATA_SIZE * sizeof(struct mortgage));    /* allocate memory for the database that will store the contents of the mortgage file */

	init_data(data);             		    /* initialise the database */

	int quit = 0;               		    /* integer that will determine when to quit */

	while(!(quit == EXIT)) {                /* Program will be based on an infinite loop until the exit condition is satisfied */

		prompt_user();					    /* Prompt user for requirement input */

		quit = examine_quit();              /* Check if user wants to quit*/


		if (quit == EXIT) {
			break;
		} else {
			requirement.duration = quit;    /* If the user did not want to quit then the argument must have been the minimum duration requirement */
			quit = 0;                       /* Reset quit variable*/
		}

		scan_user_requirements(&requirement);  /* fetch the remaining user requirements and store into the mortgage requirement */
		suitable_deals = find_suitable_mortgage_deals(data, &requirement);  /* find the deals that match user requirements*/

		if (suitable_deals == NULL) {          /* No suitable deals? Continue to prompt the user for new requirements */
			continue;
		}

		sort(suitable_deals, DEALS);           /* sort the deals that match user requirements according to specification */
		print_contents(suitable_deals, DEALS); /* print the deals that match user requirements*/

		clear_data(suitable_deals, DEALS);     /* free the current suitable deals before prompting for new user input */
	}

	return 0;
}

/** This function initialises the mortgage database by parsing the contents of the mortgage.txt file
 * @Param
 *  -> data - mortgage pointer that will point to an array of mortgages
 * */
void init_data(struct mortgage *data) {
	FILE *fin; /* define a file pointer */

	fin = fopen("mortgage.txt", "r"); /* open mortgage.txt file for read */

	int content;                /* Integer that will store the success of the file scan*/
	int counter = 0;            /* Integer that will keep track of the number of mortgages read */

	char buffer[BUFFER_SIZE];   /* Define buffer that will be used to dynamically allocate memory for the variable length bank names */

	for (int i = 0; i < DATA_SIZE; i++) {
		fscanf(fin, "%s", buffer);         /* read the name of the bank and store it into the buffer */
		data[i].bank_name = malloc(strlen(buffer) + 1);    /* +1 is for '\0' */
		strcpy(data[i].bank_name, buffer);     /* initialise the bank_name to the current contents of the buffer */

		content = fscanf(fin, "%d %f %f %d %d",          /* read the remaining information in the line in file */
			&data[i].duration,
			&data[i].initial_rate,
			&data[i].final_rate,
			&data[i].loan_to_value,
			&data[i].product_fees);

		if (content == EOF) {         /* if the file read was not successful then break */
			break;
		}

		counter++;                    /* update number of mortgages read */

	}

	ACTIVE_DATA = counter;        /* set the ACTIVE_DATA global variable to the number of mortgage files read */

	fclose(fin); /* close file properly */
}

/**
 * This function prompts the user for the input requirements
 */
void prompt_user() {

	char duration_prompt[] = "Minimum duration of deal (year)";         /* The prompt for the minimum duration of the mortgage*/
	char initial_rate_prompt[] = "Maximum initial interest rate (%)";   /* The prompt for the maximum initial interest rate of the mortgage*/
	char rate_thereafter_prompt[] = "Maximum rate thereafter (%)";      /* The prompt for the maximum rate thereafter of the mortgage*/
	char loan_to_value_prompt[] = "Minimum loan to value (%)";          /* The prompt for the minimum loan to value of the mortgage*/
	char product_fee_prompt[] = "Maximum product fees (£)";				/* The prompt for the maximum product fees of the mortgage*/
	char space[] = "  ";                                                  /* String of space used to separate the different prompts - default is two spaces*/

	printf("%s%s%s%s%s%s%s%s%s\n",    /* Prompt the user*/
			duration_prompt,
			space,
			initial_rate_prompt,
			space,
			rate_thereafter_prompt,
			space,
			loan_to_value_prompt,
			space,
			product_fee_prompt);
	fflush(stdout);     /* flush */

}

/**
 * This function scans the user input requirements and stores them in the requirement mortgage instance
 * @Param
 * 	-> requirement - address of the requirement mortgage instance
 */
void scan_user_requirements(struct mortgage *requirement) {

	requirement->bank_name = NULL;    /* Initialise bank name to the null pointer as the user requirements don't specify bank */

	scanf("%f %f %d %d",
			&requirement->initial_rate,
			&requirement->final_rate,
			&requirement->loan_to_value,
			&requirement->product_fees);  /* scan the user input requirements. The minimum duration requirement was previously examined
											    in the examination of the quit condition so no need to consider it*/
}

/*
 * This function compares a mortgage to the requirement mortgage and determines if it is a suitable deal
 * @Param
 *  -> deal - address of the mortgage deal
 *  -> requirement - address of the mortgage containing the user requirements
 * @Return
 *  -> returns an integer indicating the success of the comparison.
 *     If the mortgage suits the customer requirements, it returns 1 and otherwise 0
 */
int compare_mortgage_deals(struct mortgage *deal , struct mortgage *requirement) {
	if ((deal->duration >= requirement->duration) &&         /* compare mortgage deal to requirements */
		(deal->initial_rate <= requirement->initial_rate) &&
		(deal->final_rate <= requirement->final_rate) &&
		(deal->loan_to_value >= requirement->loan_to_value) &&
		(deal->product_fees <= requirement->product_fees)) {

		return 1;
	}

	return 0;
}

/*
 * This function finds all the suitable mortgage deals in the current database that satisfy user input requirements
 * @Param
 *  -> data - address for the database that is an array of mortgages
 *  -> requirement - address of the mortgage containing the user requirements
 * @Return
 *  -> mortgage pointer that holds the address of the a mortgage array that contains the suitable deals.
 *     If the suitable deals are found, the null pointer is returned
 */
struct mortgage * find_suitable_mortgage_deals(struct mortgage *data, struct mortgage *requirement) {

	int counter = 0;     /* counter to keep track of the number of suitable deals*/

	for (int i = 0; i < ACTIVE_DATA; i++) {

		if (compare_mortgage_deals(&data[i], requirement) == 1) {   /* If mortgage i is a suitable deal then update counter*/
			counter++;
		}

	}

	struct mortgage *suitable_deals = malloc(counter * sizeof(struct mortgage));    /* allocate memory for the suitable deals mortgage array */

	counter = 0;     /* reset counter */

	for (int i = 0; i < ACTIVE_DATA; i++) {    /* copy contents of the matching mortgage deals in the database into the suitable_deals array */

		if (compare_mortgage_deals(&data[i], requirement) == 1) {

			suitable_deals[counter].bank_name = malloc(strlen(data[i].bank_name) + 1);   /* +1 for '\0' */
			strcpy(suitable_deals[counter].bank_name, data[i].bank_name);				/* Initialise bank name for the suitable deal */

			suitable_deals[counter].initial_rate = data[i].initial_rate;				/* Initialise initial rate for the suitable deal */
			suitable_deals[counter].final_rate = data[i].final_rate;					/* Initialise final rate for the suitable deal */
			suitable_deals[counter].loan_to_value = data[i].loan_to_value;		     	/* Initialise loan to value for the suitable deal */
			suitable_deals[counter].product_fees = data[i].product_fees;		    	/* Initialise product fees for the suitable deal */
			suitable_deals[counter].duration = data[i].duration;				        /* Initialise duration for the suitable deal */
			counter++;
		}
	}

	DEALS = counter;        /* set the global variable DEALS to the number of suitable mortgage deals*/

	if (counter == 0) {     /* if no suitable deals were found return the null pointer*/
		return NULL;
	}

	return suitable_deals;
}


/*
 * This function prints out the contents of the mortgage deals specified
 * @Param
 *  -> data - address of the mortgage array that hold the mortgages to be printed
 *  -> size - the number of mortgages to be printed, i.e size of array
 */
void print_contents(struct mortgage *data, int size) {

	for (int i = 0; i < size; i++) {    /* print the contents of the mortgage array data*/
		printf("%s %d %.2f %.2f %d %d\n",
				data[i].bank_name,
				data[i].duration,
				data[i].initial_rate,
				data[i].final_rate,
				data[i].loan_to_value,
				data[i].product_fees);
	}
}


/*
 * This functions sorts the provided mortgage array and is based on the insertion sort algorithm.
 * The sorting sorts the mortgages based on increasing initial interest rate.
 * If the interest rate of two mortgages is equivalent then they are sorted based on increasing product fees.
 * If the corresponding initial interest rates and the product fees are equivalent then the mortgages are left in their current order.
 * @Param
 *  -> data - address of mortgage array containing the mortgages to be sorted
 *  -> deals - the number of mortgages in the array to be sorted
 */
void sort(struct mortgage *data, int deals) {

	struct mortgage x;        /* Declare mortgage variable - will keep track of the mortgage element in the array under consideration */
	int j;                    /* Declare integer j that will keep track of index of earlier elements in the array */

	for (int i = 0; i < deals; i++) {          /* iterate over the mortgage array */
		x = data[i];                           /* initialise x to hold the mortgage structure that is being considered */
		j = i - 1;                             /* index of first element before the x element */

		while((j >= 0) && data[j].initial_rate >= x.initial_rate) {                  /* If index j is index of earlier element and that element has an initial interest rate
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	that is higher or equal than that of the current mortgage x under consideration then sort */

			if (data[j].initial_rate == x.initial_rate) {                      /* If equal then sort based on product fees */

				if (data[j].product_fees > x.product_fees) {             /* If product fees of element j larger than fees of element x then swap positions */
					data[j + 1] = data[j];
				} else {
					break;               /* If product fees of element j not larger than earlier elements cannot have a product fee larger than element x so break*/
				}

			} else {
				data[j + 1] = data[j];                 /* Sort based on different initial interest rates */
			}

			j = j - 1;                 /* decrement j to consider the next earlier element */

		}

		data[j + 1] = x;              /* insert the mortgage under consideration into correct position */

	}
}

/*
 * This functions examines if the user specifies the quit condition.
 * If the user specifies the character 'q' then it means that the program should terminate.
 * If the user does not provide the character 'q' then it is assumed that the input is the argument
 * corresponding to the minimum duration requirement.
 * @Return
 *  -> returns an integer indicating the status of the quit condition.
 *     If the user specifies the character 'q' then -1 is returned indicating that we should terminate.
 *     Otherwise, the returned value corresponds to the minimum duration user requirement.
 */
int examine_quit() {
	int quit = getchar();     /* get the first char of user input in terms of an integer */

	int min_duration = 0;    /* declare variable to hold the min_duration argument if the user does not specify 'q'*/


	if (quit == QUIT) {     /* if the integer value of quit corresponds to the decimal ASCII code of 'q' then exit condition satisifed*/
		return -1;
	}

	while (!((quit <= DIGIT_9) && (quit >= DIGIT_0))) {    /*If first user input was not 'q' then want to ensure it is a digit and not an invalid input
	 	 	 	 	 	 	 	 	 	 	 	 so we loop until either the minimum duration is specified or the quit condition is provided. This also avoids issues with for example tab characters*/
		quit = getchar();

		if (quit == QUIT) {
				return -1;
		}
	}

	while(quit != SPACE && quit != NEW_LINE) {   /* parse the minimum duration argument character by character */
		quit = quit - DIGIT_0;                       /* convert quit to the integer digit that has been read */

		min_duration = (min_duration + quit) * 10;  /* add quit to min_duration and multiply by 10 until the whole number has been parsed*/

		quit = getchar();     /* get next digit in argument */
	}


	min_duration = min_duration / 10;    /* divide by 10 to get the real argument as we multiplied with an additional 10 for the last digit*/

	return min_duration;

}

/**
 * This function clears the mortgage data provided by freeing the memory space it occupies
 * @Param
 *  -> data - the address of the mortgage array to be freed
 *  -> size - the size of the mortgage array to be freed
 */
void clear_data(struct mortgage *data, int size) {
	for (int i = 0; i < size; i++) {  /* need to ensure that each bank name pointer for each mortgage is freed before freeing the mortgage array itself*/
		free(data[i].bank_name);
		data[i].bank_name = NULL;
	}

	free(data);    /* free the entire mortgage array*/
}

