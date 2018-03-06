# CPAR

## Compiling and Executing

### C
```
gcc matrix.c -Wall -O3 -lm -o cMatrix /opt/papi/lib/libpapi.a
# or
gcc matrix.c -Wall -O3 -fopenmp -lm -o cMatrix /opt/papi/lib/libpapi.a

./cMatrix v1
# or
./cMatrix v2
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
| C (v1)   | |
| C (v2)   | |
| C#       |0.52s|  6s  | 54s  |2m44s |2m34s |9m48s |
| Java     |0|0|0|0|0|0|
| Python   | 13s |1m47s |6m15s|14m07s|28m27s|48m05s|

### In-depth C performance analysis

#### _Naive_ algorithm
|Matrix size| Total DCM | FLOP | FLOPS |DCM/FLOP| Time |
|---|---|---|---|---|---|
|  500x500  | 73694933  |2.5e8 |2079918799|0.295 |0.1s|
| 1000x1000 |1206505880 | 2e9  |1606411509|0.603 |1.2s|
| 1500x1500 |4405047596 |6.75e9|1219365915|0.653|5.5s|
| 2000x2000 |13771367675|1.6e10|1490302415|0.861|11s|
| 2500x2500 |25933196711|3.125e10|1471864081|0.83|21s|
| 3000x3000 |46882665625|5.4e10|1561888576|0.868|35s|
|---|---|---|---|---|---|
| 5000x5000 ||2.5e11|
| 6000x6000 ||4.32e11|
| 7000x7000 ||6.86e11|
| 8000x8000 ||1.024e12|
| 9000x9000 ||1.458e12||||
|10000x10000| |2e12| | | |

#### Enhanced algorithm
|Matrix size| Total DCM | FLOP | FLOPS |DCM/FLOP| Time |
|---|---|---|---|---|---|
|  500x500  | 24109594  |2.5e8 |4703845864|0.096|0.05s |
| 1000x1000 | 189951179 | 2e9  |4904870045|0.095| 0.4s |
| 1500x1500 | 627212174 |6.75e9|4970607143|0.093| 1.4s |
| 2000x2000 |1480856000 |1.6e10|5058683894|0.093| 3.2s |
| 2500x2500 |2905929810 |3.125e10|4866208843|0.093|6.4s|
| 3000x3000 |5046291800 |5.4e10|4778139903|0.093| 11s  |
|---|---|---|---|---|---|
| 5000x5000 ||
| 6000x6000 ||
| 7000x7000 ||
| 8000x8000 ||
| 9000x9000 ||
|10000x10000||


#### Ratio comparison (V2/V1)
|Matrix size|DCM|FLOPS|DCM/FLOP|Time|
|---|---|---|---|---|
|  500x500  |18.7%|157.4%|18.6%|62.7%|
| 1000x1000 |16.6%|226.8%|16.5%|46.3%|
| 1500x1500 |14.6%|223.8%|14.6%|43.3%|
| 2000x2000 |15.2%|178.6%|15.2%|56.4%|
| 2500x2500 |11.5%|225.0%|11.5%|44.4%|
| 3000x3000 |7.7% |221.5%|7.7% |17.3%|
|---|---|---|---|---|
||||||

- On average, the enhanced version runs on less than half the time of the _naive_