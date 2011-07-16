#!/usr/bin/perl -w

# This is a script that wraps the running of make.
#
# Why do I need this ?
# To wrap stuff like building kernel module which prints way too much stuff on the screen.
#
# Design:
# - this script runs make as a child process

use strict;
use diagnostics;
use File::Spec qw();

# parameters
my($debug)=0;
my($args)='';
my($prog)='make';
my($remove_tmp)=1;

# this is a function that removes a file and can optionally die if there is a problem
sub unlink_check($$) {
	my($file,$check)=@_;
	if($debug) {
		print 'unlinking ['.$file.']'."\n";
	}
	my($ret)=unlink($file);
	if($check) {
		if($ret!=1) {
			die('problem unlinking file ['.$file.']');
		}
	}
}

# here we go...
my($volume,$directories,$myscript) = File::Spec->splitpath($0);
# temporary file name to store errors...
my($tmp_fname)='/tmp/'.$myscript.$$;

my($cmd)=$prog.' '.join(' ',@ARGV).' > /dev/null 2> '.$tmp_fname;
if($debug) {
	print 'cmd is ['.$cmd.']'."\n";
	print 'tmp_fname is ['.$tmp_fname.']'."\n";
}
my($res)=system($cmd);
if($debug) {
	print 'system returned ['.$res.']'."\n";
}
if($res) {
	# print the errors
	open(FILE,$tmp_fname) || die('unable to open');
	my($line);
	while($line=<FILE>) {
		print $line;
	}
	close(FILE) || die('unable to close');
	# remove the tmp file for the errors
	if($remove_tmp) {
		unlink_check($tmp_fname,1);
	}
	# exit with error code of the child...
	exit($res << 8);
} else {
	# remove the tmp file for the errors
	if($remove_tmp) {
		unlink_check($tmp_fname,1);
	}
}
