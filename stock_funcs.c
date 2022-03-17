// stock_funcs.c: support functions for the stock_main program.

#include "stock.h"

// PROBLEM 1: Allocate a new stock struct and initialize its fields.
// Integer fields like 'count' and 'lo_index' should be initialied to
// -1.  Pointer fields like 'prices' should be initialized to
// NULL. The stock should be heap-allocated using malloc() and
// returned.  Since this is an allocation function, no use of 'free()'
// should appear.
stock_t *stock_new(){
  // WRITE ME
  stock_t *my_stock = malloc(sizeof(stock_t));
  my_stock->data_file = NULL;              // name of the data file stock data was loaded from
  my_stock->count = -1;                    // length of prices array
  my_stock->prices = NULL;               // array of stock prices at different time points
  my_stock->lo_index = -1;                 // index of the lowest price
  my_stock->hi_index = -1;                 // index of the highest price
  my_stock->best_buy = -1;                 // index at which to buy to get best profit
  my_stock->best_sell = -1; 
  return my_stock;               // index at which to sell to get best profit stock_t;
}



// PROBLEM 1: Free a stock. Check the 'data_file' and 'prices' fields:
// if they are non-NULL, then free them. Then free the pointer to
// 'stock' itself.
void stock_free(stock_t *stock){
  // WRITE ME
  if(stock->data_file != NULL){
    free(stock->data_file);
  }
  if(stock->prices != NULL){
    free(stock->prices);
  }
  free(stock);   //ask about how to free the poitner to the stock itself
  return;
}

// PROBLEM 1: Prints data about a stock that is passed in via a
// pointer. Uses the syntax ptr->field to access fields of the struct
// pointed by 'stock'.  Output follows the general convention:
//
// ==STOCK DATA==
// data_file: data/stock-jagged.txt                      
// count: 15
// prices: [103.00, 250.00, 133.00, ...]
// lo_index:  8
// hi_index:  11
// best_buy:  8
// best_sell: 11
// profit:    232.00
//
// Each line prints a field of the stock_t struct. In all cases,
// floating point numbers are printed with 2 decimal digits of
// accuracy.
//
// NULLS FOR FIELDS
// The fields 'data_file' and 'prices' may be NULL in which case they
// will be printed specially as in
// 
// data_file: NULL
// prices: NULL
//
// This requires a manual if/else check for NULL values for these
// pointers.
// 
// PRICES FIELD
// When printing the 'prices' field, if the 'count' field is 0 to 3,
// print the entire array as in
//
// prices: []                        # count == 0
// prices: [70.00]                   # count == 1
// prices: [50.00, 90.00]            # count == 2
// prices: [59.00, 45.00, 103.00]    # count == 3
//
// Otherwise, print the first 3 elements with a ... at the end as in
//
// prices: [10.00, 20.00, 30.00, ...] # count > 3
//
// PROFIT
// There is no 'profit' field in the struct. Instead, calculate the
// profit as the difference between the price at the 'best_sell' index
// and 'best_buy' index.  If these indices are -1 indicating the best
// buy/sell time is not known or not viable, print a proit of 0.0
void stock_print(stock_t *stock){
  // WRITE ME
  printf("==STOCK DATA==\n");
  if(stock->data_file != NULL){
    printf("data_file: %s\n", stock->data_file);
  }else{
    printf("data_file: NULL\n");
  }
  printf("count: %d\n", stock->count);  //Prints the count. Shouldn't be an error so no if/else necessary I think

  if(stock->prices != NULL){  //the start of the if/elseif/else statements for the possible prices
    if(stock->count == 0){
      printf("prices: []\n");
    }
    else if(stock->count == 1){
      printf("prices: [%.2f]\n", stock->prices[0]);
    }
    else if(stock->count == 2){
      printf("prices: [%.2f, %.2f]\n", stock->prices[0], stock->prices[1]);
    }
    else if(stock->count == 3){
      printf("prices: [%.2f, %.2f, %.2f]\n", stock->prices[0], stock->prices[1], stock->prices[2]);
    }
    else if(stock->count > 3){
      printf("prices: [%.2f, %.2f, %.2f, ...]\n", stock->prices[0], stock->prices[1], stock->prices[2]);
    }
  }else{
    printf("prices: NULL\n");
  }
  printf("lo_index: %d\n", stock->lo_index);
  printf("hi_index: %d\n", stock->hi_index);
  
  printf("best_buy: %d\n", stock->best_buy);
  printf("best_sell: %d\n", stock->best_sell);
  
  if(stock->best_buy == -1 || stock->best_sell == -1){      //profit print part
    printf("profit: 0.00\n");
  }else{
    printf("profit: %.2f\n", stock->prices[stock->best_sell] - stock->prices[stock->best_buy]);
  }
  return;
}


// PROBLEM 1: Sets the index of 'lo_index' and 'hi_index' fields of
// the stock to be the positions in 'prices' of the lowest and highest
// values present in it. Uses a simple loop over the array 'prices'
// which is 'count' elements long to examine each for high/low. If
// 'count' is zero, makes no changes to 'lo_index' and 'hi_index'.
void stock_set_hilo(stock_t *stock){
  // WRITE ME
  double low = 10000; double high = 0; //values of the prices at i
  if(stock->count == 0){
    return;
  }
else{
  for(int i = 0; i < stock->count; i++){
    if(stock->prices[i] > high){
      high = stock->prices[i];
      stock->hi_index = i;
    }
    if(stock->prices[i] < low){
      low = stock->prices[i];
      stock->lo_index = i;
    }
  }
}
  return;
}
  
// PROBLEM 2: Sets the 'best_buy' and 'best_sell' fields of 'stock'.
// This corresponds to the pair which produces the best profit. On
// determining the best buy/sell indices which produce a positive
// profit, sets these fields in 'stock' and returns 0. If there is no
// buy/sell point which would result in a positive profit, sets the
// 'best_buy' and 'best_sell' indices to -1 and returns -1. Always
// calculates the earliest buy/sell pair of indices that would get the
// best profit: if 5,8 and 5,9 and 7,10 all give the same, maximal
// profit, the best buy/sell indices are set to 5,7.
// 
// ALGORITHM NOTES
// One intuitive algorithm to compute this is to try every possible
// buy index (outer loop) and every possible sell index after it
// (inner loop) looking for the maximum profit produced in it. This is
// a O(N^2) algorithm with N=count.  Using this algorithm is a good
// start. Some MAKEUP CREDIT will be awarded for implementing a more
// efficient, O(N) algorithm here. See the specification for more details.
int stock_set_best(stock_t *stock){
  // WRITE ME
stock->best_buy = -1;
stock->best_sell = -1;
double best_profit = 0.0;
for(int i = 0; i < stock->count -1; i++){
  for(int j = i + 1; j < stock->count; j++){
    if(stock->prices[j] - stock->prices[i] > best_profit){
      stock->best_buy = i;
      stock->best_sell = j;
      best_profit = stock->prices[j] - stock->prices[i];
    }
  } 
}
  if(stock->best_buy == -1 && stock->best_sell == -1){
    return -1;
  }return 0;
}


// PROBLEM 2: Opens file named 'filename' and counts how many times
// the '\n' newline character appears in it which corresponds to how
// many lines of text are in it.  Makes use of either fscanf() with
// the %c format to read single characters or alternative I/O
// functions like fgetc(). Closes the file before returning a count of
// how many lines are it it.  If for any reason the file cannot be
// opened, prints a message like
//
// Could not open file 'not-there.txt'
//
// and returns -1 to indicate failure.
int count_lines(char *filename){
  // WRITE ME
//   while(ret != EOF){
//     int ret =                                     // # tokens read or EOF
//     printf("%c tokens read\n",ret);               // print number of tokens read
//   }
//   if(ret == EOF){                               // check for end of file
//       printf("WARNING: END OF FILE at iteration %d\n",i);
//       break;
//     }
char c;
int count = 0;
FILE *fin = fopen(filename,"r");
if(fin == NULL){
  printf("Could not open file 'data/not-there.txt'\n");
  return -1;}
while(1){
  int ret = fscanf(fin, "%c", &c);
if(ret == EOF){
     break;
}
if(c == '\n'){
    count++;
}
}
fclose(fin);
return count;
}

// PROBLEM 2: Loads a stock from file 'filename' into 'stock' filling
// its 'prices' and 'count' fields in. Makes use of the count_lines()
// function to determine how many lines are in the file which will
// dictate 'count' and the length of 'prices'. Allocates space in the
// heap for the stock's 'prices' array, opens the 'filename' and
// iterates through reading prices into the array. The data format for
// prices files is
//
// time_03 133.00
// time_04 143.00
// time_05 168.00
// time_06 91.00
// 
// where each line has a time as as single string and a price which is
// floating point number. The times can be ignored which can be
// accomplished with a fscanf() call with format "%*s" to read a
// string but ignore/discard it.
// 
// Assigns the 'datafile' field to be a duplicated string of
// 'filename' for which 'strdup()' is extremely useful. This string
// must be free()'d later likely in 'stock_free()'
// 
// On successfully loading the stock, returns 0.
//
// If 'filename' cannot be opened, prints the message 
// 
// Unable to open stock file 'some-stock.txt', bailing out
//
// with 'filename' substituted in for the name of the stock and
// returns -1.
int stock_load(stock_t *stock, char *filename){
  // WRITE ME
  FILE *file_ptr = fopen(filename, "r");
  if(file_ptr == NULL){
    printf("Could not open file 'data/not-there.txt'\n");
    printf("Unable to open stock file 'data/not-there.txt', bailing out\n");
    return -1;
  }
  int count = count_lines(filename);
  stock->count = count;
  stock->data_file = strdup(filename);
  stock->prices = malloc(count*sizeof(double));
  for(int i = 0; i < stock->count; i++){
    fscanf(file_ptr, "%*s %lf", &stock->prices[i]);  //%*s ignores the string 
  }
  fclose(file_ptr);
  return 0;
}

// PROBLEM 2: Plots a graphical representation of stock
// information. First calculates and prints plot which is in the
// following format:
//
// ==PLOT DATA==
// start/stop:  0 15
// max_height:  14
// price range: 309.00
// plot step:   22.07
//            +--B=S----------+
//     300.93 |    H   *      |
//     278.86 | *  H   *      |
//     256.79 | *  H   *      |
//     234.71 | *  H   *      |
//     212.64 |**  H   *      |
//     190.57 |**  H * *      |
//     168.50 |**  H** *  *  *|
//     146.43 |**  H** *  ****|
//     124.36 |**  H****  ****|
//     102.29 |**  H****  ****|
//      80.21 |** *H***** ****|
//      58.14 |** *H***** ****|
//      36.07 |** *H***** ****|
//      14.00 |****H*****L****|
//            +^----^----^----+
//             0    5    10   
// 
// Here brief notes on the format with more detail in the project
// specification.
// - Parameters start, stop, and max_height are printed first along with
//   calculated information like the price_range (uses hi_index and
//   lo_index)
// - The main body of the plot is exactly max_height characters high. The
//   lowest line is the price at lo_index; the highest is hi_index minus
//   plot_step
// - The vertical axis prices can be printed with the format specifier
//   %10.2f to give the correct leading whitespace with 2 digits of
//   accuracy
// - If the hi and lo prices appear in the range, their bar is printed
//   with an H or an L while all other stocks are printed with a *
// - The top axis may include a B and an S at the positions of the
//   best_buy and best_sell index with a = between these to show the
//   period of ownership.
// - The bottom axis shows tic marks as ^ and below them index labels at
//   values divisible by 5. For the numeric index labels, it is easiest
//   to print spaces to a value divisible by 5 then use the format
//   specifier %-5d to print integers: this aligns left and prints
//   whitespace padding on the right to width 5. In a loop, one can
//   advance by +5 each time a label is printed.
void stock_plot(stock_t *stock, int max_height, int start, int stop){
  // WRITE ME
  printf("==PLOT DATA==\n");
  printf("start/stop: %d %d\n", start, stop);
  printf("max_height: %d\n", max_height);
  printf("price range: %.2f\n", stock->prices[stock->hi_index] - stock->prices[stock->lo_index]);
  printf("plot step: %.2f\n", (stock->prices[stock->hi_index] - stock->prices[stock->lo_index])/max_height);
  //Prints the top horizontal axis
  printf("%10s +","");
  for(int i = start; i < stop; i++){
    
    if(stock->best_buy == i){
      printf("B");
      }
    else if(i > stock->best_buy && i < stock->best_sell){
      printf("=");
      }
    else if(stock->best_sell == i){
      printf("S");
    }
    else{
      printf("-");
    }
  }
  printf("+\n");
  //Pseudocode for the vertical axis:
//   for h=max_height-1 down to 0 do
//   thresh = h*plot_step + min_price
//   for i=start index to stop index     # note start/stop are parameters to stock_plot()
//     if stock price i >= thresh
//       print "*"
//     else
//       print " "
//   done
//   print "\n"
// done
double plot_step = ((stock->prices[stock->hi_index] - stock->prices[stock->lo_index])/max_height);
for(int h = max_height-1; h >= 0; h--){
   double thresh = (h*plot_step) + stock->prices[stock->lo_index];
   printf("%10.2f |", thresh);
   for(int i = start; i < stop; i++){
    if(stock->lo_index == i && h == 0){
      printf("L");
     }
    else if(stock->hi_index == i){
      printf("H");
    }
    else if(stock->prices[i] >= thresh){
       printf("*");
     }
     else{
       printf(" ");
     }
   }
   printf("|\n");
}
printf("           +");
for(int j = start; j <stop; j++){
  if(j % 5 == 0){
    printf("^");
  }
  else{
    printf("-");
  }
}
printf("+\n");
printf("            ");
int div = start;
while(div % 5 != 0){
  printf(" ");
  div++;
}
for(; div<stop; div += 5){
  printf("%-5d",div);
}
  return;
}

