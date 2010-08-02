#!/bin/sed -nr
/call_func/,/^$/ {
	p
}
