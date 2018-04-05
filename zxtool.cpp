/* Original code by

Jack Raats
Noorddonk 107
4651 ZD Steenbergen
01670 - 65298
Email:  2:285/751@fidonet.org
27:1331/304@SIGnet.org

30 december 1992     DeSmet C / C68 v3.05
*/

/* Modded out of recognition by morris
*/

#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include "argcrack.h"

// using gfx character representations compatible with ZXTEXT2P by chris cowley
// http://freestuff.grok.co.uk/zxtext2p/index.html
//
const char *tabel[] =
    {
        " ", "\\' ", "\\ '", "\\''", "\\. ", "\\: ", "\\.'", "\\:'",
        "\\##", "\\,,", "\\~~", "\"", "#", "$", ":", "?",
        "(", ")", ">", "<", "=", "+", "-", "*",
        "/", ";", ",", ".", "0", "1", "2", "3",
        "4", "5", "6", "7", "8", "9", "A", "B",
        "C", "D", "E", "F", "G", "H", "I", "J",
        "K", "L", "M", "N", "O", "P", "Q", "R",
        "S", "T", "U", "V", "W", "X", "Y", "Z",
        "RND", "INKEY$", "PI", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "?", "?", "?", "?", "?", "?", "?", "?",
        "\\::", "\\.:", "\\:.", "\\..", "\\':", "\\ :", "\\'.", "\\ .",
        "\\@@", "\\;;", "\\!!", "%\"", "%�", "%$", "%:", "%?",
        "%(", "%)", "%>", "%<", "%=", "%+", "%-", "%*",
        "%/", "%;", "%,", "%.", "%0", "%1", "%2", "%3",
        "%4", "%5", "%6", "%7", "%8", "%9", "%A", "%B",
        "%C", "%D", "%E", "%F", "%G", "%H", "%I", "%J",
        "%K", "%L", "%M", "%N", "%O", "%P", "%Q", "%R",
        "%S", "%T", "%U", "%V", "%W", "%X", "%Y", "%Z",
        "\"\"", "AT ", "TAB ", "?", "CODE ", "VAL ", "LEN ", "SIN ",
        "COS ", "TAN ", "ASN ", "ACS ", "ATN ", "LN ", "EXP ", "INT ",
        "SQR ", "SGN ", "ABS ", "PEEK ", "USR ", "STR$ ", "CHR$ ", "NOT ",
        "**", " OR ", " AND ", "<=", ">=", "<>", " THEN ", " TO ",
        " STEP ", "LPRINT ", "LLIST ", "STOP ", "SLOW ", "FAST ", "NEW ", "SCROLL ",
        "CONT ", "DIM ", "REM ", "FOR ", "GOTO ", "GOSUB ", "INPUT ", "LOAD ",
        "LIST ", "LET ", "PAUSE ", "NEXT ", "POKE ", "PRINT ", "PLOT ", "RUN ",
        "SAVE ", "RAND ", "IF ", "CLS ", "UNPLOT ", "CLEAR ", "RETURN ", "COPY "
};

static const int D_FILE = 0x400c;
static const int DF_CC = 0x400e;
static const int VARS = 0x4010;
static const int E_LINE = 0x4014;
static const int CH_ADD = 0x4016;
static const int STKBOT = 0x401a;
static const int STKEND = 0x401c;
static const int NXTLIN = 0x4029;

static const int FPMARKER = 0x7e;
static const int REMTOKEN = 0xea;
static const int NEWLINE = 0x76;

typedef struct
{
      int nBytes;
      const char *name;
      const char *description;
} sysvar;

sysvar sysvars[] =
    {
        1, "VERSN", "0 identifies 8K ROM in saved programs.",
        2, "E_PPC", "Number of current line (with program cursor).",
        2, "D_FILE", "Address of display file.",
        2, "DF_CC", "Address of PRINT position in display file.",
        2, "VARS", "Address of variables in memory. Usually after D_FILE.",
        2, "DEST", "Address of variable in assignment.",
        2, "E_LINE", "Address of line being edited in memory.",
        2, "CH_ADD", "Address of the next character to be interpreted.",
        2, " X_PTR", "Address of the character preceding the marker.",
        2, "STKBOT", "Address of the Calculator stack in memory.",
        2, "STKEND", "End of the Calculator stack.",
        1, "BREG", "Calculator's b register.",
        2, "MEM", "Address of area used for calculator's memory.",
        1, "unused", "Not used.",
        1, "DF_SZ", "The number of lines in the lower part of the screen.",
        2, "S_TOP", "The top program line number in automatic listings.",
        2, "LAST_K", "Shows which keys pressed.",
        1, "DBOUNCE", "Debounce status of keyboard.",
        1, "MARGIN", "Number of blank lines above or below picture - 31.",
        2, "NXTLIN", "Address of next program line to be executed.",
        2, "OLDPPC", "Line number to which CONT jumps.",
        1, "FLAGX", "Various flags.",
        2, "STRLEN", "Length of string type designation in assignment.",
        2, "T-ADDR", "Address of next item in syntax table.",
        2, "SEED", "The seed for RND, the variable that is set by RAND.",
        2, "FRAMES", "Counts the frames displayed on the television.",
        1, "COORDSX", "x-coordinate of last pointed PLOTted.",
        1, "COORDSY", "y-coordinate of last pointed PLOTted.",
        1, "PR_CC", "LSB of address for LPRINT to print at (in PRBUFF).",
        1, "S_POSNX", "Column number for PRINT position.",
        1, "S_POSNY", "Line number for PRINT position.",
        1, "CDFLAG", "Bit 7 is on (1) during compute and display mode.",
        33, "PRBUFF", "Printer buffer (33rd character is ENTER).",
        30, "MEMBOT", "Calculator's memory area.",
        2, "unused", "Not used."};

typedef std::vector<unsigned char> bytevec;

int peek(const bytevec &storage, int positie)
{
      return storage[positie - 0x4009];
}

void poke(bytevec &storage, int positie, unsigned char val)
{
      storage[positie - 0x4009] = val;
}

int dpeek(const bytevec &storage, int positie)
{
      positie -= 0x4009;
      int a = storage[positie];
      int b = storage[positie + 1];

      return a + 256 * b;
}

void dpoke(bytevec &storage, int positie, int value)
{
      positie -= 0x4009;
      storage[positie] = value & 0xff;
      storage[positie + 1] = value / 256;
}

bool getlineinfo(bytevec &storage, int &pc, int &linenumber, int &linelength)
{
      linenumber = 256 * peek(storage, pc) + peek(storage, pc + 1);
      linelength = dpeek(storage, pc + 2);
      pc += 4;

      return true;
}

// not really needed as we can use the size of the contining vector.
// i'll leave it here for documentation purposes ;)
//
int pfilesize(const bytevec &storage)
{
      return dpeek(storage, STKEND) - 0x4009;
}

int read(bytevec &storage, const char *name)
{
      size_t length = 0;

      FILE *fi = fopen(name, "rb");
      if (fi != NULL)
      {
            fseek(fi, 0, SEEK_END);
            length = ftell(fi);
            rewind(fi);

            storage.resize(length);

            length = fread(&storage[0], 1, length, fi);
            fclose(fi);
      }

      return length;
}

bool write(const char *name, void *addr, int length)
{
      int written = 0;

      FILE *fo = fopen(name, "wb");
      if (fo != NULL)
      {
            written = fwrite(addr, 1, length, fo);
            fclose(fo);
      }

      return written == length;
}

bool writevec(const char *name, const bytevec &storage)
{
      return write(name, (void *)(&storage[0]), storage.size());
}

double getFP(const bytevec &storage, int offset)
{
      unsigned char *buffer = (unsigned char *)&storage[offset - 0x4009];

      // comment in zx81 rom disassemby:
      //   ; If the exponent is zero then the number is zero and an early return is made.
      //
      if (buffer[0] == 0)
      {
            return 0;
      }
      else
      {
            double exponent = buffer[0] - 128;
            double sign = (buffer[1] >= 0x80) ? -1 : 1;
            double mantissa = ((buffer[1] | 0x80) / 256.0) + (buffer[2] / 65536.0) + (buffer[3] / 16777216.0) + (buffer[4] / 4294967296.0);
            return sign * mantissa * pow(2.0, exponent);
      }
}

void printFP(double fp)
{
      // print as an int if there's no fractional part.
      //
      if ((fp - int(fp)) == 0)
      {
            printf("%d", int(fp));
      }
      else
      {
            printf("%f", fp);
      }
}

void recurse_string(bytevec &storage, int &pc, const int depth, const std::vector<int> &dims)
{
      for (int i = 0; i < dims[depth]; ++i)
      {
            if (depth != dims.size() - 1)
            {
                  recurse_string(storage, pc, depth + 1, dims);
                  if (i != dims[depth] - 1)
                  {
                        printf(", ");
                  }
                  else
                  {
                        printf("\n");
                  }
            }
            else
            {
                  printf("\"");
                  for (int j = 0; j < dims[depth]; ++j)
                  {
                        int y = peek(storage, pc);
                        printf("%s", tabel[y]);
                        ++pc;
                  }
                  printf("\"");
                  return;
            }
      }
}

void recurse_number(bytevec &storage, int &pc, const int depth, const std::vector<int> &dims)
{
      for (int i = 0; i < dims[depth]; ++i)
      {
            if (depth != dims.size() - 1)
            {
                  recurse_number(storage, pc, depth + 1, dims);
            }
            else
            {
                  printFP(getFP(storage, pc));
                  pc += 5;
                  if (i != dims[depth] - 1)
                  {
                        printf(", ");
                  }
                  else
                  {
                        printf("\n");
                  }
            }
      }
}

int main(int argc, char **argv)
{
      argcrack args(argc, argv);

      if (argc < 2 || args.helprequested())
      {
            puts("\nZX81 P-file Utility by SirMorris  V1.5");
            puts("");
            puts("Largely based on ZX81LIST (C) Jack Raats, Steenbergen 1992");
            puts("");
            puts("  Usage:");
            puts("    ZXTool [p-file name] ([option]...[option])");
            puts("");
            puts("  Options:");
            puts("    vrb            : verbose. Extra information is printed.");
            puts("    list           : produce program listing.");
            puts("    vars           : produce variable listing.");
            puts("    sysvars        : produce system variable listing.");
            puts("    noauto         : stop the program from auto-running.");
            puts("    auto=[line]    : make the program auto-run from the specified line number.");
            puts("    extract=[line] : extract program line to temp.q");
            puts("    insert         : insert/overwrite program line from temp.q");
            puts("    zxpand         : use extended ZXpand BASIC tokens");
            puts("");
            puts("  If no options are specified, a listing is performed.");
            puts("");

            return 0;
      }

      bytevec pfile;
      if (!read(pfile, argv[1]))
      {
            printf("Error opening P file '%s'\n", argv[1]);
            return 1;
      }

      // dfile comes at the end of the BASIC program
      //
      int dfile = dpeek(pfile, D_FILE);

      //printf("Loaded OK. Size: %d bytes\n", pfile.size());

      // will be set to true when an action is performed
      //
      bool acted = false;

      bool verbose = args.ispresent("vrb");
      if (args.ispresent("zxpand")) {
            tabel[255] = "CAT ";
            tabel[225] = "ZXPAND ";
            tabel[226] = "CONFIG ";
      }

      int autorunline;
      if (args.getint("auto=", autorunline))
      {
            printf("Action: set auto-run line number to %d:\n", autorunline);

            int pc = 0x407d;
            int linenumber, linelength;

            while (pc < dfile)
            {
                  getlineinfo(pfile, pc, linenumber, linelength);
                  if (linenumber == autorunline)
                  {
                        break;
                  }
                  pc += linelength;
            }

            if (pc == dfile)
            {
                  printf("  Fail - requested line number not found\n");
            }
            else
            {
                  // set auto-run address to address of requested line number
                  //
                  int autorunaddr = pc - 4;
                  dpoke(pfile, NXTLIN, autorunaddr);

                  if (writevec(argv[1], pfile))
                  {
                        printf("  OK.\n");
                  }
                  else
                  {
                        printf("  Fail - couldn't write P file.\n");
                  }
            }

            acted = true;
      }

      if (args.ispresent("noauto"))
      {
            printf("Action: prevent program from auto-running:\n");

            // set autorun = actual D_FILE
            //
            dpoke(pfile, NXTLIN, dfile);
            if (writevec(argv[1], pfile))
            {
                  printf("  OK\n");
            }
            else
            {
                  printf("  Fail - couldn't write P file.");
            }

            acted = true;
      }

      if (args.ispresent("sysvars"))
      {
            puts("Action - dump system variables:\n");

            int sv = 0x4009;

            for (int i = 0; i < sizeof(sysvars) / sizeof(sysvar); ++i)
            {
                  if (sysvars[i].nBytes == 1)
                  {
                        printf("%s\t%02x     (%5d)  %s\n", sysvars[i].name, peek(pfile, sv), peek(pfile, sv), verbose ? sysvars[i].description : "");
                        ++sv;
                  }
                  else if (sysvars[i].nBytes == 2)
                  {
                        printf("%s\t%04x   (%5d)  %s\n", sysvars[i].name, dpeek(pfile, sv), dpeek(pfile, sv), verbose ? sysvars[i].description : "");
                        sv += 2;
                  }
                  else
                  {
                        printf("%s\t[%d bytes]      %s\n", sysvars[i].name, sysvars[i].nBytes, verbose ? sysvars[i].description : "");
                        sv += sysvars[i].nBytes;
                  }
            }

            acted = true;
      }

      int extractline;
      if (args.getint("extract=", extractline) || args.getint("export=", extractline))
      {
            printf("Action - extract program line %d to file 'temp.q':\n", extractline);

            int pc = 0x407d;
            int linenumber, linelength;

            while (pc < dfile)
            {
                  getlineinfo(pfile, pc, linenumber, linelength);
                  if (linenumber == extractline)
                  {
                        if (write("temp.q", &pfile[pc - 0x4009 - 4], linelength + 4))
                        {
                              printf("  OK - wrote %d bytes.\n", linelength + 4);
                        }
                        else
                        {
                              printf("  Fail - couldn't write to 'temp.q'\n");
                        }
                        break;
                  }

                  pc += linelength;
            }

            if (pc == dfile)
            {
                  printf("  Fail - requested line number not found.\n");
            }

            acted = true;
      }

      if (args.ispresent("insert") || args.ispresent("import"))
      {
            printf("Action - insert program line from 'temp.q':\n");

            // find the first line number which is less than the incoming

            bytevec segment;
            if (read(segment, "temp.q"))
            {
                  int iln = segment[0] * 256 + segment[1];

                  int oldpc, pc = 0x407d;
                  int oldlinelength, linelength, linenumber = -1;

                  while (linenumber < iln && pc < dfile)
                  {
                        getlineinfo(pfile, pc, linenumber, linelength);
                        oldpc = pc - 4;
                        oldlinelength = linelength;
                        pc += linelength;
                  }

                  if (linenumber < iln)
                  {
                        // the line we're inserting is going at the end.
                        // there is no old line so no old line length
                        oldpc = pc - 4;
                  }

                  // we need to keep track of the magnitude of adjustment we have to apply to the system variables
                  //
                  int insertBytes = segment.size();

                  if (linenumber == iln)
                  {
                        char *temp = (char *)(&pfile[0]) + (oldpc - 0x4009);

                        // we need to replace the given line. Erase it from the source P file
                        //
                        pfile.erase(pfile.begin() + (oldpc - 0x4009), pfile.begin() + (oldpc - 0x4009) + oldlinelength + 4);
                        insertBytes -= oldlinelength + 4;
                  }

                  pfile.insert(pfile.begin() + (oldpc - 0x4009), segment.begin(), segment.end());

                  // re-align the system variables
                  //
                  dpoke(pfile, D_FILE, dpeek(pfile, D_FILE) + insertBytes);
                  dpoke(pfile, DF_CC, dpeek(pfile, DF_CC) + insertBytes);
                  dpoke(pfile, VARS, dpeek(pfile, VARS) + insertBytes);
                  dpoke(pfile, E_LINE, dpeek(pfile, E_LINE) + insertBytes);
                  dpoke(pfile, CH_ADD, dpeek(pfile, CH_ADD) + insertBytes);
                  dpoke(pfile, STKBOT, dpeek(pfile, STKBOT) + insertBytes);
                  dpoke(pfile, STKEND, dpeek(pfile, STKEND) + insertBytes);

                  // stop an updated program from auto-running
                  //
                  dpoke(pfile, NXTLIN, dpeek(pfile, D_FILE));

                  if (writevec(argv[1], pfile))
                  {
                        printf("  OK - line %d %s.\n", iln, iln == linenumber ? "overwritten" : "inserted");
                  }
                  else
                  {
                        puts("  Fail - P file was not written\n");
                  }
            }

            acted = true;

            // hmm, should probably not allow anything else to happen after program modification??
      }

      if (args.ispresent("vars"))
      {
            printf("Action - dump variables:\n");

            int vars = dpeek(pfile, VARS);
            int end = dpeek(pfile, E_LINE);
            while (vars < end - 1)
            {
                  unsigned char vartoken = peek(pfile, vars);
                  int type = vartoken >> 5;
                  int letter = (vartoken & 0x3f) | 0x20;
                  switch (type)
                  {
                  case 3:
                        // single-letter number
                        //printf("NUMBR %s = ", tabel[letter]);
                        printf("\n%s = ", tabel[letter]);
                        printFP(getFP(pfile, vars + 1));
                        printf("\n");
                        vars += 6;
                        break;

                  case 5:
                        // multi-letter number
                        //printf("NUMBR %s", tabel[letter]);
                        printf("\n%s", tabel[letter]);
                        while (letter < 128)
                        {
                              ++vars;
                              letter = peek(pfile, vars);
                              printf("%s", tabel[letter & 0x7f]);
                        }
                        printf(" = ");
                        printFP(getFP(pfile, vars + 1));
                        printf("\n");
                        vars += 6;
                        break;

                  case 4:
                        // array of numbers
                        //printf("N-ARR %s(", tabel[letter]);
                        printf("\n%s(", tabel[letter]);
                        {
                              int tlen = dpeek(pfile, vars + 1);
                              int ndims = peek(pfile, vars + 3);
                              std::vector<int> dims;

                              vars += 4;

                              // collect the dimensions
                              for (int i = 0; i < ndims; ++i)
                              {
                                    dims.push_back(dpeek(pfile, vars));
                                    printf("%s%d", i ? "," : "", dims[i]);
                                    vars += 2;
                              }
                              printf(") =\n");

                              recurse_number(pfile, vars, 0, dims);
                        }
                        break;

                  case 7:
                        // for/next loop counter
                        //printf("FR/NX %s = ", tabel[letter]);
                        printf("\n%s = ", tabel[letter]);
                        printFP(getFP(pfile, vars + 1));
                        printf(" TO ");
                        printFP(getFP(pfile, vars + 6));
                        printf(" STEP ");
                        printFP(getFP(pfile, vars + 11));
                        printf("\n");
                        vars += 18;
                        break;

                  case 2:
                        // string
                        //printf("STRNG %s = '", tabel[letter]);
                        printf("\n%s = \"", tabel[letter]);
                        {
                              for (int i = 0; i < dpeek(pfile, vars + 1); ++i)
                              {
                                    printf("%s", tabel[peek(pfile, vars + 3 + i)]);
                              }
                        }
                        printf("\"\n");
                        vars += 3 + dpeek(pfile, vars + 1);
                        break;

                  case 6:
                        // array of characters
                        // TODO - print the values?
                        //printf("C-ARR %s$(", tabel[letter]);
                        printf("\n%s$(", tabel[letter]);
                        {
                              int tlen = dpeek(pfile, vars + 1);
                              int ndims = peek(pfile, vars + 3);
                              std::vector<int> dims;

                              vars += 4;

                              // collect the dimensions
                              for (int i = 0; i < ndims; ++i)
                              {
                                    dims.push_back(dpeek(pfile, vars));
                                    printf("%s%d", i ? "," : "", dims[i]);
                                    vars += 2;
                              }
                              printf(") =\n");

                              // the width of each string in the array is the last dimension
                              // they are stored:
                              //   1,1,... 1,2,... 1,3,... 1,x,n
                              //   2,1,... 2,2,... 2,3,...

                              // sounds like a job for recursion
                              recurse_string(pfile, vars, 0, dims);
                        }
                        break;
                  }
            }
            acted = true;
      }

      if (args.ispresent("list") || !acted)
      {
            int pc = 0x407d;
            int linenumber, linelength;

            while (pc < dfile)
            {
                  getlineinfo(pfile, pc, linenumber, linelength);
                  if (verbose)
                  {
                        printf("%04x  % 5d ", pc - 4, linenumber);
                  }
                  else
                  {
                        printf("% 5d ", linenumber);
                  }

                  while (linelength)
                  {
                        unsigned char token = peek(pfile, pc);
                        switch (token)
                        {
                        case REMTOKEN:
                              printf("%s ", tabel[token]);
                              for (int i = 1; i < linelength - 1; ++i)
                              {
                                    if (peek(pfile, pc + i) & 0x40)
                                    {
                                          // stop - unprintable character detected
                                          //
                                          printf("[%d bytes]", linelength - 2);
                                          pc += linelength - 2;
                                          linelength = 2;
                                          break;
                                    }
                                    // else go on to display the characters
                              }

                              ++pc;
                              --linelength;
                              break;

                        case NEWLINE:
                              puts("");
                              ++pc;
                              --linelength;
                              break;

                        case FPMARKER:
                              pc += 6;
                              linelength -= 6;
                              break;

                        default:
                              printf("%s", tabel[token]);
                              ++pc;
                              --linelength;
                              break;
                        }
                  }
            }
      }
}
