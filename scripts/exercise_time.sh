#!/bin/bash

# This is a script to announce remaining exercise time

start_time=300
dec=10

let x=$start_time
while [[ $x -gt 0 ]]; do
	echo "you have $x seconds remaining for the exercise" | espeak > /dev/null 2> /dev/null
	echo $x
	let "x=x-10"
	sleep 10
done
