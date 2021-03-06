use Algorithm::Networksort;
use Algorithm::Networksort::Best qw(:all);

my @algo = ( "bosenelson", "hibbard", "batcher", "bitonic" );

for my $small (2..24)
{
	for my $alg (@algo)
	{
		my $nw = nwsrt(inputs => $small, algorithm => $alg);
		print $alg, $small, "\n";
		print $nw, "\n\n";
	}

}

my @large = (9..24);
for my $inputs (@large)
{
	my @nwkeys = nw_best_names($inputs);
	for my $name (@nwkeys)
	{	
		#print $name,"\n";
    		my $nw = nwsrt_best(name => $name);
		print $name, "\n";
                print $nw, "\n\n";
	}
}

for my $extra (25..32)
{
        for my $alg (@algo)
        {
                my $nw = nwsrt(inputs => $extra, algorithm => $alg);
                print $alg, $extra, "\n";
                print $nw, "\n\n";
        }

}
