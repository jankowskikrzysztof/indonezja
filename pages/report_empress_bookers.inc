<script>
function changePeriod(url)
{
    location.href = 'report_empress.php?action=bookers&period=' + url;
}
</script>

<script>
function changePeriodBook(url)
{
    location.href = 'report_empress.php?action=bookers&period=<?php echo $_GET['period'];?>&' + url;
}
</script>

<?php
$stmt = $dbh->prepare("SELECT booker.id_booker, booker.name as book_name
                              FROM booker

                              ORDER BY booker.name");
$stmt -> execute();
$booker_array = $stmt->fetchAll();
?>

<div align=center class="pure-form">

<input type=year value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>


  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
    <tr align=center>
     <th>Booker
              <div class="pure-form">
              <select onchange="changePeriodBook(this.value)">
                      <option value='?'>All</option>
     <?php
           foreach($booker_array as $row)
              {
              if($row['id_booker'] == $_GET['book_sel'])
                 $sel = 'selected';
              else
                 $sel = '';

              echo "<option value='book_sel=".$row['id_booker']."' ".$sel.">".$row['book_name']."</option>";

              }
     ?>

              </select></div></th>
     <th>Reference</th>
     <th>Trip</th>
     <th>Client</th>
     <th>Invoice No/Date</th>
     <th>Public rate</th>
     <th>Commission</th>
     <th>Due to Pay</th>
     <th>Still to Pay</th>
  </tr>
 </thead>

<?php


//$period_year = '2018';

if($_GET['book_sel'] <> '')
  $book_sel = 'and booker.id_booker ='.$_GET['book_sel'];
else
  $book_sel = '';


$stmt = $dbh->prepare("SELECT cash_book.*,
                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
                              client.name as client_name,
                              GROUP_CONCAT(activity.shortcut) as shortcut,
                              booker.name as booker_name,
                              boat_trip.txt_route,
                              boat_trip.date_from,
                              boat_trip.date_to,
                              (SELECT SUM(value_foreign) FROM settlements WHERE settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book') as set_value

                              FROM (cash_book, cash_book_item)
                              left join client on client.id_client=cash_book.client_id
                              left join activity on activity.id_activity=cash_book_item.activity_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id

                              WHERE
                                   cash_book.type=1
                              and cash_book.location_id=14
                              and cash_book_item.cash_book_id=cash_book.id_cash_book

                              and client_id<>0

                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and cash_book.booker_id<>0

                              ".$book_sel."

                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date, cash_book.bill_no");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   //$still_to_pay = $row['value']-$row['total_pay'];
   $still_to_pay = $row['value_currency']-$row['set_value'];
   if($row['pay_cash']<>0)
      $still_to_pay = $still_to_pay - $row['pay_cash_currency'];
   if($row['pay_creditcard']<>0)
      $still_to_pay = $still_to_pay - $row['pay_creditcard_currency'];

   if($row['currency_second'] <> '' and $row['currency_second'] <> '0.0' and $row['currency']<>$row['currency_second'])
      {
      $value_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['value_currency'], $row['currency_second']).' '.$row['currency_second'].'</font>';
      $value_before_discount_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['value_before_discount_currency'], $row['currency_second']).' '.$row['currency_second'].'</font>';
      $booker_commission_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['booker_commission_currency'], $row['currency_second']).' '.$row['currency_second'].'</font>';
      //$still_to_pay_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['value_currency']-$row['total_pay_currency'], $row['currency_second']).' '.$row['currency_second'];
      $still_to_pay_currency_v = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($still_to_pay, $row['currency_second']).' '.$row['currency_second'];
      }
    else
      {$value_currency=''; $still_to_pay_currency = ''; $value_before_discount_currency = ''; $booker_commission_currency = '';}

   echo '<tr>
   <td>'.$row['booker_name'].'</td>
   <td align=center>'.$row['booking_no'].'</td>
   <td><font style="font-size:80%;"><a href="report_empress.php?action=trip_result&id_boat_trip='.$row['boat_trip_id'].'">'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].' ['.$row[shortcut].']</a></font></td>
   <td><a href="client.php?action=edit&id='.$row['client_id'].'">'.$row['client_name'].'</a></td>
   <td align=center><a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['bill_no'].'</a><br>'.$row['date'].'</td>
   <td align=right>'.$formatter->formatCurrency($row['value_before_discount'],'IDR').' '.$row['currency'].$value_before_discount_currency.'</td>
   <td align=right>'.$formatter->formatCurrency($row['booker_commission'],'IDR').' '.$row['currency'].$booker_commission_currency.'</td>
   <td align=right>'.$formatter->formatCurrency($row['value'],'IDR').' '.$row['currency'].$value_currency.'</td>
   <td align=right>'.$row['currency'].$still_to_pay_currency_v.'</td>
   </tr>';
//$formatter->formatCurrency($still_to_pay,'IDR').' '.
   $sum_value_before_discount += $row['value_before_discount'];
   $sum_booker_commission += $row['booker_commission'];
   $sum_value += $row['value'];
   $sum_still_to_pay += $still_to_pay;

   }

   echo '<tr>
   <td colspan=5></td>
   <td align=center>'.$formatter->formatCurrency($sum_value_before_discount,'IDR').'</td>
   <td align=center>'.$formatter->formatCurrency($sum_booker_commission,'IDR').'</td>
   <td align=center>'.$formatter->formatCurrency($sum_value,'IDR').'</td>
   <td align=center>'.$formatter->formatCurrency($sum_still_to_pay,'IDR').'</td>
   </tr>';


?>

</table>
