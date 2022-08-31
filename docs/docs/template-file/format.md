# Tempalte File Format 

## Intro 

In order to facilitate greater flexibility in using the software, a file format for describing the data that a user 
wants to simulate in any format desired.

## Basics 

- The file comprises of several fixed blocks, and several sections of free definition.
- Files all have the `.tplx` extension 

### Version Line 

This line is used to describe the version of the file - currently there is only 1 version, `v1`.

Example:
>v1

---

### Description line 

This line is used to contain a summary of the template file. 

Example: 
> \# This is a 96-well plate, containing 2 distinct samples with dilution schemes running from Left to Right

#### Notes 
- the line **must be** prefixed with a pound sign (`#`)
- the comment can only span 1 line; wrapping the line manually is not allowed  

---

### Format Declaration Line 

This line contains information describing the dimensions of the plate and the _directionality_ (what direction 
across the plate) of the dilution scheme.

Example:
>12 8 LR

#### Notes
- the line must be laid out in the following way: `<cols> <rows> <directionality>`
- `rows` and `cols` **must be** positive, non-zero integers
- the following values are valid for `directionality`:
  - `LR`: meaning from Left to Right (top to bottom), wherein each new row marks a new sample run 
  - `TB`: meaning from Top to  Bottom (left to right), wherein each new col marks a new sample run 

--- 

### Plate Declaration Block 
This is a more free form section, describing the contents of the plate format - i.e. how many samples, where are they on 
the plate, the position of controls and blanks etc. 

Example (for a plate of dimensions `7x2` with directionality `LR`):
```
s1,s,s,s,s,hc,lc
s2,s,s,s,s,lc,hc
```

This means that: 

- there are 2 distinct samples (`s1` & `s2`) with corresponding dilution schemes flowing from the left most column to the right most
- and that each row of the plate contains a high control (`hc`) and a low control (`lc`). 

#### Notes
- the number of rows and columns must match that declared in the `Format Declaration Line`, or else an error will be thrown
whilst the file is parsed 
- directionality must be one of the valid values provided, else an error will be thrown whilst the file is parsed 
- the set of valid well types are:
  - `sx`: the initial sample in a dilution scheme; is numbered (e.g. `s1`, `s2`, ..., `s400`, ..., `sx`)
  - `s`: a sample in a dilution scheme 
  - `hc`: a _high control_ well
  - `lc`: a _low control_ well
  - `pc`: a _positive control_ well
  - `hc`: a _blank_ well

---

### Data Block 

The data block contains information on how to populate the plate with actual numeric data; these data are used as the initial
concentration values from which simulated data is derived.

Example (for a plate of dimensions `7x2` with directionality `LR`):

Plate Declaration block:
```
s1,s,s,s,s,hc,lc
s2,s,s,s,s,lc,hc
```

Data Block:
```
>>s1 10 10
>>s2 10 2
>>hc 10
>>lc 10
```

#### Notes
- all well-types described in the `plate declaration block` are valid here, except `s` (samples) 
- well types that are not present in the `plate declaration block` can be declared here, but must be given a value of `NA`
- each `sx` declared in the `plate declaration block` must be defined in the `data block`
- the format for each `sx` here is `sx <initial concentration> <dilution factor>`
  - e.g. `s1 10 3` means 
    - each `s1` in the plate will be replaced with the value `10` 
    - each sample (`s`) in a dilution scheme will be a `3-fold` dilution (10, 3.3, 1.1, ...)
- other well types follow this scheme instead: `hc <concentration>` 
  - `hc` can be replaced with `lc`, `pc` or `bl`
- if a well type is both present in the `plate declaration block` and `data block`, but is declared with `NA` then an error will be thrown
