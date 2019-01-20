<?php

include('/var/www/scuba.gruvi.pl/inc/db_config.inc');

function convertCurrency($amount, $from, $to){
  $conv_id = "{$from}_{$to}";
  $string = file_get_contents("http://free.currencyconverterapi.com/api/v3/convert?q=$conv_id&compact=ultra");
  $json_a = json_decode($string, true);

  return $amount * round($json_a[$conv_id], 4);
}

$usd_curr = convertCurrency(1, "USD", "IDR");
$pln_curr = convertCurrency(1, "PLN", "IDR");
$eur_curr = convertCurrency(1, "EUR", "IDR");



if($usd_curr<>0)
        {
        $stmt = $dbh->prepare("INSERT INTO `cur_rate`
                                  (`date`,
                                   `currency`,
                                   `value`)

                           VALUES ('".date('Y-m-d')."',
                                   'USD',
                                   '".$usd_curr."'
                                   )
                           ");
        $stmt -> execute();
        }

if($pln_curr<>0)
        {
        $stmt = $dbh->prepare("INSERT INTO `cur_rate`
                                  (`date`,
                                   `currency`,
                                   `value`)

                           VALUES ('".date('Y-m-d')."',
                                   'PLN',
                                   '".$pln_curr."'
                                   )
                           ");
        $stmt -> execute();
        }

if($eur_curr<>0)
        {
        $stmt = $dbh->prepare("INSERT INTO `cur_rate`
                                  (`date`,
                                   `currency`,
                                   `value`)

                           VALUES ('".date('Y-m-d')."',
                                   'EUR',
                                   '".$eur_curr."'
                                   )
                           ");
        $stmt -> execute();
        }


?>