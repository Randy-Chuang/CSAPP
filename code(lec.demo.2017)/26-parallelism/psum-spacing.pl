#!/usr/bin/perl

# Collecting experimental data for psum program
# showing effects of false sharing

# Program
$prog = "./psum";

# Parameters
$lognele = 30;
$maxthreads = 16;
$maxspacing = 16;

print "Summing 2^$lognele values\n";
print "Threads\t";
for ($s = 1; $s <= $maxspacing; $s = 2 * $s) {
    print "S$s\t";
}
print "\n";

for ($t = 1; $t <= $maxthreads; $t++) {
    print "$t\t";
    for  ($s = 1; $s <= $maxspacing; $s = 2 * $s) {
	$result = `$prog -t $t -l $lognele -s $s -m g`;
	if ($result =~ "\t([0-9]+\.[0-9]+)") {
	    print "$1\t";
	} else {
	    print "?\t";
	}
    }
    print "\n";
}
