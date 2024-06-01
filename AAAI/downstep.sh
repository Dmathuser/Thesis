#!/bin/bash

# a simple script to help convert a theis to a paper.  

# we need to change:
#  \chapter to \section
#  \section to \subsection
#  \subsection to \subsubsection
#  \subsubsection to \paragraph

sed -i -e "s/\\\subsubsection/\\\paragraph/g" "$1"
sed -i -e "s/\\\subsection/\\\subsubsection/g" "$1"
sed -i -e "s/\\\section/\\\subsection/g" "$1"
sed -i -e "s/\\\chapter/\\\section/g" "$1"
