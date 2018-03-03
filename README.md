# CPAR

## Compiling and Executing

### C
```
gcc matrix.c -Wall -lm -o cMatrix /opt/papi/lib/libpapi.a
./cMatrix
```

### Java
```
javac Matrix.java
java Matrix
```

### C#
```
mcs Matrix.cs
mono Matrix.exe
```

### Python 2.7
```
python matrix.py
```


## Running the tools

### PAPI
Checking available flags in my system:
```
/opt/papi/bin/papi_avail -a
```

### TipTop
```
/opt/tiptop/bin/tiptop
```


## Running Times

### Matrix-multiplying _naive_ algorithm

|Languages | 500 | 1000 | 1500 | 2000 | 2500 | 3000 |
|:--------:|-----|------|------|------|------|------|
| C        |0.75s|  8s  | 30s  | 55s  |2m15s |3m45s |
| C#       |0.52s|  6s  | 54s  |2m44s |2m34s |9m48s |
| Java     |0|0|0|0|0|0|
| Python   | 13s |1m47s |6m15s|14m07s|28m27s|48m05s|

### In-depth C analysis

#### _Naive_ algorithm
|Matrix size| DCM(L1)  | DCM(L2)  | TotalDCMs | FLOP | FLOPS | CM/FLOP|
|---|---|---|---|---|---|---|
|  500x500  |109360723 | 22230593 | 131591316 |2.5e8 |334711900|0.526 |
| 1000x1000 |1064928939|156936675 |1221865614 | 2e9  |237798047|0.611 |
| 1500x1500 |3591878018|1133366641|4725244659 |6.75e9|228237680|0.700|
| 2000x2000 |8508132460|2197347447|10705479907|1.6e10|292430201|0.669|
| 2500x2500 |16610916870|11061465491|27672382361|3.125e10|230752839|0.886|
| 3000x3000 |28705456121|42632944095|71338400216|5.4e10|239550480|1.321|
