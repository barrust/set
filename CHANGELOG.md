## Current Version

* Updated the FNV-1a seed value

### Version 0.2.0
* Add ability to set the initial size of the set
* Correct usage of `const char*`
* Added in-depth testsuite

### Version 0.1.9
* Speed up the node removal process
* Set compare function
    * New definitions for comparison results
* Set clear function

### Version 0.1.8
* Performance improvements
    * Reduce calls to strlen
    * strncpy replaced with memcpy since non-overlapping memory

### Version 0.1.7
* Unique HashFunction type when using with related libraries

### Version 0.1.6
* Restructured repository
* Add set_to_array function to get keys from the set easily
* Speed improvements when removing keys

### Version 0.1.5
* First version including:
   * Insertion, Lookup, and Removal of elements
   * Union and Intersection
   * Difference and Symmetric Difference
   * Superset, and Subset
