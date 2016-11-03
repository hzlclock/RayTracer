#!/usr/bin/perl -w

require strict;

my ($dir, $test_dir, $scheme, $token) = @ARGV;

open S, "$scheme" || die "FAIL: could not open grading scheme.";

sub run_test
{
    my ($file,$max_error,$solution,$mask)=@_;
    my $pass_error = 0;
    `rm -f $dir/file.txt ; cp $test_dir/$file $dir/file.txt`;
    `rm -f $dir/sol.png ; cp $test_dir/$solution $dir/sol.png`;
    `rm -f $dir/mask.png ; cp $test_dir/$mask $dir/mask.png`;
    $_=`( cd $dir ; timeout 10 ./ray_tracer -d file.txt sol.png mask.png || echo TIMEOUT )`;
    if(/TIMEOUT/)
    {
        print "FAIL: ($file) Diff test timed out.\n";
    }
    elsif(/$token ([0-9.]+)$/)
    {
        if($1+0>$max_error)
        {
            print "FAIL: ($file) Too much error. Actual: $1  Max: $max_error.\n";
        }
        else
        {
            print "PASS: ($file) diff $1 vs $max_error.\n";
            return 1;
        }
    }
    else
    {
        print "FAIL: ($file) Did not find diff amount.\n";
    }
    return 0;
}

my $total_score=0;
while(<S>)
{
    /^\s*\#/ && next;
    /^\s*$/ && next;
    if(/^t\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*$/)
    {
        my $points=$1;
        my $max_error=$2;
        my $file=$3;
        my $solution=$4;
        my $mask=$5;

        if(&run_test($file,$max_error,$solution,$mask))
        {
            print "+$points points\n";
            $total_score+=$points;
        }
        else
        {
            print "no points\n";
        }
        
        next;
    }

    print "Unrecognized command: $_";
}

print "FINAL SCORE: $total_score\n";

