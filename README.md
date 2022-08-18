# Random Well-Plate Generator

A random Well Plate data generator written in C++.

---

# Libraries used

| Library                                        | version  |    
|------------------------------------------------|----------|
| [gflags](https://github.com/gflags/gflags)     | 2.2.2    | 
| [gtest](https://github.com/google/googletest)  | 1.12.1   |

--- 

## TODO:

- [ ] CLI 
  - [ ] accept path to input template 
  - [ ] accept number of plates to generate
  - [ ] accept equation type
    - [ ] EC50
    - [ ] IC50
    - [ ] XC50 - a mix of (E/I)C50
  - [ ] accept output format
    - [ ] CSV
    - [ ] TSV
    - [ ] Excel (xlsx)
  - [ ] accept output directory 
    - must exist prior to call 

- [ ] template parser 
  - [ ] read template file 
  - [ ] parse template into internal representation of plate
    - [ ] read + store file version 
    - [ ] preallocate array of `cols`x`rows` dims
    - [ ] set directionality 
  - [ ] store values for parameters 
    - store in map 
    - convert `NA` to `NULL/nullptr`

- [ ] data fuzzer  
  - [ ] take internal template representation 
  - [ ] create initial array of 0 vals 
  - [ ] set values for all defined parameters (sx, hc, lc, pc, bl)
  - [ ] for each `sx` (if present)
    - [ ] iterate over the array in the defined manner (e.g. `LR`=`Left->Right`)
    - [ ] based on ith col/row, perform dilution (e.g. 3rd col with df=10, perform dilution of 10^3)
  - [ ] iterate over all values and 
    - [ ] derive a value based on the supplied equation
    - [ ] 'fuzz' derived data (add some noise to the data, using guassian distribution)
  - [ ] data generated should be a 2d array of floating point values 
  - [ ] push finalised plate to a queue  

- [ ] exporter 
  - [ ] check `save-dir` is an existing directory 
    - [ ] if not, exit program 
  - [ ] pop plate from queue (when available)
  - [ ] write file to output directory 
    - [ ] filename 
      - should be some UUID 
      - alternatively generate hash of file and use that for name

- [ ] documentation 
  - [ ] add CLI help menu 
  - [ ] create doc site for usage 
    - [ ] documentation on file format 
    - [ ] documentation on CLI usage 
    - [ ] documentation on _Interactive mode_ usage

- [ ] CI/CD
  - [ ] run some tests on each build 
  - [ ] job to build docs on push to docs dir 
  - [ ] job to build exe for win+linux on push to src 
  - [ ] job to auto-generate a changelog 
    - [ ] git commit messages should follow [conventional commits standards](https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13)
    - [ ] [article on changelog generation from conventional commits](https://mokkapps.de/blog/how-to-automatically-generate-a-helpful-changelog-from-your-git-commit-messages/)

- [ ] allow user to run in _Interactive Mode_
    - [ ] CLI flag that gives user a TUI which will help for debugging
        - usage: `<exe> -t <path to template> <opts> -I`
    - [ ] print description of file to user
    - [ ] print plate format block
    - [ ] prompt user to continue
        - [ ] if yes continue
        - [ ] else exit program
    - [ ] show user a progress bar 

- [ ] implement template file param override (v2) 
  - e.g. override `s1=10` with `s1=20` (`--override 's1=10',<additional overrides>`)

- [ ] figure out some threading system (v2/3) 
  - default to 1 
  - provide CLI flag for specifying the number of execution threads 

--- 

## Custom template file format

Proposed file format for specifying template files for data generation.

- file extension: `.tplx`
- example of the format:

```
v1
# 96-well plate with a dilution scheme flowing L->R
12 8 LR
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,hc,bl
s2,s,s,s,s,s,s,s,s,s,hc,bl
s2,s,s,s,s,s,s,s,s,s,bl,lc
s2,s,s,s,s,s,s,s,s,s,bl,lc
s2,s,s,s,s,s,s,s,s,s,bl,lc
s2,s,s,s,s,s,s,s,s,s,bl,lc
>>s1 10 10
>>s2 10  3
>>hc 10
>>lc 10
>>bl NA
>>pc NA
```

### format details

- `v<x>`: version specifier
- `# <template descriptor>`: comment line describing the template created; must be 1 line, **do not wrap**.
- declaration block:
    - `<x> <y>`: the `cols`x`rows` dimensions for the plate format (e.g. `12 8` means a `12` col x `8` row plate)
    - `<dir>`: specifies the directionality for a given sample dilution scheme. valid values:
        - `LR`: Left->Right (Top->Bottom)
        - `TB`: Top->Bottom (Left->Right)
- well plate format block:
    - CSV block of parameter names
    - contains information about what data is associated with each row/column in the template
        - e.g. for size `5 1` and the following format block `s  s  s  hc lc`
        - there are **3 distinct samples** (`s`)
        - there is 1 _high control_ sample (`hc`)
        - there is 1 _low control_ sample (`lc`)
- data block:
    - contains information about initialisation values for each masked value in the table
    - samples
        - `<sample id> <initial conc.> <dilution factor>`
    - high control
        - `hc <conc.>`
    - low control
        - `lc <conc.>`
    - blank
        - `bl <conc.>`
    - positive control
      - `pc <conc.>`
