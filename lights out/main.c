#include <unistd.h>   /* both of these are  */
#include <termios.h>  /* for get_keypress() */

#include <stdio.h>

/* Useful Constants:
 *   -- The house is 69 characters wide
 *   -- The house is 26 characters high
 *   -- Each window interior is 11 characters wide
 *   -- Each window interior is 3 character high
 */
#define HOUSE_WIDTH   69
#define HOUSE_HEIGHT  26
#define WINDOW_WIDTH  11
#define WINDOW_HEIGHT  3
#define WINDOW_NUMBERS 9


/* Reads a keypress from the keyboard and returns the
 * corresponding character.
 *
 *   This is necessary because scanf() misbehaves inside of looping
 *   constructs.  You do not need to understand how this function works.
 *   Just use this function as a tool.
 *
 * Return Value:
 *   The character corresponding to the key pressed on the keyboard.
 */
static char get_keypress ()
{
    struct termios t;
    char ret;

    static int init = 0;

    if (!init) {
        init = 1;
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }

    ret = (char)getchar();
    printf("\n");

    return ret;
}


/* Updates the graphics for the window @ coordinates (x, y) to match the
 * `state` array.
 *
 *   This function modifies the `house` array by updating the characters
 *   inside the window located at the zero indexed coordinates (x, y) to
 *   match the window's state in the `state` array.  If the window's
 *   state is 1, then the window is filled with the '#' character.
 *   Likewise, if the window's state is 0 in the `state` array, the
 *   window is filled with the ' ' character.
 *
 * Parameters:
 *   house -- pointer to characters representing the house
 *
 *   state -- pointer to the game state array
 *
 *       x -- the horizontal coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the left column and 2 being the right column)
 *
 *       y -- the vertical coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the top row and 2 being the bottom row)
 */
static void window_update_graphics (char *house, const int *state, int x, int y)
{
  /* Step 1: Write this function! */
  int i, j;
  int row_start = 8, ud_diff = 6, lr_diff = 7;
  
  for (i= 0; i < WINDOW_HEIGHT; i++)
  {
    for (j = 0; j < WINDOW_WIDTH ; j++)
    {
      if (state[3*y+x] == 1)
        house[HOUSE_WIDTH*(row_start + ud_diff*y + i) + WINDOW_WIDTH*(x + 1) + lr_diff*x + j] = '#';
      else
        house[HOUSE_WIDTH*(row_start + ud_diff*y + i) + WINDOW_WIDTH*(x + 1) + lr_diff*x + j] = ' ';
    }
  }
}


/* Toggles the state of the window @ coordinates (x, y) in the game
 * state array.
 *
 *   This function modifies the `state` array by toggling the value
 *   corresponding to the window at the provided coordinates (x, y).  If
 *   the current value is 1, then it will be toggled to 0.  Likewise, if
 *   the current value is 0, then it will be toggled to 1.
 *
 * Parameters:
 *   state -- pointer to the game state array
 *
 *       x -- the horizontal coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the left column and 2 being the right column)
 *
 *       y -- the vertical coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the top row and 2 being the bottom row)
 */
static void window_toggle_state (int *state, int x, int y)
{
  int idx = 3*y + x;

  if (state[idx] == 0)
    state [idx] = 1;
  else
    state [idx] = 0;
}


/* Toggles the state and, correspondingly, updates the graphics for the
 * window @ coordinates (x, y).
 *
 *   Given the zero indexed coordinates (x, y) of a window, this
 *   function ensures that the corresponding element in the game state
 *   array is toggled appropriately and that the graphics corresponding
 *   to that window are updated appropriately to reflect the new state.
 *
 *   Hint: call other functions you have written to make this easier.
 *
 * Parameters:
 *   state -- pointer to the game state array
 *
 *   house -- pointer to characters representing the house
 *
 *       x -- the horizontal coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the left column and 2 being the right column)
 *
 *       y -- the vertical coordinate of the window for which the
 *            state will be updated (zero indexed, with 0 being
 *            the top row and 2 being the bottom row)
 */
static void window_update (int *state, char *house, int x, int y)
{
  window_toggle_state (state, x, y);
  window_update_graphics (house, state, x, y);
}


/* Toggles the lights based on the user's window selection.
 *
 *   Given the user's choice of window number, this function updates the
 *   corresponding window's (or windows') state and graphics, as
 *   necessary.
 *
 *   Hints:
 *     -- A primary job of this function will be to convert the choice
 *        of the user into the zero indexed (x, y) coordinates of a
 *        window.
 *
 *     -- Converting between `choice` and the (x, y) coordinates will
 *        probably be easier if you internally number the windows
 *        starting at zero (i.e. 0 thru 8 instead of 1 thru 9).
 *
 *     -- This function will need to call one other function
 *        (perhaps multiple times).
 *
 *   While completing Step 2, this function will only toggle the window
 *   corresponding to the user's selection.  For Step 3, this function
 *   will toggle the selected window and its neighbors (as described
 *   above in the instructions).
 *
 * Parameters:
 *    state -- pointer to the game state array
 *
 *    house -- pointer to characters representing the house
 *
 *   choice -- integer corresponding to the user's window selection
 *             (an integer value within the range 1 to 9)
 */
static void window_toggle (int *state, char *house, int choice)
{
  int x = (choice - 1) % 3;
  int y = (choice - 1) / 3;

  if (x == 0)
    window_update (state, house, x +1 , y);
  else if (x ==1)
  {
    window_update (state, house, x + 1, y);
    window_update (state, house, x - 1, y);
  }
  else
    window_update (state, house, x - 1, y);

  if (y == 0)
    window_update (state, house, x, y + 1);
  else if (y ==1)
  {
    window_update (state, house, x, y + 1);
    window_update (state, house, x, y - 1);
  }
  else
    window_update (state, house, x, y - 1);

  window_update (state, house, x, y);
}


/* Checks if all lights have been turned off.
 *
 *   Iterates through the game state array and returns 1 if all of the
 *   lights have been successfully turned off; otherwise returns 0.
 *
 * Parameters:
 *   state -- pointer to the game state array
 *
 * Return Values:
 *   1 if all lights have been turned off; 0 otherwise.
 */
static int solved (const int *state)
{
  int i;
  for (i = 0; i < WINDOW_NUMBERS; i++)
  {
    if (state[i])
      return 0;
  }
  return 1;
}


/* Prints the characters in the house array to the screen
 *
 * Parameters:
 *   house -- pointer characters representing the house
 */
static void house_display (const char *house)
{
    int x, y;

    for (y=0; y<HOUSE_HEIGHT; y++, printf("\n"))
        for (x=0; x<HOUSE_WIDTH; x++)
            printf ("%c", house[HOUSE_WIDTH * y + x]);
}


/* Initializes the graphics for each of the 9 windows.
 *
 *   Cycles through the 9 windows and fills each window in the `house`
 *   array of chars with either ' ' or '#' characters depending on the
 *   current state of the window in the state array by calling
 *   `window_update_graphics()` for each window individually.
 *
 * Parameters:
 *   house -- pointer to characters representing the house
 *
 *   state -- pointer to the game state array
 */
static void house_init (char *house, const int *state)
{
    int x, y;

    for (y=0; y<3; y++)
        for (x=0; x<3; x++)
            window_update_graphics (house, state, x, y);
}



int main ()
{
    char choice;
    char house[] =
    "                                             ______________          "
    "                                            |______________|         "
    "      _______________________________________|            |_____     "
    "     '                                       |____________|     `    "
    "    |                                                           |    "
    "    '-----------------------------------------------------------'    "
    "    |           1                 2                 3           |    "
    "    |     +-----------+     +-----------+     +-----------+     |    "
    "    |     |           |     |           |     |           |     |    "
    "    |     |           |     |           |     |           |     |    "
    "    |     |           |     |           |     |           |     |    "
    "    |     +-----------+     +-----------+     +-----------+     |    "
    "    |           4                 5                 6           |    "
    "    |     +-----------+     +-----------+     +-----------+     |    "
    "    |     |           |     |           |     |           |     |    "
    "    |     |           |     |           |     |           |     |    "
    "  _ |     |           |     |           |     |           |     |    "
    " |#||     +-----------+     +-----------+     +-----------+     |    "
    " |_||           7                 8                 9           |    "
    "  `-|     +-----------+     +-----------+     +-----------+     |    "
    "    -     |           |     |           |     |           |     |    "
    "    '     |           |     |           |     |           |     |    "
    "    '     |           |     |           |     |           |     |    "
    "   o'     +-----------+     +-----------+     +-----------+     |    "
    "    '                                                           |    "
    "____'___________________________________________________________'____";

    
    /* This will be our initial game state (do not change this!)
     *   [ 1 = Light is ON,  0 = Light is OFF ] */
    int state[] = { 1, 1, 0,
                    1, 1, 0,
                    1, 0, 0 };


    house_init (house, state);

    /* The Game Loop */
    while (1) {
        house_display (house);

        if (solved (state)) {
            printf ("Congratulations!  You won!\n");
            break;
        }

        printf ("Choose a Window (0 to exit): ");
        choice = get_keypress();

        if (choice < '0' || choice > '9') {
            printf ("INVALID SELECTION!\n");
            continue;
        }

        if (choice == '0')
            break;

        /* convert numerical character to corresponding value
           (check the ASCII table if this doesn't make sense) */
        choice -= '0';

        /* update game state & graphics */
        window_toggle (state, house, choice);
    }

    printf ("Goodbye!\n");

    return 0;
}
