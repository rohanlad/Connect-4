# Connect 4
My first C program - an implementation of the Connect 4 game.

I have chosen to store the grid using a ‘2D array’ - essentially a pointer to a pointer to a character. Each token/position on the grid is a character, and I am considering the grid as an array of rows, so each row is considered an array of characters, and my grid ```**matrix``` is a pointer to this array of arrays of characters i.e like an array of strings where each string is a row. I chose this data structure because using simple pointer arithmetic, it is very efficient for selecting a particular character in the grid. ```*(u->matrix[i] + j)``` allows me to access the ```j```th character on the ```i```th row.

It was crucial that I was able to validate the input file in my ```read_in_file``` function, because my code expects the format of the input to be in a very specific format, so any input files which do not adhere to this format (i.e invalid input files) would break the code somewhere down the line. So several checks are made in this function. Firstly, we check if infile is ```NULL``` (although as explained later I would’ve liked this to check to be made in ```main.c```). If it’s not ```NULL```, we proceed. We start reading in the characters of the input file. If at any point we come across an unexpected character, we exit the program. Using calculations between various ‘counter’ variables, we can identify if the input is valid i.e ensure there are a consistent number of rows/columns of equal size, check for any unexpected/unnecessary whitespaces or blank lines, and ensure the input terminates with a single newline character. If all these checks pass, we then check the following: the number of columns is between 4 and 512, the number of rows is at least 4, there are always either 1 or 0 more ‘```x```’ tokens than ‘```o```’ tokens on the grid, and the tokens have already ‘dropped down’ due to gravity so that the current board position is valid. We exit the program unless all these checks have passed. That guarantees to the rest of our code that the board will be of expected format.

In my ```write_out_file``` function I check that outfile is not ```NULL``` before attempting to use it to write to a file. In my ```read_in_move``` function, I handle the user input like so: ```if (scanf("%d",&m.column) != 1)``` and then exit the program if this check is satisfied. This ensures that the user actually inputs 2 integers, which guarantees the later code will not fail as it is expecting the
move to consist of 2 integers. Throughout the code, whenever memory is dynamically allocated, I always immediately then check if the corresponding pointer isn’t ```NULL```, to ensure that the memory was successfully allocated.

When I initially store memory for the board, I incrementally dynamically allocate only enough memory that is required at the time in the code. So I allocate just enough memory to store an additional character in each row (using ```realloc```), each time I come across a valid character in the input file, and once the row is complete and valid, I then allocate enough memory to store just one new row in the board (using ```realloc```). This approach is more memory-efficient because it ensures I never allocate more memory than what I need. I always ensure that any dynamically allocated memory is freed. This is done primarily in the
```cleanup_board``` function, where I first iterate through board ```u``` to free the memory that each character pointer points to (i.e clear each ‘row’). After that I clear board ```u``` itself, ultimately meaning that I have cleared all the memory allocated both that ```u``` points at and within ```u```. In the ```is_winning_move``` function I do a similar thing for the variable ```store_old_board``` which is initialised in this function. I free all the memory as soon as I am done with this variable, so before I even return from the function, as opposed to within the ```cleanup_board``` function which is called at the end of the program. This ensures I am more memory efficient because I free memory as soon as I’m done with it. Throughout the code, whenever I exit the program due to an error, if there is an existing file that I have opened, I always close it using ```fclose``` before I exit the program, to be efficient with our resources.

There are a few ways in which the robustness of ```main.c``` could be improved. Firstly, on line 7, I
have the line infile=fopen("initial
board.txt"
,
_
"r"); This needs to be error handled because I
immediately start using the infile variable in the line after. Infile would be NULL, if, say,
initial
_
board.txt didn’t exist in the directory, which could cause problems down the line in our
code. So I could implement an IF statement straight after this line which checks whether infille
is NULL - if it is NULL, I should exit the program accordingly. Instead, as a workaround, I have
had to implement this check within the function read
in
_
_
file in my connect4.c file just to make
sure I still retain the robustness, but in an efficient program, if infile was NULL the code should
terminate there and then and shouldn’t be calling read
in
_
_
file in the first place. Similarly, on line
21 I have outfile=fopen("final
board.txt"
,
_
"w"); and then the line after I use the variable
outfile. I need to error handle the outfile variable in similar fashion. Again, as a (not ideal)
workaround I have had to implement this error handling directly within my write
out
file function
_
_
in connect4.c.
On line 14, I read in a move from the user. On line 15 I then pass in this move to another
function which checks whether or not the move is valid, and decides what action to take
thereafter. However, there is a problem here: there are 2 possible ‘layers’ of what an invalid
move could mean in this context. The user could enter 2 valid integers, which simply amount to
an invalid move on the board, or the user could enter something entirely different, such as 2
letters, or even a malicious/dangerous input. Currently the code is designed to read in the user’s
input on line 14, and then pass this input, regardless of what it is, into the validate function on
line 15 to check whether or not it is a valid move. But what if the user input was of malicious
nature? I certainly would not want to store that in memory and pass it into the validate
function, I would want to reject that from the read
in
move function as soon as I read it.
_
_
This is exactly what I’ve implemented in my read
in
_
_
move function; I have implemented checks
whereby if the user does not input 2 integers, the program is exited. I leave the validate function
to check if these 2 integers correspond to a valid move. But because of how main.c is designed,
this incorporates another issue. Let’s say the user misclicks and accidentally enters a
non-integer as their move. The program will just exit and terminate the entire game, which is
poor user experience (if say it was just a misclick for example). Instead this should really be
handled by main.c which can identify the input was invalid, and ask for the input again. This is
all done before I actually validate the move. This way I can just return something appropriate
from my read
in
_
_
move function instead of exiting the program.
To make connect4.h more robust, I could implement #include guards i.e put the entirety of its
contents inside a #ifndef #define … #endif clause. This should make it absolutely certain that I
don’t include the header file multiple times, so as to prevent multiple declarations of any
identifiers.
