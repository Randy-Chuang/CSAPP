#!/usr/bin/perl

# Collecting experimental data for psum program

# Program
$prog = "./psum";

# Parameters
$lognele = 30;
$maxthreads = 16;
$ntests = 6;
@modes = ("r", "s", "m", "g", "g", "l");
@sizes = (1, 1, 1, 1, 16, 1);
@descrs = ("Race", "Semaphore", "Mutex", "Adjacent memory acc", "Spaced memory acc", "Register acc");

print "Summing 2^$lognele values\n";
print "Threads\t";
for ($i = 0; $i < $ntests; $i++) {
    print "$descrs[$i]\t";
}
print "\n";

for ($t = 1; $t <= $maxthreads; $t++) {
    print "$t\t";
    for  ($i = 0; $i < $ntests; $i++) {
	$result = `$prog -t $t -l $lognele -s $sizes[$i] -m $modes[$i]`;
	if ($result =~ "\t([0-9]+\.[0-9]+)") {
	    print "$1\t";
	} else {
	    print "?\t";
	}
    }
    print "\n";
}
