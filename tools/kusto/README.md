# kusto

* sample data in the Azure Data Explorer: https://dataexplorer.azure.com/clusters/help/databases/Samples

## kusto query
* kusto query starts with the data source, followed by commands that transform the data into what is needed
    * the data source could either be a table or an operator that produces a table
    * the pipe `|` delimiter is used to pass the output from previous command into the next command

### query statement

* kusto query statements: query statements in kQL produces tables that could be used in other parts of the query and must end with a semicolon(`;`)

#### let statement
* let statements are used for
    * define scalar values
    * define a table
    * define a view
    * define a function

* `let` to define scalar values
```kusto
let cutoff = ago(1m);
StormEvents
| where StartTime > cutoff
| count 
```
* `let` to define a function
    * supported arg type:
        * `bool`
        * `string`
        * `long`
        * `datetime`
        * `timespan`
        * `real`
        * `dynamic`
```kusto
let MultiplyByN = (val:long, n:long) { val * n };
range x from 1 to 5 step 1
| extend result = MultiplyByN(x, 100)
```
* `let` to define a table
```kusto
let NCEvents = StormEvents | where State == "NORTH CAROLINA";
NCEvents
| take 10
```

### string operators
* `term`
    * kusto splits a string value into maximum sequence of ASCII alphanumeric characters, and each of those sequences is made into a term
    * kusto builds a term index consisting of all terms that are 3 characters long or more
    * **character scanning using `contains` operator is much slower than term indexing using string operators such as `has`, `!has`, and so on.**

|Operator|Description|Case-Sensitive|Example (yields true)|
|-|-|-|-|
|==|Equals|Yes|"aBc" == "aBc"|
|!=|Not equals|Yes|"abc" != "ABC"|
|=~|Equals|No|"abc" =~ "ABC"|
|!~|Not equals|No|"aBc" !~ "xyz"|
|contains|RHS occurs as a subsequence of LHS|No|"FabriKam" contains "BRik"|
|!contains|RHS doesn't occur in LHS|No|"Fabrikam" !contains "xyz"|
|has|Right-hand-side (RHS) is a whole term in left-hand-side (LHS)|No|"North America" has "america"|
|!has|RHS isn't a full term in LHS|No|"North America" !has "amer"|
|has_all|Same as has but works on all of the elements|No|"North and South America" has_all("south", "north")|
|has_any|Same as has but works on any of the elements|No|"North America" has_any("south", "north")|
|in|Equals to one of the elements|Yes|"abc" in ("123", "345", "abc")|
|!in|Not equals to any of the elements|Yes|"bca" !in ("123", "345", "abc")|
|in~|Equals to any of the elements|No|"Abc" in~ ("123", "345", "abc")|
|!in~|Not equals to any of the elements|No|"bCa" !in~ ("123", "345", "ABC")|

### tabular operators

|name|description|
|-|-|
|print|output a single row with one or more scalar expressions|
|search|provides a multi-table/multi-column search experience|
|where|filters a table to the subset of rows that satisfy a comparison|
|take/limit|returns up to the specified number of rows|
|summarize| produces a table that summarizes the content of the selected columns|
|extend|creates calculated columns and appends them to the result set|
|project|selects the columns to include|
|distinct|produces a table with the distinct combination of the provided columns of the input table|
|sort/order|sort the rows of the input table into order by one or more columns|
|join|merges the rows of two tables to form a new table by matching the values of the specified columns from table|
|union|take two or more tables and returns the rows of all of them|
|render|display the output in a chart, including pie, bar, and line charts|
#### print
```kusto
print name = "Alice", job = "Consultant", Salary = 100
| take 1
```
#### search
* `search` tries to find qualified rows that has columns that satisfy the search predicate
* column restriction: `ColumnName:StringLiteral`
* search predicate:

|literal|operator|
|-|-|
|billg|has|
|\*billg|hassuffix|
|billg*|hasprefix|
|\*billg\*|contains|
|bil\*lg|matches regex|

* query to search rows that has `State` that `has` `"New York"`
```kusto
StormEvents
| search State:"New York"
| take 1
```
* query to search for rows that has `State` starts with `"New"`
```kusto
StormEvents
| search State:"New*"
| take 1
```
* query to search with case insensitive
```kusto
StormEvents
| search kind=case_insensitive State:"new york"
| take 1
```

#### where
```kusto
StormEvents
| where State hassuffix "York"
| take 1
```

### summarize
* `summarize` groups the rows according to the `by` group columns, and calculates aggregations over each group
* the input rows are arranged into groups having the same values of the `by` expressions, then the specified aggregation functions are computed over each group, producing a row for each group. The result contains the `by` columns and also at least one column for each computed aggregate.
* syntax:

```
T | summarize [Column =] Aggregation, ... by [Column =] GroupExpression, ...
```
#### unique combination
* without `by`, `summarize` calculates unique combinations of specified columns:
* without `summarize`
```kusto
StormEvents
| search State:"MISSISSIPPI"
| take 5
| project State, Source
```
* output
![image](https://user-images.githubusercontent.com/35479537/188535378-11d4d3eb-52af-40c6-aeca-1811a21b7636.png)

* with `summarize`
```kusto
StormEvents
| search State:"MISSISSIPPI"
| take 5
| summarize by State, Source
```

* output
![image](https://user-images.githubusercontent.com/35479537/188535488-00da6d59-c29c-4712-8795-14f1fc57e4d2.png)

#### minimum and maximum timestamp
```kusto
StormEvents
| search State:"MISSISSIPPI"
| summarize Min = min(StartTime), Max = max(StartTime)
```

#### count
* use `summarize` to get a new colum called `StormCount`, which is the count of storm events for a state
```kusto
StormEvents
| summarize StormCount = count() by State
| sort by StormCount
| take 5
```

#### arg_max
* get most recent storm event for each state
```kusto
StormEvents
| summarize arg_max(StartTime, *) by State
| take 5
```

### extend
* `extend` allows users to extend a table with new columns
    * the new columns are always shown as the last columns in the list
* get the longest storm event for each state
```kusto
StormEvents
| extend StormDuration = EndTime - StartTime
| summarize LongestStorm = max(StormDuration) by State
| sort by LongestStorm
| take 10
```

### project
* `project` determines the columns to show
* `project` could also be used to create new columns
```kusto
StormEvents
| project State, StormDuration = EndTime - StartTime
| take 10
```

### distinct
* `distinct` is similar to `summarize by`, which produces a table with the distinct combination of the provided columns of the input table
* but `distinct` could use `*`, while `summarize by` could not.
```kusto
StormEvents
| distinct State, Source
| take 5
```

### sort
* `sort by Column, ... [asc]`

### join
* `join` merges the rows of two tables to form a new table by matching values of the specified columns from each table.
* `join` merges the rows that has same columns, and those columns are specified after `on`
```
LeftTable | join [JoinParameters] (RightTable) on Attributes
```
* `LeftTable`: `$left`
* `RightTable`: `$right`


#### join flavor
|join flavor|output|
|-|-|
|innerunique(default)|deduplication the left table first(keep the first record), then join with the right table|
|inner|standard join, every row of the left table is joined with each row on the right table that has the same set of `joined-on` keys|
|leftouter|always contains all records of the left table, even if the join condition doesn't find any matching record|
|rightouter|always contains all records of the right table|
|fullouter|always contains all records of both tables|
|leftanti, anti, or leftantisemi|returns all records from the left side that don't match any record from the right side|
|rightanti or rightantisemi|returns all records from the right side that don't match any record from the left side|
|leftsemi|like `inner`, but keeps the records from the left table only|
|rightsemi|like `inner`, but keeps the records from the right table only|

* let table `X` be

|Name|Count|
|-|-|
|a|1|
|b|2|
|b|3|
|c|4|
* let table `Y` be

|Name|Number|
|-|-|
|b|10|
|c|20|
|c|30|
|d|40|

* `innerunique`

|Name|Count|Name1|Number|
|:----|:----|:----|:----|
|b|2|b|10|
|c|4|c|20|
|c|4|c|30|

* `inner`

|Name|Count|Name1|Number|
|:----|:----|:----|:----|
|b|3|b|10|
|b|2|b|10|
|c|4|c|20|
|c|4|c|30|

* `leftouter`

|Name|Count|Name1|Number|
|:----|:----|:----|:----|
|b|3|b|10|
|b|2|b|10|
|c|4|c|20|
|c|4|c|30|
|a|1| | |	

* `rightouter`

|Name|Count|Name1|Number|
|:----|:----|:----|:----|
|b|3|b|10|
|b|2|b|10|
|c|4|c|20|
|c|4|c|30|
| | |d|40|

* `fullouter`

|Name|Count|Name1|Number|
|:----|:----|:----|:----|
|b|3|b|10|
|b|2|b|10|
|c|4|c|20|
|c|4|c|30|
| | |d|40|
|a|1| | |

* `leftanti`

|Name|Count|
|:----|:----|
|a|1|

* `rightanti`

|Name|Count|
|:----|:----|
|d|40|

* `leftsemi`

|Name|Count|
|:----|:----|
|c|4|
|b|3|
|b|2|

* `rightsemi`

|Name|Number|
|:----|:----|
|b|10|
|c|20|
|c|30|

### union
* `union` takes two or more tables and returns the rows of all of them
    * `withsource`: produce an extra column show which table this record is from
    * `kind` could be either `inner` or `outer`
        * `inner` keeps only common columns
        * `outer` keeps all columns, empty cells are set to `null`
```kusto
let X = view () {
StormEvents
| project-rename EventClass = EventType
| project State, EventClass
| take 5
};
let Y = view() {
StormEvents
| project State, EventType
| take 5
};
union withsource=TableName kind=outer X, Y
```
* output

|TableName|State|EventClass|EventType|
|:----|:----|:----|:----|
|Y|ATLANTIC SOUTH| |Waterspout|
|Y|FLORIDA| |Heavy Rain|
|Y|FLORIDA| |Tornado|
|Y|GEORGIA| |Thunderstorm Wind|
|Y|MISSISSIPPI| |Thunderstorm Wind|
|X|ATLANTIC SOUTH|Waterspout| |
|X|FLORIDA|Heavy Rain| |
|X|FLORIDA|Tornado| |
|X|GEORGIA|Thunderstorm Wind| |
|X|MISSISSIPPI|Thunderstorm Wind| |


# reference
* https://docs.microsoft.com/en-us/azure/data-explorer/kql-quick-reference
* https://docs.microsoft.com/en-us/azure/data-explorer/kusto/query/datatypes-string-operators
