<script>
function changePeriod(url)
{
    location.href = 'income.php?period=' + url;
}
</script>


<div align=center class="pure-form">

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
<?php
if($_SESSION['permission']=='rw' and $personel_location_selected<>14)
echo '  
      <th colspan=10><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>';
?>
  </tr>
  <tr>
     <th>Day</th>
     <th>Location</th>
     <th>Guest Name</th>
     <th>Activity</th>
     <th>Bill No.</th>
     <th>Amount</th>
     <th>Payment type</th>
     <th>Payment</th>
     <th>Still to pay</th>
     <th></th>
  </tr>
 </thead>

<?php

setlocale(LC_MONETARY,'id_ID');


if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';


$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
                              activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no, cash_book.currency,
                              location_groups.name as loc_grp_name, DAY(cash_book.date) as dzien,

                              cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
                              cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.value,
                              GROUP_CONCAT(activity.shortcut) as shortcut

                              FROM cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              ".$sel_loc_pers."
                              ".$location_limit."
                              ".$period_limit."
                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and activity.id_activity=cash_book_item.activity_id
                              and cash_book_item.cash_book_id=cash_book.id_cash_book
                              and location.id_location=cash_book.location_id
                              and location.location_groups_id=location_groups.id_location_groups
                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date desc");
$stmt -> execute();

$row_array = $stmt->fetchAll();

$sum_value = '';
$sum_total_pay = '';
$sum_still_to_pay = '';

foreach($row_array as $row)
   {

   $payment_type = '';
   if($row['pay_cash']<>0)
      $payment_type .= 'cash,';
   if($row['pay_creditcard']<>0)
      $payment_type .= 'creditcard,';
   if($row['pay_roomcharge']<>0)
      $payment_type .= 'roomcharge,';
   if($row['pay_bank']<>0)
      $payment_type .= 'bank,';
   if($row['pay_paypal']<>0)
      $payment_type .= 'paypal,';

   if($row['value']==0)
      $value_error_color = ' style="background-color:#FFCCCB"';
   else
      $value_error_color = '';

   $still_to_pay = $row['value']-$row['total_pay'];

   echo '<tr>
   <td align=center>'.$row['dzien'].'</td>
   <td>'.$row['loc_name'].'</td>
   <td>'.$row['desc'].'</td>
   <td>'.$row['shortcut'].'</td>
   <td>'.$row['bill_no'].'</td>
   <td align=right'.$value_error_color.'>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>
   <td align=center>'.$payment_type.'</td>
   <td align=right>'.$formatter->formatCurrency($row['total_pay'], 'IDR').'</td>
   <td align=right>'.$formatter->formatCurrency($still_to_pay, 'IDR').'</td>
   <td>';

	if($_SESSION['permission']=='rw' and $row['boat_trip_id']<>0)
		echo '<a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit" aria-hidden="true"></i>[e]</a>';
	if($_SESSION['permission']=='rw' and $row['boat_trip_id']==0)
		echo '<a href="?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit" aria-hidden="true"></i>[e]</a>';

   echo '</td>
   </tr>';

   $sum_value += $row['value'];
   $sum_total_pay += $row['total_pay'];
   $sum_still_to_pay += $still_to_pay;

   }


?>
<tr>
  <td colspan=5></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
  <td></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_total_pay, 'IDR'); ?></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_still_to_pay, 'IDR'); ?></td>
  <td></td>
</tr>

</table>
