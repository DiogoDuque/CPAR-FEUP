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

### In-depth C performance analysis

#### _Naive_ algorithm
|Matrix size| Total DCM | FLOP | FLOPS |DCM/FLOP| Time |
|---|---|---|---|---|---|
|  500x500  | 131591316 |2.5e8 |334711900|0.526 |0.75s|
| 1000x1000 |1221865614 | 2e9  |237798047|0.611 |8s|
| 1500x1500 |4725244659 |6.75e9|228237680|0.700|30s|
| 2000x2000 |10705479907|1.6e10|292430201|0.669|55s|
| 2500x2500 |27672382361|3.125e10|230752839|0.886|2m15s|
| 3000x3000 |71338400216|5.4e10|239550480|1.321|9m48s|

#### Enhanced algorithm
|Matrix size| Total DCM | FLOP | FLOPS |DCM/FLOP| Time |
|---|---|---|---|---|---|
|  500x500  | 24601735  |2.5e8 |526900371|0.098 |0.47s|
| 1000x1000 | 202614102 | 2e9  |539410548|0.101 |3.7s |
| 1500x1500 | 691060765 |6.75e9|510887852|0.102 | 13s |
| 2000x2000 |1633239359 |1.6e10|522205942|0.102 | 31s |
| 2500x2500 |3188586292 |3.125e10|519162374|0.102| 1m |
| 3000x3000 |5494637425 |5.4e10|530525707|0.102 |1m42s|

#### Ratio comparison (V2/V1)
|Matrix size|DCM|FLOPS|DCM/FLOP|Time|
|---|---|---|---|---|
|  500x500  |18.7%|157.4%|18.6%|62.7%|
| 1000x1000 |16.6%|226.8%|16.5%|46.3%|
| 1500x1500 |14.6%|223.8%|14.6%|43.3%|
| 2000x2000 |15.2%|178.6%|15.2%|56.4%|
| 2500x2500 |11.5%|225.0%|11.5%|44.4%|
| 3000x3000 |7.7% |221.5%|7.7% |17.3%|

- On average, the enhanced version runs on less than half the time of the _naive_
