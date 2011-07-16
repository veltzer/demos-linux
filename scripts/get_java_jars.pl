#!/usr/bin/perl -w

use strict;
use diagnostics;
use File::Basename;

=head TODO

Turn this script into one using a standard XML parser and not cat/grep
combination.

=cut

if(scalar(@ARGV)!=1) {
	die("usage: prog [base_jars|jnlp|jars|classpath]");
}

my($cmd)=$ARGV[0];
#my($str)=`cat .classpath | grep kind=\"lib\" | cut -d \\\" -f 4`;
my(@array)=`cat java/.classpath | grep kind=\\\"lib | cut -d \\\" -f 4`;

for(my($i)=0;$i<@array;$i++) {
	chop($array[$i]);
	if($cmd eq "base_jars") {
		my($name,$path,$suffix)=fileparse($array[$i],[".jar"]);
		$array[$i]=$name;
	}
	if($cmd eq "jnlp") {
		my($name,$path,$suffix)=fileparse($array[$i],[".jar"]);
		$array[$i]="<jar href=\\\"".$name."\\\"/>";
	}
	if($cmd eq "classpath") {
		$array[$i]='java/'.$array[$i];
	}
}

if($cmd eq "jars" or $cmd eq "base_jars") {
	print join(" ",@array);
}
if($cmd eq "classpath") {
	print join(":",@array);
}
if($cmd eq "jnlp") {
	print join("\n",@array);
}
