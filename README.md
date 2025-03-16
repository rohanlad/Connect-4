# Connect 4
My first C program - an implementation of the Connect 4 game.

I have chosen to store the grid using a ‘2D array’ - essentially a pointer to a pointer to a character. Each token/position on the grid is a character; consider the grid as an array of rows, where each row is considered an array of characters, and the grid ```**matrix``` is a pointer to this array of arrays of characters i.e like an array of strings where each string is a row. I chose this data structure because using simple pointer arithmetic, it is very efficient for selecting a particular character in the grid. ```*(u->matrix[i] + j)``` allows access the ```j```th character on the ```i```th row.

It was crucial that I was able to validate the input file in my ```read_in_file``` function, because the code expects the format of the input to be in a specific format, so any input files which do not adhere to this format (i.e invalid input files) would break the code somewhere down the line. So several checks are made in this function. Firstly, it checks if infile is ```NULL``` (although as explained later I would’ve liked this to check to be made in ```main.c```). If it’s not ```NULL```, it proceeds. It starts reading in the characters of the input file. If at any point it comes across an unexpected character, the program is exited. Using calculations between various ‘counter’ variables, it can identify if the input is valid i.e ensure there are a consistent number of rows/columns of equal size, check for any unexpected/unnecessary whitespaces or blank lines, and ensure the input terminates with a single newline character. If all these checks pass, it then checks the following: the number of columns is between 4 and 512, the number of rows is at least 4, there are always either 1 or 0 more ‘```x```’ tokens than ‘```o```’ tokens on the grid, and the tokens have already ‘dropped down’ due to gravity so that the current board position is valid. The program exits unless all these checks have passed. That gives the rest of the program the guarantee that the board will be of expected format.

The ```write_out_file``` function checks that outfile is not ```NULL``` before attempting to use it to write to a file. The ```read_in_move``` function handles the user input like so: ```if (scanf("%d",&m.column) != 1)``` and then exits the program if this check is satisfied. This ensures that the user actually inputs 2 integers, which guarantees the later code will not error as it is expecting the
move to consist of 2 integers. Throughout the program, whenever memory is dynamically allocated, we always immediately then check if the corresponding pointer isn’t ```NULL```, to ensure that the memory was successfully allocated.

When memory for the board is initially stored, we incrementally dynamically allocate only enough memory that is required at the time in the code. So just enough memory is allocated to store an additional character in each row (using ```realloc```), each time a valid character in the input file is met, and once the row is complete and valid, enough memory is allocated to store just one new row in the board (using ```realloc```). This approach is more memory-efficient because it ensures we never allocate more memory than what we need. We always ensure that any dynamically allocated memory is freed. This is done primarily in the
```cleanup_board``` function, which first iterates through board ```u``` to free the memory that each character pointer points to (i.e clear each ‘row’). After that, board ```u``` itself is cleared, ultimately meaning that all the memory allocated both that ```u``` points at and within ```u``` is cleared. In the ```is_winning_move``` function, a similar thing is done for the variable ```store_old_board``` which is initialised in this function. All the memory is freed as soon as we are done with this variable, so before even returning from the function, as opposed to within the ```cleanup_board``` function which is called at the end of the program. This underlines my memory efficient approach - because I free memory as soon as I’m done with it. Throughout the code, whenever the program exits due to an error, if there is an existing file that has been opened, I always close it using ```fclose``` before the program exits, to be efficient with our resources.

There are a few ways in which the robustness of ```main.c``` could be improved. Firstly, on line 7, the line ```infile=fopen("initial_board.txt","r")```; this needs to be appropriately error handled because the ```infile``` variable begins to be used immediately in the line after. ```infile``` would be ```NULL```, if, say, ```initial_board.txt``` didn’t exist in the directory, which could cause problems later down the line in the program. So a check could be implemented straight after this line that checks whether ```infile``` is ```NULL``` - if it is ```NULL```,the program could terminate accordingly. Instead, as a workaround, I have had to implement this check within the function ```read_in_file``` in my ```connect4.c``` file just to make sure I still retain the robustness, but in an efficient program, if ```infile``` was ```NULL``` the code should terminate there and then and shouldn’t be calling ```read_in_file``` in the first place. Similarly, on line
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
