# zxtools

## Two (count 'em) tools to help with cross-development of ZX81 programs.

Built on the shoulders of the giants that are:

* Chris Cowley
* Russell Marks
* Jack Raats

Thanks fellas.

---  
Binaries are available pre-compiled for [windows](https://github.com/charlierobson/zxtools/tree/master/bin-win) and [mac/linux](https://github.com/charlierobson/zxtools/tree/master/bin-mac).
---  

### zxtool will:
* decompile a P file into its associated BASIC text
* dump variables and their content
* dump the SYSVARS
* remove a P file's auto-run
* force a P file to auto-run at a particular line number
* extract and insert arbitrary lines of code
```
mbp:zxtools charlie$ ./zxtool 

ZX81 P-file Utility by SirMorris  V1.5

Largely based on ZX81LIST (C) Jack Raats, Steenbergen 1992

  Usage:
    ZXTool [p-file name] ([option]...[option])

  Options:
    vrb            : verbose. Extra information is printed.
    list           : produce program listing.
    vars           : produce variable listing.
    sysvars        : produce system variable listing.
    noauto         : stop the program from auto-running.
    auto=[line]    : make the program auto-run from the specified line number.
    extract=[line] : extract program line to temp.q
    insert         : insert/overwrite program line from temp.q
    zxpand         : use extended ZXpand+ BASIC tokens

  If no options are specified, a listing is performed.

mbp:zxtools charlie$ 
```

### zxtext2p will:
* compile a text-based representation of a zeddy program into a standard P file
* allow development without line numbers, using a label-based system for GO(TO/SUB)

```
mbp:zxtools charlie$ ./zxtext2p -h

usage: zxtext2p [-h] [-i incr] [-l] [-o output-file] [-s line] [input-file]

        -h      show this help information.
        -i      in labels mode, set line number increment. (default = 2).
        -l      use labels rather than line numbers.
        -o      specify output file (default `out.p').
        -s      in labels mode, set starting line number (default = 10).
mbp:zxtools charlie$ 
```

