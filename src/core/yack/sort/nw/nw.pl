use Algorithm::Networksort;
use Algorithm::Networksort::Best qw(:all);

my @algo = ( "bosenelson", "batcher" );

for my $small (2..8)
{
	for my $alg (@algo)
	{
		my $nw = nwsrt(inputs => $small, algorithm => $alg);
		print $alg, $small, "\n";
		print $nw, "\n\n";
	}

}

my @large = (9..12);
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

