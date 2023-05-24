#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Stores parameters that specify how to the program should behave.
 *
 * Populated by the get_parms() function.  You don't really need to
 * worry about this bit.  I wrote it for you. */
struct parms {
    char *filename;
    unsigned char mode;
};

typedef struct parms Parms;


/** U S E R   I N T E R F A C E ****************************************/


/* This function simply displays helpful usage information if the
 * program is called improperly or with no command line arguments.
 *
 * You don't need to know how this function works. */
void print_usage(const char *cmd)
{
    printf("Usage: %s MODE filename\n\n"
           "Available Modes:\n"
           "  -c     Compress: Performs RLE compression on \"filename\"\n"
           "                   and writes result to \"filename.rle\"\n\n"
           "  -x     Expand: Performs RLE expansion on \"filename\". The\n"
           "                 supplied \"filename\" must have the extension\n"
           "                 \".rle\" The result is written to \"filename\"\n"
           "                 with the extension \".rle\" removed.\n\n"
           "  -d     Debug: Prints a hexdump of \"filename\" to the screen.\n\n"
           "  -g     Generate: Writes the test file described in the Project 2\n"
           "                   assignment document to disk as \"filename\". Use\n"
           "                   this file to test and debug your program.\n\n"
           "Examples:\n"
           "  %s -c test.bin\n\tProduces RLE encoded file test.bin.rle\n"
           "  %s -x test.bin.rle\n\tExpands test.bin.rle to disk as test.bin\n"
           "  %s -d test.bin.rle\n\tDisplays raw contents of test.bin.rle\n"
           "  %s -g test.bin\n\tGenerates test file with known contents to disk as test.bin\n",
           cmd, cmd, cmd, cmd, cmd);
}


/* This function parses the command line arguments supplied in
 * argc/argv and populates the Params struct with the mode and filename
 * specified by the user on the command line.
 *
 * You don't need to know how this function works. */
int get_parms(Parms *parms, int argc, char **argv)
{
  int i = 0;
  char *modes = "cxdg";

  if (argc != 3 || *argv[1] != '-')
    return 0;

  while (modes[i] && modes[i] != *(argv[1] + 1))
    i++;

  if ( (parms->mode = i) == strlen(modes) ) {
    fprintf(stderr, "Invalid Mode %s\n", argv[1]);
    return 0;
  }

  parms->filename = argv[2];

  return 1;
}



/** H E L P E R   F U N C T I O N S ************************************/


/* Returns a newly allocated string on the heap containing the supplied
 * filename with the specified extension added to its end.  This
 * function effectively just concatenates two strings. */
char *filename_add_ext(const char *filename, const char *ext)
{
  char* new_filename;
  new_filename = malloc (strlen(filename) + strlen(ext) + 1);
  strcpy (new_filename, filename);
  strcat (new_filename, ext);
  return new_filename;
  
}


/* Returns a newly allocated string on the heap containing the supplied
 * filename with its extension removed.
 *
 * For example:
 *   if `filename` contains the string "test.txt.rle", then this
 *   function will return a string on the heap containing
 *   "test.txt" */
char *filename_rm_ext(const char *filename)
{
  char *new_filename;
  int i, count = 1;
  for (i = strlen(filename) - 1; i >= 0 ; i--){
    if (filename[i] == '.'){
      break;
    }
    count++;
  }
  if (count > strlen(filename)){
    fprintf (stderr, "No extension to remove!\n");
    return NULL;
  }
  new_filename = malloc (strlen(filename)-count+1);
  for (i = 0; i < strlen(filename)-count; i++){
    new_filename[i] = filename[i];
  }

  new_filename[strlen(filename)-count] = '\0';
  return new_filename;
}


/* This function returns zero if the supplied filename does not have the
 * extension ".rle"; otherwise it returns a non-zero value. */
int check_ext(const char *filename)
{
  char buf[5];    buf[4] = '\0';
  int i, j;
  
  for (i = strlen(filename) - 1, j = 3; j >= 0 ; i--, j--){
    buf[j] = filename[i];
  }
  
  if (!strcmp(buf, ".rle")){
    return 1;
  }
  else {
    return 0;
  }
}


/* This function returns zero if the supplied file does not have the
 * !RLE "magic" byte sequence as its first four bytes; otherwise it
 * returns a non-zero value. */
int check_magic(FILE *fp)
{
  char buf [5];  buf[4] = '\0';
  fread(buf, sizeof(char), 4, fp);
  if (!strcmp(buf, "!RLE")){
    return 1;
  }
  else{
    return 0;
  }
}



/** M O D E S **********************************************************/


/* This function performs the run length encoding (RLE) algorithm on the
 * bytes of the specified file (which must not be modified).  The result
 * is written to a new file having the supplied filename PLUS the ".rle"
 * extension.  Called when -c is supplied for mode on the command line.
 *
 *   For example, if the supplied filename is "test.bin", this function
 *   should create the file "test.bin.rle", whose contents will be the
 *   run length encoded bytes of "test.bin".
 *
 * This function is also responsible for writing the !RLE "magic" byte
 * sequence to the first four bytes of the output file, thus marking it
 * as a genuine RLE encoded file. */
void compress(const char *filename)
{
  FILE *raw, *compressed;
  char *new_filename, *magic_bytes= "!RLE";
  unsigned char cur_byte, next_byte, ret, count = 1;

  raw = fopen (filename, "rb");
    
  if (!raw){
    fprintf(stderr, "Failed to open %s\n", filename);
    return;
  }
  new_filename = filename_add_ext(filename, ".rle");
  compressed = fopen (new_filename, "wb");
  if (!compressed){
    fprintf(stderr, "Failed to create %s\n", new_filename);
    fclose(raw);
    free(new_filename);
    return;
  }
  free(new_filename);
  fwrite(magic_bytes, 1 , strlen(magic_bytes), compressed);
  
  fread(&cur_byte, 1, 1, raw);   /*Read in first byte before looping*/
  
  while (1){
    ret = fread(&next_byte, 1, 1, raw);  /*next values from what we write*/
    if (next_byte == cur_byte && count != 255 && ret){
      count ++;
      continue;
    }
    fwrite(&count, 1, 1, compressed);
    fwrite(&cur_byte, 1, 1, compressed);  /*next could be a new byte*/
    cur_byte = next_byte;  /*we have an uncounted or unwritten byte in next*/
    count = 1;
    if (!ret){
      break;
    }
  }
  fclose(compressed);
  fclose(raw);
}


/* This function performs run length decoding on the bytes of the
 * supplied filename and writes the result to disk as the contents of a
 * newly created file with the supplied filename MINUS the ".rle"
 * extension.  Called when -x is supplied for mode on the command line.
 *
 *   For example, if the supplied filename is "test.bin.rle", this
 *   function will create the file "test.bin", whose contents will be
 *   the run length decoded bytes of "test.bin".
 *
 * This function also checks that the supplied filename has both the
 * ".rle" extension and !RLE "magic" byte sequence.  If either is not
 * present, this function prints an appropriate error message to stderr,
 * does not create a new file, and simply returns without performing
 * any run length decoding. */
void expand(const char *filename)
{
  FILE *raw, *expanded;
  char *new_filename;
  char count, byte, ret;
  int i;

  raw = fopen (filename, "rb");
  if (!raw){
    fprintf (stderr, "Failed to open %s \n", filename);
    return;
  }
  if (!check_ext(filename)){
    fprintf (stderr, ".rle extension is not in %s!\n", filename);
    fclose(raw);
    return;
  }
  if (!check_magic(raw)){
    fprintf (stderr, "Magic bytes not found in %s!\n", filename);
    fclose(raw);
    return;
  }
  new_filename = filename_rm_ext(filename);
  expanded = fopen (new_filename, "wb");
  
  if (!expanded){
    fprintf (stderr, "Failed to create %s\n", new_filename);
    fclose(raw);
    free(new_filename);
    return;
  }
  free (new_filename);

  while (1) {
    ret = fread(&count, 1, 1, raw);
    if (!ret){
      break;
    }
    fread(&byte, 1, 1, raw);
    for (i = 0; i < count; i++) {
      fwrite (&byte, 1, 1, expanded);
    }
  }
  fclose (raw);
  fclose (expanded);
}


/* This function prints a raw byte dump of the specified file to the
 * screen.  Called when -d is supplied for mode on the command line.
 * This allows you to look at the raw bytes of any file you want, which
 * should very much help you debug your program.
 *
 * You don't need to know how this function works. */
void debug(const char *filename)
{
    unsigned char b[16];
    unsigned int nbytes, i;

    unsigned int offset = 0;
    FILE *fp = fopen(filename, "rb");

    if (!fp) {
        fprintf(stderr, "error -- failed to open file '%s'\n", filename);
        return;
    }

    while ( (nbytes = fread(b, sizeof(*b), 16, fp)) > 0 ) {
        printf("%08x: ", offset);
        for (i=0; i<16; i++) {
            i   < nbytes ? printf("%02x", b[i])  : printf("  ");
            ++i < nbytes ? printf("%02x ", b[i]) : printf("   ");
        }

        printf(" ");

        for (i=0; i<nbytes; i++)
            printf("%c", isprint(b[i]) ? b[i] : '.');

        printf("\n");
        offset += 16;
    }

    fclose(fp);
}


/* This function generates the test file discussed in the Project
 * description document.  Called when -g is supplied for mode on the
 * command line.  This provides a file you understand the contents of,
 * which should very much help you debug your program.
 *
 * You don't need to know how this function works. */
void generate(const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    unsigned char test[] =
        "\xee\x5d\x41\x41\x41\x8e\x2a\x3a\x3a\x3a\x3a\xe8\xbd"
        "\x22\x88\x0a\xac\xac\xac\x9a\x51\x3a\x3a\x3a\xaa\xaa"
        "\xaa\x31\x61\x27\xa7\x7e\x6d\x52\xd8\x74\x6a\x6f\x6e"
        "\x4e\xe8\x90\xa0\x7d\x88\x64\x12\x21\xdb\xae\xf7\xf7"
        "\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7"
        "\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7"
        "\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7"
        "\xf7\xf7\xf7\xf7\xf7\xaa\xaa\xbb\xbb";

    if (!fp) {
        fprintf(stderr, "error -- failed to open '%s'\n", filename);
        return;
    }

    if (1 != fwrite(test, sizeof(test) - 1, 1, fp))
        fprintf(stderr, "failed to write '%s' (oh no!)\n", filename);

    fclose(fp);
}



/** M A I N ************************************************************/


int main(int argc, char **argv)
{
    Parms parms;

    /* simply an array of function pointers */
    void (*action[])(const char *filename) = {
        compress,
        expand,
        debug,
        generate
    };

    /* get mode and filename from command line arguments;
     * prints the usage and exits if improper arguments are supplied */
    if(!get_parms(&parms, argc, argv)) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    /* call the function for the specified mode and pass
     * it the filename supplied at the command line */
    action[parms.mode](parms.filename);

    return 0;
}