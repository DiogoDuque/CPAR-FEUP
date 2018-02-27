# CPAR

## Compiling and Executing

### C
```
gcc matrix.c -Wall -o cMatrix /opt/papi/lib/libpapi.a
./cMatrix
```

### Java
```
javac Matrix.java
java Matrix
```

### C#
```
WIP
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

### Matrix-multiplying algorithm

| Language | Times |
|          | 500 | 1000 | 1500 | 2000 | 2500 | 3000 | Total|
|----------|-----|------|------|------|------|------|------|
| C        |0|0|0|0|0|0|0|
| C#       |0|0|0|0|0|0|0|
| Java     |0|0|0|0|0|0|0|
| Python   |13s|1m47s|6m15s|14m07s|28m27s|48m05s|1h38m55s|