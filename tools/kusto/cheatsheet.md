# kusto query language

## basic concepts
* queries consists of multiple query statements, delimited by a semicolon(`;`)
* pipe(`|`) is used to filter the tabular data produced by the query operator before the pipe through the query operator after the pipe.

## operators
* `count`: return the rows coun in the table
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| count
```
* `take`: returns up to the specified number of rows of data
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| take 100
```
* `project`: selects columns to show
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| project deviceHostName, ifName, interfaceMuxHardwareState, interfaceMuxOverallState, interfaceMuxHealth
| take 100
```
* `where`: filters a table by a predicate
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| project deviceHostName, ifName, interfaceMuxHardwareState, interfaceMuxOverallState, interfaceMuxHealth
| where deviceHostName contains "mwh03-0101-0805-02"
| take 10
```
* `sort`: sort the rows of input table into order by one or more
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| project deviceHostName, ifName, interfaceMuxHardwareState, interfaceMuxOverallState, interfaceMuxHealth
| where deviceHostName contains "mwh03-0101-0805-02"
| take 10
| sort by ifName desc 
```
* `top`: returns the first N records sorted by the specified columns
```
cluster("Azphynet").database("azdhmds").MuxCableFirmwareVersionST
| project deviceHostName, ifName, interfaceMuxHardwareState, interfaceMuxOverallState, interfaceMuxHealth
| top 10 by ifName
```
* `extend`: computes derived columns
* `summarize`: aggregates groups of rows
