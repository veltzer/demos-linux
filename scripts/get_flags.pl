#!/usr/bin/perl -w

use strict;
use diagnostics;

my($source)=$ARGV[0];
my($target)=$ARGV[1];

sub trim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}


my($debug)=0;

if($debug) {
	print "source is $source\n";
	print "target is $target\n";
}
my(@flags);
open(FILE,$source) || die("unable to open file $source");
my($line);
while($line=<FILE>) {
	chop($line);
	if($line=~/EXTRA_CMDS/) {
		my($extra)=$line=~/EXTRA_CMDS=(.*)/;
		$extra=~s/SOURCE/$source/g;
		$extra=~s/TARGET/$target/g;
		$extra=trim(`$extra`);
		push(@flags,$extra);
	}
	if($line=~/EXTRA_LIBS/) {
		my($extra)=$line=~/EXTRA_LIBS=(.*)/;
		$extra=~s/SOURCE/$source/g;
		$extra=~s/TARGET/$target/g;
		$extra=trim($extra);
		push(@flags,$extra);
	}
}
close(FILE) || die("unable to close file $source");

print join(" ",@flags);
