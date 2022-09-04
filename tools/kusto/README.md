# kusto

* sample data in the Azure Data Explorer: https://dataexplorer.azure.com/clusters/help/databases/Samples

## kusto query
* kusto query starts with the data source, followed by commands that transform the data into what is needed
    * the data source could either be a table or an operator that produces a table
    * the pipe `|` delimiter is used to pass the output from previous command into the next command

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



# reference
* https://docs.microsoft.com/en-us/azure/data-explorer/kql-quick-reference
* https://docs.microsoft.com/en-us/azure/data-explorer/kusto/query/datatypes-string-operators
