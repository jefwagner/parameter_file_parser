Parameter file parser
=====================

This is a file that lets you read in variables from simple
parameter files. An example parater file is:

```
# Example Parameter file
#--------------------------------------------------------------------
# Any line starting with a `#` is a comment.  
# A comment goes until the end of the line
  # The comment doesn't have to start at the first character.

#--------------------------------------------------------------------
# The main purpose is to list variables and values

var length 10.0

# The format is the keyword `var` followed by a variable name and a 
# value. The variable name must have no spaces or whitespace, and the
# same with the values. 

# The values be anything type
var width 10.0 # floating point values
var num_of_particles 100 # integers
var output_file_name output.dat # string

#--------------------------------------------------------------------# We can also give list of variables

list box_size 10.0 10.0 

# The format is the keyword `list` followed by a variable name,
# followed by the list of values. 

# Line breaks and other whitespace is not meaningfull
list tempuratures 
0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 10
```
