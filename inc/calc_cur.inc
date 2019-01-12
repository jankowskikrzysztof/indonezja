<?php

/*
$stmt = $dbh->prepare("SELECT cur_rate.*
                              FROM cur_rate");
$stmt -> execute();
$cur_array = $stmt->fetchAll();

foreach($cur_array as $row)
   {
   $rates[$row['currency']][$row['date']] = $row['value'];
   }

echo '<pre>';
print_r($rates['USD']);
echo '</pre>';
*/


function conv_currency($value,$from_to,$currency,$date,$dbh)
         {
         $select = "SELECT cur_rate.*
                              FROM cur_rate
                              WHERE currency='".$currency."'
                              and date<='".$date."'
                              ORDER BY date desc
                              LIMIT 1";
         //echo $select.'<br>';
         $stmt = $dbh->prepare($select);
         $stmt -> execute();
         $cur_array = $stmt->fetchAll();
         foreach($cur_array as $row)
            {
            $cur_value = $row['value'];
            //echo $cur_value.' - '.$row['date'].'<br>';
            }


         if($from_to == 'to' and $currency <> 'IDR')
            $value = $value / $cur_value;
         if($from_to == 'from' and $currency <> 'IDR')
            $value = $value * $cur_value;

         if($currency == 'IDR')
            $value = $value;


         return round($value,2);
         }

/// JAK UZYWAC:

//$change_value = 1000000;

//echo $change_value.' = '.conv_currency($change_value,'to',USD,'2018-05-08',$dbh).'<br>';

//$change_value = 1;

//echo $change_value.' = '.conv_currency($change_value,'from',USD,'2018-05-08',$dbh).'<br>';

//$change_value = 1000000;

//echo $change_value.' = '.conv_currency($change_value,'to',IDR,'2018-05-08',$dbh).'<br>';


?>