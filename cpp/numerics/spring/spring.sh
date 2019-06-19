#!/bin/bash

make spring_calc && \
    ./spring_calc -m 10 > spring.data

if [[ $? -ne 0 ]]; then
    exit 255
fi

gnuplot -persist <<- 'EOF'
    plot 'spring.data' using 1:2 title "Height" with lines
EOF
