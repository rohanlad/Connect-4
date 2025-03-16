//You can include any of headers defined in the C11 standard here. They are:
//assert.h, complex.h, ctype.h, errno.h, fenv.h, float.h, inttypes.h, iso646.h, limits.h, locale.h, math.h, setjmp.h, signal.h, stdalign.h, stdarg.h, stdatomic.h, stdbool.h, stddef.h, stdint.h, stdio.h, stdlib.h, stdnoreturn.h, string.h, tgmath.h, threads.h, time.h, uchar.h, wchar.h or wctype.h
//You may not include any other headers.
#include <stdio.h>
#include <stdlib.h>
#include "connect4.h"

struct board_structure
{
  char **matrix;
  int num_rows;
  int num_columns;
  int x_has_won;
  int o_has_won;
  int x_winning_coords[8];
  int o_winning_coords[8];
};

void adjust_for_wrap_around(board u)
{
  for (int k = 1; k < 8; k += 2)
  {
    if (u->x_winning_coords[k] >= u->num_columns)
    {
      u->x_winning_coords[k] = (u->x_winning_coords[k] - u->num_columns);
    }
    if (u->o_winning_coords[k] >= u->num_columns)
    {
      u->o_winning_coords[k] = (u->o_winning_coords[k] - u->num_columns);
    }
  }
}

int match_4(board u, int coords[8])
{
  for (int k = 1; k < 8; k += 2)
  {
    if (coords[k] >= u->num_columns)
    {
      coords[k] = (coords[k] - u->num_columns);
    }
  }
  return ((*(u->matrix[coords[0]] + coords[1])) == (*(u->matrix[coords[2]] + coords[3])) && (*(u->matrix[coords[2]] + coords[3])) == (*(u->matrix[coords[4]] + coords[5])) && (*(u->matrix[coords[4]] + coords[5])) == (*(u->matrix[coords[6]] + coords[7])));
}

int drop_down_tokens(board u)
{
  int made_a_change = 0;
  for (int col = 0; col < u->num_columns; col++)
  {
    int count = u->num_rows - 1;
    for (int i = (u->num_rows - 1); i > -1; i--)
    {
      if ((*(u->matrix[i] + col)) != '.')
      {
        if (count != i)
        {
          made_a_change = 1;
        }
        (*(u->matrix[count] + col)) = (*(u->matrix[i] + col));
        count--;
      }
    }
    for (int k = count; k > -1; k--)
    {
      (*(u->matrix[k] + col)) = '.';
    }
  }
  return made_a_change;
}

board setup_board()
{
  board u = (board)malloc(sizeof(struct board_structure));
  if (u == NULL)
  {
    fprintf(stderr, "Error! Memory not allocated.\n");
    exit(1);
  }
  u->matrix = NULL;
  u->num_rows = 0, u->num_columns = 0, u->x_has_won = 0, u->o_has_won = 0;
  for (int i = 0; i < 8; i++)
  {
    u->x_winning_coords[i] = 0;
    u->o_winning_coords[i] = 0;
  }
  return u;
}

void cleanup_board(board u)
{
  for (int i = 0; i < u->num_rows; i++)
  {
    free(u->matrix[i]);
  }
  free(u->matrix);
  free(u);
}

void read_in_file(FILE *infile, board u)
{
  if (infile == NULL)
  {
    fprintf(stderr, "Error: Input file not read\n");
    exit(1);
  }
  char *string = NULL;
  int index = 0, count_num_x = 0, count_num_o = 0, characterCount = 0, newlinecount = 0;
  int c = fgetc(infile);
  while (c != EOF)
  {
    index = 0;
    while (c != EOF)
    {
      if (c == '\n')
      {
        newlinecount = newlinecount + 1;
        break;
      }
      characterCount = characterCount + 1;
      if (c == 'x')
      {
        count_num_x = count_num_x + 1;
      }
      else if (c == 'o')
      {
        count_num_o = count_num_o + 1;
      }
      else if (c != '.')
      {
        fprintf(stderr, "Error: Invalid input file. The grid cannot contain any whitespaces, and can only contain '.', 'x', or 'o'.\n");
        fclose(infile);
        exit(1);
      }
      string = realloc(string, (index + 2));
      if (string == NULL)
      {
        fprintf(stderr, "Error! Memory not allocated.\n");
        fclose(infile);
        exit(1);
      }
      string[index] = c;
      index = index + 1;
      c = fgetc(infile);
    }
    if (string != NULL)
    {
      string[index] = '\0';
    }
    else
    {
      string = " ";
    }
    u->matrix = realloc(u->matrix, ((u->num_rows + 1) * sizeof(*u->matrix)));
    if (u->matrix == NULL)
    {
      fprintf(stderr, "Error! Memory not allocated.\n");
      fclose(infile);
      exit(1);
    }
    u->matrix[u->num_rows] = string;
    u->num_rows = u->num_rows + 1;
    string = NULL;
    c = fgetc(infile);
  }
  u->num_columns = index;
  if (characterCount != (u->num_rows * u->num_columns) || newlinecount != u->num_rows)
  {
    fprintf(stderr, "Error: Invalid input file. A valid input file must contain a consistent grid, terminate with a single newline character, and must not contain any whitespaces.\n");
    fclose(infile);
    exit(1);
  }
  if (u->num_columns > 512 || u->num_columns < 4)
  {
    fprintf(stderr, "Error: Invalid input file. The number of columns must be at least 4 and at most 512.\n");
    fclose(infile);
    exit(1);
  }
  if (u->num_rows < 4)
  {
    fprintf(stderr, "Error: Invalid input file. The number of rows must be at least 4.\n");
    fclose(infile);
    exit(1);
  }
  if ((count_num_x - count_num_o) != 0 && (count_num_x - count_num_o) != 1)
  {
    fprintf(stderr, "Error: Invalid input file. There are %d 'x' tokens and %d 'o' tokens; this is incorrect.\n", count_num_x, count_num_o);
    fclose(infile);
    exit(1);
  }
  if (drop_down_tokens(u))
  {
    fprintf(stderr, "Error: Invalid input file. The tokens should have dropped to the bottom.\n");
    fclose(infile);
    exit(1);
  }
}

void write_out_file(FILE *outfile, board u)
{
  if (outfile == NULL)
  {
    fprintf(stderr, "Error: Cannot open output file\n");
    exit(1);
  }
  current_winner(u);
  if (u->x_has_won)
  {
    adjust_for_wrap_around(u);
    for (int i = 0; i < 8; i += 2)
    {
      (*(u->matrix[u->x_winning_coords[i]] + u->x_winning_coords[i + 1])) = 'X';
    }
  }
  if (u->o_has_won)
  {
    adjust_for_wrap_around(u);
    for (int i = 0; i < 8; i += 2)
    {
      (*(u->matrix[u->o_winning_coords[i]] + u->o_winning_coords[i + 1])) = 'O';
    }
  }
  for (int j = 0; j < u->num_rows; j++)
  {
    fprintf(outfile, "%s \n", u->matrix[j]);
  }
}

char next_player(board u)
{
  int count_x = 0;
  int count_o = 0;
  for (int i = 0; i < u->num_rows; i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if ((*(u->matrix[i] + j)) == 'x')
      {
        count_x = count_x + 1;
      }
      else if (((*(u->matrix[i] + j)) == 'o'))
      {
        count_o = count_o + 1;
      }
    }
  }
  if (count_x > count_o)
  {
    return 'o';
  }
  else
  {
    return 'x';
  }
}

char current_winner(board u)
{
  // HORIZONTAL CHECK
  for (int i = 0; i < u->num_rows; i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if ((*(u->matrix[i] + j)) == 'x')
      {
        int coords[8] = {i, j, i, (j + 1), i, (j + 2), i, (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->x_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->x_winning_coords[k] = coords[k];
            }
          }
          u->x_has_won = 1;
        }
      }
      else if ((*(u->matrix[i] + j)) == 'o')
      {
        int coords[8] = {i, j, i, (j + 1), i, (j + 2), i, (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->o_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->o_winning_coords[k] = coords[k];
            }
          }
          u->o_has_won = 1;
        }
      }
    }
  }
  //VERTICAL CHECK
  for (int i = 0; i < (u->num_rows - 3); i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if ((*(u->matrix[i] + j)) == 'x')
      {
        int coords[8] = {i, j, (i + 1), j, (i + 2), j, (i + 3), j};
        if (match_4(u, coords))
        {
          if (!(u->x_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->x_winning_coords[k] = coords[k];
            }
          }
          u->x_has_won = 1;
        }
      }
      else if ((*(u->matrix[i] + j)) == 'o')
      {
        int coords[8] = {i, j, (i + 1), j, (i + 2), j, (i + 3), j};
        if (match_4(u, coords))
        {
          if (!(u->o_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->o_winning_coords[k] = coords[k];
            }
          }
          u->o_has_won = 1;
        }
      }
    }
  }
  // DIAGONAL UPWARDS (LEFT TO RIGHT) CHECK
  for (int i = 3; i < u->num_rows; i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if ((*(u->matrix[i] + j)) == 'x')
      {
        int coords[8] = {i, j, (i - 1), (j + 1), (i - 2), (j + 2), (i - 3), (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->x_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->x_winning_coords[k] = coords[k];
            }
          }
          u->x_has_won = 1;
        }
      }
      else if ((*(u->matrix[i] + j)) == 'o')
      {
        int coords[8] = {i, j, (i - 1), (j + 1), (i - 2), (j + 2), (i - 3), (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->o_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->o_winning_coords[k] = coords[k];
            }
          }
          u->o_has_won = 1;
        }
      }
    }
  }
  // DIAGONAL DOWNWARDS (LEFT TO RIGHT) CHECK
  for (int i = 0; i < (u->num_rows - 3); i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if ((*(u->matrix[i] + j)) == 'x')
      {
        int coords[8] = {i, j, (i + 1), (j + 1), (i + 2), (j + 2), (i + 3), (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->x_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->x_winning_coords[k] = coords[k];
            }
          }
          u->x_has_won = 1;
        }
      }
      else if ((*(u->matrix[i] + j)) == 'o')
      {
        int coords[8] = {i, j, (i + 1), (j + 1), (i + 2), (j + 2), (i + 3), (j + 3)};
        if (match_4(u, coords))
        {
          if (!(u->o_has_won))
          {
            for (int k = 0; k < 8; k++)
            {
              u->o_winning_coords[k] = coords[k];
            }
          }
          u->o_has_won = 1;
        }
      }
    }
  }
  if (u->x_has_won && u->o_has_won)
  {
    return 'd';
  }
  else if (u->x_has_won)
  {
    return 'x';
  }
  else if (u->o_has_won)
  {
    return 'o';
  }
  else
  {
    for (int i = 0; i < u->num_rows; i++)
    {
      for (int j = 0; j < u->num_columns; j++)
      {
        if ((*(u->matrix[i] + j)) == '.')
        {
          return '.';
        }
      }
    }
    return 'd';
  }
}

struct move read_in_move(board u)
{
  struct move m;
  printf("Player %c enter column to place your token: ", next_player(u));
  if (scanf("%d", &m.column) != 1)
  {
    fprintf(stderr, "Error: Invalid response.\n");
    exit(1);
  };
  printf("Player %c enter row to rotate: ", next_player(u));
  if (scanf("%d", &m.row) != 1)
  {
    fprintf(stderr, "Error: Invalid response.\n");
    exit(1);
  };
  return m;
}

int is_valid_move(struct move m, board u)
{
  if ((m.column > 0) && (m.column <= u->num_columns) && ((*(u->matrix[0] + (m.column - 1))) == '.') && (m.row >= (u->num_rows * -1)) && (m.row <= u->num_rows))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

char is_winning_move(struct move m, board u)
{
  char **store_old_board = calloc(u->num_rows, sizeof(char *));
  if (store_old_board == NULL)
  {
    fprintf(stderr, "Error! Memory not allocated.\n");
    exit(1);
  }
  for (int i = 0; i < u->num_rows; i++)
  {
    store_old_board[i] = malloc(u->num_columns * sizeof(char));
    if (store_old_board[i] == NULL)
    {
      fprintf(stderr, "Error! Memory not allocated.\n");
      exit(1);
    }
    for (int j = 0; j < u->num_columns; j++)
    {
      (*(store_old_board[i] + j)) = (*(u->matrix[i] + j));
    }
  }
  play_move(m, u);
  char winner = current_winner(u);
  for (int i = 0; i < u->num_rows; i++)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      (*(u->matrix[i] + j)) = (*(store_old_board[i] + j));
    }
  }
  u->x_has_won = 0;
  u->o_has_won = 0;
  for (int k = 0; k < u->num_rows; k++)
  {
    free(store_old_board[k]);
  }
  free(store_old_board);
  return winner;
}

void play_move(struct move m, board u)
{
  // Place token into board
  for (int i = (u->num_rows - 1); i > -1; i--)
  {
    if ((*(u->matrix[i] + (m.column - 1))) == '.')
    {
      (*(u->matrix[i] + (m.column - 1))) = next_player(u);
      break;
    }
  }
  //Rotate board (if desired)
  char hold_value;
  if (m.row < 0)
  {
    for (int j = 0; j < u->num_columns; j++)
    {
      if (j == 0)
      {
        hold_value = (*(u->matrix[u->num_rows + m.row] + j));
        (*(u->matrix[u->num_rows + m.row] + j)) = (*(u->matrix[u->num_rows + m.row] + j + 1));
      }
      else if (j == (u->num_columns - 1))
      {
        (*(u->matrix[u->num_rows + m.row] + j)) = hold_value;
      }
      else
      {
        (*(u->matrix[u->num_rows + m.row] + j)) = (*(u->matrix[u->num_rows + m.row] + j + 1));
      }
    }
  }
  else if (m.row > 0)
  {
    for (int j = (u->num_columns - 1); j > -1; j--)
    {
      if (j == (u->num_columns - 1))
      {
        hold_value = (*(u->matrix[u->num_rows - m.row] + j));
        (*(u->matrix[u->num_rows - m.row] + j)) = (*(u->matrix[u->num_rows - m.row] + j - 1));
      }
      else if (j == 0)
      {
        (*(u->matrix[u->num_rows - m.row] + j)) = hold_value;
      }
      else
      {
        (*(u->matrix[u->num_rows - m.row] + j)) = (*(u->matrix[u->num_rows - m.row] + j - 1));
      }
    }
  }
  drop_down_tokens(u);
}