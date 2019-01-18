<script src="https://unpkg.com/sticky-table-headers"></script>


      <style>
         tr {
             font-size: 80%;
         }
      </style>

<script>
function changePeriod(url)
{
    location.href = 'invoice.php?action=<?php echo $action; ?>&period=' + url;
}
</script>

<?php

 include('inc/calc_cur.inc');


 $cur = $_GET['cur'];
 if($cur <> '') $show_currency = $cur; else $show_currency = 'IDR';

 $period = $_GET['period'];

 $starting_year  =date('Y', strtotime('-3 year'));
 $ending_year = date('Y', strtotime('+1 year'));
 $current_year = date('Y');
 for($starting_year; $starting_year <= $ending_year; $starting_year++)
     {
     $yr_option .= '<option value="'.$starting_year.'"';
     if( ($starting_year ==  $current_year and $period == '') or $period == $starting_year)
         {
            $yr_option .= ' selected="selected"';
         }
     $yr_option .= ' >'.$starting_year.'</option>';
     }
?>

<div align=center class="pure-form">
<select name=period onchange="changePeriod(this.value)">
<?php echo $yr_option; ?>
</select>
<br><br>
</div>


  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<!--  <tr>
      <th colspan=6><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr>
    <th colspan=16 align=right>All values in <?php echo $show_currency; ?>. Show currency in: <a href="invoice.php?action=emp&period=<?php echo $period_year; ?>&cur=USD">USD</a>, <a href="invoice.php?action=emp&period=<?php echo $period_year; ?>&cur=EUR">EUR</a>, <a href="invoice.php?action=emp&period=<?php echo $period_year; ?>&cur=PLN">PLN</a> or <a href="invoice.php?action=emp&period=<?php echo $period_year; ?>">IDR</a></th>
  </tr>
-->
<tr align=center>
     <th>Trip</th>
     <th>Booker</th>
     <th>Date</th>
     <th>Invoice No</th>
     <th>Public<br>rate</th>
     <th>Commission</th>
     <th>Due to Pay</th>
     <th>Settlements</th>
     <th>Still<br>to Pay</th>
     <th>Payment<br>type</th>
     <th>Reference</th>
     <th>Client</th>
  </tr>
 </thead>

<?php


//$period_year = '2018';



$stmt = $dbh->prepare("SELECT cash_book.*,
                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
                              client.name as client_name,
                              GROUP_CONCAT(activity.shortcut) as shortcut,
                              booker.name as booker_name,
                              boat_trip.txt_route,
                              boat_trip.date_from,
                              boat_trip.date_to,
                              GROUP_CONCAT(settlements.id_settlements) as settlements, 
                              (SELECT SUM(value) FROM settlements WHERE settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book') as set_value

                              FROM (cash_book, cash_book_item)
                              left join client on client.id_client=cash_book.client_id
                              left join activity on activity.id_activity=cash_book_item.activity_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              
                              left join settlements on settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book'

                              WHERE
                                   cash_book.type=1
                              and cash_book.location_id=14
                              and cash_book_item.cash_book_id=cash_book.id_cash_book

                              and client_id<>0

                              AND YEAR(`cash_book`.`date`) = '".$period_year."'


                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   //conv_currency($change_value,'to',USD,'2018-05-08',$dbh)



   /// VALUES


   //echo "conv_currency( ".$row['value']." - ".$row['total_pay'].",'to', ".$show_currency." , ".date('Y-m-d').")<br>";

   /*
   $still_to_pay = conv_currency($row['value']-$row['total_pay'],'to',$show_currency,date('Y-m-d'),$dbh);
   $value_before_discount = conv_currency($row['value_before_discount'],'to',$show_currency,$row['date'],$dbh);
   $booker_commission = conv_currency($row['booker_commission'],'to',$show_currency,$row['date'],$dbh);
   $due_to_pay = conv_currency($row['value'],'to',$show_currency,$row['date'],$dbh);
   */

  $show_currency = 'IDR';
  $value_before_discount = $row['value_before_discount_currency'];
  $booker_commission = $row['booker_commission_currency'];
  $due_to_pay = $row['value_currency'];
  $still_to_pay = $row['value_currency']-$row['set_value'];


    $payment_type = '';
    if($row['pay_cash']<>0)
        {
        $still_to_pay = $still_to_pay - $row['pay_cash_currency'];
        $payment_type .= 'cash,';
        }
    if($row['pay_creditcard']<>0)
        {
        $payment_type .= 'creditcard,';
        $still_to_pay = $still_to_pay - $row['pay_creditcard_currency'];
        }
    if($row['pay_roomcharge']<>0)
        $payment_type .= 'roomcharge,';
    if($row['pay_bank']<>0)
        $payment_type .= 'bank,';
    if($row['pay_paypal']<>0)
        $payment_type .= 'paypal,';

   
   /// END VALUES

   echo '<tr>
   <td><a href="report_empress.php?action=trip_result&id_boat_trip='.$row['boat_trip_id'].'">'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'<br>'.$row[shortcut].'</a></td>
   <td>'.$row['booker_name'].'</td>
   <td>'.$row['date'].'</td>
   <td><a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['bill_no'].'</td>
   <td class=cash>'.$formatter->formatCurrency($value_before_discount,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($booker_commission,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($due_to_pay,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($row['set_value'],$show_currency).'</td>   
   <td class=cash>'.$formatter->formatCurrency($still_to_pay,$show_currency).'</td>
   <td class=cash>'.$payment_type.'</td>
   <td align=center>'.$row['booking_no'].'</td>
   <td><a href="client.php?action=edit&id='.$row['client_id'].'">'.$row['client_name'].'</a></td>
   </tr>';



   }

?>
</table>

<script>
    $('table').stickyTableHeaders();
</script>