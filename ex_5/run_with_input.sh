#!/bin/bash

# Input file
input_file="input.txt"

# Path to your C program executable
executable="/Users/yuvalanteby/Library/CloudStorage/OneDrive-Bar-IlanUniversity-Students/BIU/1st_year/Clion/ex_5/ex_5.c"

# Check if the executable exists
if [ ! -x "$executable" ]; then
    echo "Error: Executable not found or not executable."
    exit 1
fi

# Run the program with input redirection
while IFS= read -r line; do
    echo "$line" | "$executable"
done < "$input_file"