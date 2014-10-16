#!/usr/bin/perl -w

# This is a general script to grep using perl to overcome some of the definiencies
# of grep(1) grepping...

use strict;
use diagnostics;

my($pattern)=$ARGV[0];
my($filenamepattern)=$ARGV[1];
my($debug)=0;
my($print_filename)=1;
my($print_onlyfilename)=1;
my(%printed_filenames);

for(my($i)=2;$i<@ARGV;$i++) {
	my($filename)=$ARGV[$i];
	if($debug) {
		print "filename is [$filename]\n";
	}
	if($filename!~$filenamepattern) {
		next;
	}
	open(FILE,$filename) || die "Can't open file [$filename]: $!";
	my($line);
	while($line=<FILE>) {
		if($line=~$pattern) {
			if($print_onlyfilename) {
				if(!defined($printed_filenames{$filename})) {
					print $filename."\n";
					$printed_filenames{$filename}=defined;
				}
			} else {
				if($print_filename) {
					print $filename.": ";
				}
				print $line;
			}
		}
	}
	close(FILE) || die("unable to close file [$filename]: $!");
}
