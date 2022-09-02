# Tempalte File Format 

## Intro 

In order to facilitate greater flexibility in using the software, a file format for describing the data that a user 
wants to simulate in any format desired.

## Basics 

- The file comprises several fixed blocks, and several sections of free definition.
- Files all have the `.tplx` extension

!!! example

    96-well-lr.tplx

---

## Version Line 

This line is used to describe the version of the file - currently there is only 1 version, `v1`.

!!! example

    v1

---

## Description line 

This line is used to contain a summary of the template file. 

!!! example
    
    ```# This is a 96-well plate, containing 2 distinct samples with dilution schemes running from Left to Right```

!!! danger 

    the comment can only span 1 line; wrapping the line manually is not allowed.  

??? note
    
    the line **must be** prefixed with a pound sign (`#`).

---

## Format Declaration Line 

This line contains information describing the dimensions of the plate and the _directionality_ (what direction 
across the plate) of the dilution scheme.

!!! example

    12 8 LR

!!! warning 

    `rows` and `cols` **must be** positive, non-zero integers

!!! info

    the line must be laid out in the following way: `<cols> <rows> <directionality>`

??? info "valid values for directionality"
    the following values are valid for `directionality`:
    <ul>
      <li>`LR`: meaning from Left to Right (top to bottom), wherein each new row marks a new sample run</li>
      <li>`TB`: meaning from Top to  Bottom (left to right), wherein each new col marks a new sample run</li>
    <ul>

--- 

## Plate Declaration Block 
This is a more free form section, describing the contents of the plate format - i.e. how many samples, where are they on 
the plate, the position of controls and blanks etc. 

Example (for a plate of dimensions `7x2` with directionality `LR`):
!!! example

    ```
    s1,s,s,s,s,hc,lc
    s2,s,s,s,s,lc,hc
    ```


This means that: 

- there are 2 distinct samples (`s1` & `s2`) with corresponding dilution schemes flowing from the left most column to the right most
- and that each row of the plate contains a high control (`hc`) and a low control (`lc`). 

!!! warning 

    the number of rows and columns must match that declared in the `Format Declaration Line`, or else an error will be thrown
    whilst the file is parsed 

!!! warning

    directionality must be one of the valid values provided, else an error will be thrown whilst the file is parsed

??? info "valid well types"

    the set of valid well types are:
    <ul>
      <li>`sx`: the initial sample in a dilution scheme; is numbered (e.g. `s1`, `s2`, ..., `s400`, ..., `sx`)</li>
      <li>`s`: a sample in a dilution scheme</li>
      <li>`hc`: a _high control_ well</li>
      <li>`lc`: a _low control_ well</li>
      <li>`pc`: a _positive control_ well</li>
      <li>`hc`: a _blank_ well</li>
    </ul>

---

## Data Block 

The data block contains information on how to populate the plate with actual numeric data; these data are used as the initial
concentration values from which simulated data is derived.

Example (for a plate of dimensions `7x2` with directionality `LR`):

!!! example "Plate Declaration block & Data Block"

    ```
    s1,s,s,s,s,hc,lc
    s2,s,s,s,s,lc,hc
    >>s1 10 10
    >>s2 10 2
    >>hc 10
    >>lc 10
    ```

!!! tip

    all well-types described in the `plate declaration block` are valid here, except `s` (samples)

!!! danger

    if a well type is both present in the `plate declaration block` and `data block`, but is declared with `NA` then an error will be thrown

!!! note

    each `sx` declared in the `plate declaration block` must be defined in the `data block`

??? note "using `NA` values correctly"

    well types that are not present in the `plate declaration block` can be declared here, but must be given a value of `NA`

??? info "sample (`sx`) data format"

    the format for each `sx` here is `sx <initial concentration> <dilution factor>`
    <br/>e.g. `s1 10 3` means 
    <ul>
      <li>each `s1` in the plate will be replaced with the value `10`</li> 
      <li>each sample (`s`) in a dilution scheme will be a `3-fold` dilution (10, 3.3, 1.1, ...)</li>
    </ul>

??? info "other well type data format"

    other well types follow this scheme instead: `hc <concentration>`
    <br/>`hc` can be replaced with `lc`, `pc` or `bl`
