# Random Well-Plate Generator

A random Well Plate data generator written in C++.

---

# Libraries used

| Library                                        | version  |    
|------------------------------------------------|----------|
| [gflags](https://github.com/gflags/gflags)     | 2.2.2    | 
| [gtest](https://github.com/google/googletest)  | 1.12.1   |

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
>>s2 10 3
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

### usage notes 

- if you want to create a single point plate, then 
  - set all _sample_ wells to `s1`
  - set the `DF` column for `>>s1` to either _any float_ (e.g. `10`) or `NA`
  - for example:
```
...
10 1 LR
s1,s1,s1,s1,s1,s1,s1,s1,hc,lc
>>s1 10 NA
...
```
