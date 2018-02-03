#!/usr/bin/perl -w

use Sys::Hostname;
use Socket;

####functions
#turns a number to an IP (dot-decimal notation)
sub pickAddress
{
    my $A = $_[0];
    my $aux;
    my $S="";
   
    $aux=$A & 4278190080;#first byte
    $aux=$aux>>24;
    $S=int($aux).".";#127.
   
    $aux=$A & 16711680;#second byte
    $aux=$aux>>16;
    $S=$S.int($aux).".";#127.0.

    $aux=$A & 65280;#third byte
    $aux=$aux>>8;
    $S=$S.int($aux).".";#127.0.0.
   
    $aux=$A & 255;
    $S=$S.int($aux);#127.0.0.1

    return $S;
}

#returns up if the host answer echo requests
sub state
{
    my $IP = $_[0];
    my $line;
    my $rtt;
    @SALIDA=`ping -nfq -w 1 -c 2 $IP`;
    while ($line=pop(@SALIDA))
    {
        if($line=~/0 received/)
        {
            return extraTest($IP);
        }
    }
    $rtt="UP";
    return $rtt;
}

#a running host might not answer echo requests, so lets try some common open ports.
sub extraTest
{
    my $IP=$_[0];
    @SALIDA=`nmap $IP -p T:25,22,21,53,80,110,443,U:53 2>/dev/null`;
    while ($line=pop(@SALIDA))
    {
        if($line=~/0 hosts up/)
        {
            $rtt="DOWN";
            return $rtt;#so it seems to be down... any other test?
        }
    }
    $rtt="UP";
    return $rtt;
}
####end of functions

####main code
if($< != 0)
{
    print STDERR "You must be root!\n";
    exit(-3);
}

$NET_ADDR="";
$NET_MASK="";
$HOSTS="";
$FIRST="";
$LAST="";

$argc=@ARGV;
if($argc < 1 or $argc > 2)
{
    print STDERR "USE: script [mask_bits]\n";
    exit(-1);
}

$NET_ADDR=$ARGV[0];
$NET_MASK=$ARGV[1];

#is it a real IP address?
if($NET_ADDR!~/^([\d]+)\.([\d]+)\.([\d]+)\.([\d]+)$/)
{
    print STDERR "Invalid network address. Example: 192.168.0.1\n";
    exit(-4);
}

#valid mask?
if($argc == 2 and ($NET_MASK > 30 || $NET_MASK < 1))
{
    print STDERR "Invalid mask. Mask [1 .. 30]\n";
    exit(-2);
}


#from 127.0.0.1 to 01111111 0000000 00000000 00000001
@L=split('\.',$NET_ADDR);
$NET_ADDR=$L[0]<<24;
$NET_ADDR=$NET_ADDR | $L[1]<<16;
$NET_ADDR=$NET_ADDR | $L[2]<<8;
$NET_ADDR=$NET_ADDR | $L[3];

if($argc == 2)
{#subnet check
    $HOSTS=2**(32-$NET_MASK) - 2;
   
    $aux=$NET_MASK;
    $NET_MASK=1;
    for($i=0;$i<32;$i++)
    {
        if($aux>0)
        {
            $NET_MASK=$NET_MASK<<1;
            $NET_MASK=$NET_MASK | 1;   
            $aux-=1;
        }
        else
        {
            $NET_MASK=$NET_MASK<<1;
        }
    }
    #now the real mask is ready, so i get the real net addr just in case
    $NET_ADDR=$NET_MASK & $NET_ADDR;
    $FIRST=$NET_ADDR+1;
    $LAST=$NET_ADDR+$HOSTS;
    print "NET ADDRESS: ".pickAddress($NET_ADDR).". NET MASK: ".pickAddress($NET_MASK).". HOSTS: $HOSTS. FIRST: ".pickAddress($FIRST).". LAST: ".pickAddress($LAST).".\n";

    $counter=1;

    while($counter<=$HOSTS)
    {
        $next_addr=pickAddress($NET_ADDR+$counter);
   
        $name=gethostbyaddr(inet_aton($next_addr),AF_INET);
   
        if(!$name)
        {
            $name="NONAME";
        }
   
        print $next_addr." is ".state($next_addr)." name $name\n";   
   
        $counter+=1;
    }
}
else
{#single host check
    $next_addr=pickAddress($NET_ADDR);
    $name=gethostbyaddr(inet_aton($next_addr),AF_INET);
    if(!$name)
    {
        $name="NONAME";
    }
    print $next_addr." is ".state($next_addr)." name $name\n";   
}
