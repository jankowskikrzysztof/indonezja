      <style>
         tr {
             font-size: 70%;
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
-->
  <tr>
    <th colspan=15 align=right>All values in <?php echo $show_currency; ?>. Show currency in: <a href="invoice.php?action=emp2&period=<?php echo $period_year; ?>&cur=USD">USD</a>, <a href="invoice.php?action=emp2&period=<?php echo $period_year; ?>&cur=EUR">EUR</a>, <a href="invoice.php?action=emp2&period=<?php echo $period_year; ?>&cur=PLN">PLN</a> or <a href="invoice.php?action=emp2&period=<?php echo $period_year; ?>">IDR</a></th>
  </tr>
  <tr align=center>
     <th>Trip</th>
     <th>Booker</th>
     <th>Date</th>
     <th>Invoice No</th>
     <th>Public<br>rate</th>
     <th>Commission</th>
     <th>Due to Pay</th>
     <th>Pay Cash<br>Pay Bank</th>
     <th>Deposit</th>
     <th>Deposit 2</th>
     <th>Deposit 3</th>
     <th>Deposit 4</th>
     <th>Still<br>to Pay</th>
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
                              boat_trip.date_to

                              FROM (cash_book, cash_book_item)
                              left join client on client.id_client=cash_book.client_id
                              left join activity on activity.id_activity=cash_book_item.activity_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id

                              WHERE
                                   cash_book.type=1
                              and cash_book.location_id=21
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

   $still_to_pay = conv_currency($row['value']-$row['total_pay'],'to',$show_currency,date('Y-m-d'),$dbh);

   $value_before_discount = conv_currency($row['value_before_discount'],'to',$show_currency,$row['date'],$dbh);

   $booker_commission = conv_currency($row['booker_commission'],'to',$show_currency,$row['date'],$dbh);

   $due_to_pay = conv_currency($row['value'],'to',$show_currency,$row['date'],$dbh);

   $pay_cash_part = conv_currency($row['pay_cash_part'],'to',$show_currency,$row['date'],$dbh);
   $pay_bank_part = conv_currency($row['pay_bank_part'],'to',$show_currency,$row['date'],$dbh);

   $pay_cash_deposit = conv_currency($row['pay_cash_deposit'],'to',$show_currency,$row['date_pay_deposit'],$dbh);
   $pay_cash_deposit2 = conv_currency($row['pay_cash_deposit2'],'to',$show_currency,$row['date_pay_deposit2'],$dbh);
   $pay_cash_deposit3 = conv_currency($row['pay_cash_deposit3'],'to',$show_currency,$row['date_pay_deposit3'],$dbh);
   $pay_cash_deposit4 = conv_currency($row['pay_cash_deposit4'],'to',$show_currency,$row['date_pay_deposit4'],$dbh);

   $pay_bank_deposit = conv_currency($row['pay_bank_deposit'],'to',$show_currency,$row['date_pay_deposit'],$dbh);
   $pay_bank_deposit2 = conv_currency($row['pay_bank_deposit2'],'to',$show_currency,$row['date_pay_deposit2'],$dbh);
   $pay_bank_deposit3 = conv_currency($row['pay_bank_deposit3'],'to',$show_currency,$row['date_pay_deposit3'],$dbh);
   $pay_bank_deposit4 = conv_currency($row['pay_bank_deposit4'],'to',$show_currency,$row['date_pay_deposit4'],$dbh);


   /// END VALUES


   if($row['date_pay_deposit']<>'0000-00-00') $date_dep1 = $row['date_pay_deposit'].'<br>'; else $date_dep1 = '';
   if($row['date_pay_deposit2']<>'0000-00-00') $date_dep2 = $row['date_pay_deposit2'].'<br>'; else $date_dep2 = '';
   if($row['date_pay_deposit3']<>'0000-00-00') $date_dep3 = $row['date_pay_deposit3'].'<br>'; else $date_dep3 = '';
   if($row['date_pay_deposit4']<>'0000-00-00') $date_dep4 = $row['date_pay_deposit4'].'<br>'; else $date_dep4 = '';

   echo '<tr>
   <td><a href="report_empress.php?action=trip_result&id_boat_trip='.$row['boat_trip_id'].'">'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'<br>'.$row[shortcut].'</a></td>
   <td>'.$row['booker_name'].'</td>
   <td>'.$row['date'].'</td>
   <td><a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['bill_no'].'</td>
   <td class=cash>'.$formatter->formatCurrency($value_before_discount,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($booker_commission,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($due_to_pay,$show_currency).'</td>
   <td class=cash valign=bottom>'.$formatter->formatCurrency($pay_cash_part,$show_currency).'<br>'.$formatter->formatCurrency($pay_bank_part,$show_currency).'</td>
   <td class=cash valign=bottom>'.$date_dep1.$formatter->formatCurrency($pay_cash_deposit,$show_currency).'<br>'.$formatter->formatCurrency($pay_bank_deposit,$show_currency).'</td>
   <td class=cash valign=bottom>'.$date_dep2.$formatter->formatCurrency($pay_cash_deposit2,$show_currency).'<br>'.$formatter->formatCurrency($pay_bank_deposit2,$show_currency).'</td>
   <td class=cash valign=bottom>'.$date_dep3.$formatter->formatCurrency($pay_cash_deposit3,$show_currency).'<br>'.$formatter->formatCurrency($pay_bank_deposit3,$show_currency).'</td>
   <td class=cash valign=bottom>'.$date_dep4.$formatter->formatCurrency($pay_cash_deposit4,$show_currency).'<br>'.$formatter->formatCurrency($pay_bank_deposit4,$show_currency).'</td>
   <td class=cash>'.$formatter->formatCurrency($still_to_pay,$show_currency).'</td>
   <td align=center>'.$row['booking_no'].'</td>
   <td><a href="client.php?action=edit&id='.$row['client_id'].'">'.$row['client_name'].'</a></td>
   </tr>';


   }

?>
</table>
