# Template File Examples 

This page contains some examples of both valid and invalid file formats, with reasons for invalidity.

---

# Valid Formats 

## 96-well plate containing 1 sample type and a Left to right dilution scheme 
```
v1
# 96-well plate with Left>Right dilution scheme 
12 8 LR
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,hc,bl
s1,s,s,s,s,s,s,s,s,s,lc,bl
s1,s,s,s,s,s,s,s,s,s,lc,bl
s1,s,s,s,s,s,s,s,s,s,lc,bl
s1,s,s,s,s,s,s,s,s,s,lc,bl
>>s1 10 10
>>hc 10
>>lc 10
>>bl 0
```


## 96-well plate with Top>Bottom dilution scheme 
```
v1
# 96-well plate with a dilution scheme flowing L->R
12 8 TB
s1,s1,s1,s1,s1,s1,s1,s1,s1,s1,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
s,s,s,s,s,s,s,s,s,s,hc,lc
>>s1 10 10
>>hc 10
>>lc 10
```

## 96-well plate containing 1 sample type and a Left to right dilution scheme where multiple dilution schemes are present per line
```
v1
# 96-well plate with Left>Right dilution scheme 
12 8 LR
s1,s,s,s,s,s1,s,s,s,s,hc,bl
s1,s,s,s,s,s1,s,s,s,s,hc,bl
s1,s,s,s,s,s1,s,s,s,s,hc,bl
s1,s,s,s,s,s1,s,s,s,s,hc,bl
s1,s,s,s,s,s1,s,s,s,s,lc,bl
s1,s,s,s,s,s1,s,s,s,s,lc,bl
s1,s,s,s,s,s1,s,s,s,s,lc,bl
s1,s,s,s,s,s1,s,s,s,s,lc,bl
>>s1 10 10
>>hc 10
>>lc 10
>>bl 0
```
- this well plate contains 2 dilution schemes for `s1` per row, each comprising of 5 wells 

--- 

# Invalid Formats

## invalid version 
```
v6
# 96-well plate with a dilution scheme flowing L->R
5 1 LR
s1,s,s,hc,lc
>>s1 10 10
>>hc 10
>>lc 10
```
- (currently) the only valid version is `v1`


## File where dimensions declared in the format block and used in the declaration block mismatch 
```
v1
# 96-well plate with a dilution scheme flowing L->R
5 1 LR
s1,s,s,hc,lc,bl
>>s1 10 10
>>hc 10
>>lc 10
>>bl 0
```
- number of cols declared to be 5, but actually got 6 


## Declared sample is initialised with NA values 
```
v1
# 96-well plate with a dilution scheme flowing L->R
5 2 LR
s1,s,s,hc,lc
s2,s,s,lc,hc
>>s1 NA 10
>>s2 10 NA
>>hc 10
>>lc 10
>>bl 0
```
- `s1`, the initial concentration is not declared as a valid number 
- `s2`, the dilution factor is not declared as a valid number 

