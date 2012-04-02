#!/usr/bin/perl -w

# this is a script that greps a full file instead of line by line.

use strict;
use diagnostics;

my($pattern)=$ARGV[0];
my($filenamepattern)=$ARGV[1];
my($debug)=0;
my($print_filename)=1;
my(%printed_filenames);

for(my($i)=2;$i<@ARGV;$i++) {
	my($filename)=$ARGV[$i];
	if($debug) {
		print "filename is [$filename]\n";
	}
	if($filename!~$filenamepattern) {
		next;
	}
	# read the entire file
	my($content)='';
	open(FILE,$filename) || die "Can't open file [$filename]: $!";
	my($line);
	while($line=<FILE>) {
		$content.=$line;
	}
	close(FILE) || die("unable to close file [$filename]: $!");
	if($content=~$pattern) {
		if(!defined($printed_filenames{$filename})) {
			print $filename."\n";
			$printed_filenames{$filename}=defined;
		}
	}
}
