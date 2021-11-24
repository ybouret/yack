use Algorithm::Networksort;

my $nw = nwsrt(inputs => 3, algorithm => 'bosenelson');
$nw->index_base([1..3]);
$nw->formats([ "SWAP(%d, %d);\n" ]);
print $nw->formatted();
#print $nw . "\n";


