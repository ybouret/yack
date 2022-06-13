use Algorithm::Networksort;

my $nw = Algorithm::Networksort->new(inputs => 4, algorithm => 'hibbard');
 
print "There are ", $nw->length(),
    " comparators in this network, grouped into\n",
    $nw->depth(), " parallel operations.\n\n";

print $nw, "\n";
 
my @grouped_network = $nw->group(grouping=>'parallel');
print "\nThis will be graphed in ", scalar @grouped_network,
    " columns.\n";

print $nw->graph_text();

