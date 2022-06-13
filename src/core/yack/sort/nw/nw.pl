use Algorithm::Networksort;
use Algorithm::Networksort::Best qw(:all);

my @algo = ( "bosenelson", "batcher" );

#my $nw = nwsrt(inputs => $inputs, algorithm => $algorithm);
for my $small (2..8)
{
	for my $alg (@algo)
	{
		my $nw = nwsrt(inputs => $small, algorithm => $alg);
		print $nw->title(), "\n";
		print $nw, "\n\n";
	}

}

exit(0);
 
my $inputs = 9;
 
#
# First find if any networks exist for the input size.
#
my @nwkeys = nw_best_names($inputs);
 
#
# For each sorting network, show the comparators.
#
for my $name (@nwkeys)
{
    my $nw = nwsrt_best(name => $name);
 
    #
    # Print the list, and print the graph of the list.
    #
    print $nw->title(), "\n", $nw->formatted(), "\n\n";
    #print $nw->graph_text(), "\n\n";
}

