#!/usr/bin/perl -w

open STDIN,"tcpdump -n -s 1024 -tt dst host HOST_IP and dst port 514 2>/dev/null |" or die "Oops";
open STDOUT,">>/dev/null";
open STDERR,">>/dev/null";

$t1=time();
$t2=$t1+300;

#Imprime en "valores" un historico con el siguiente formato
#fecha y hora
#mensajes -> host
#mensajes -> host
#...
#Ademas escribe en total_logs, el valor logs.value mensajes
#este archivo es levantado por munin para graficar la cantidad
#de mensajes recibidos en los ultimos 5 minutos
sub ver_valores
{
        my $i;
        my $s="";
        my $x=0;
        foreach $i (sort (keys %LOGS))
        {
                $x=$x+$LOGS{$i};
                $s=$s."$LOGS{$i} -> $i\n";
        }
        open(F,">>valores");
        print F localtime()."\n$s\n\n";
        close(F);
        open(F,">total_logs");
        print F "logs.value $x";
        close(F);
}


#Pone en 0 todos los valores del hash
sub reiniciar
{
        foreach $i (keys %LOGS)
        {
                $LOGS{$i}=0;
        }
}

while()
{
        my @a=split(' ',$_);
        my @b=split('\.',$a[2]);
        my $c="$b[0].$b[1].$b[2].$b[3]";
        if(defined $LOGS{$c})
        {
                $LOGS{$c}=$LOGS{$c}+1;
        }
        else
        {
                $LOGS{$c}=1;
        }
        $t1=time();
        if($t1 > $t2)#pasaron al menos 300 segundos
        {
                $t2=$t1+300;
                ver_valores();
                reiniciar();
        }
}
